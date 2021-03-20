#include "ship.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Ship::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2{0.0f, -0.8f};

  std::array<glm::vec2, 21> positions{
      // Ship body
      glm::vec2{0.0f, +15.5f}, glm::vec2{5.2f, 08.0f},
      glm::vec2{5.2f, 1.0f}, glm::vec2{15.5f, -11.0f},
      glm::vec2{15.5f, -15.5f}, glm::vec2{+06.0f, -10.0f},
      glm::vec2{+05.0f, -14.0f}, glm::vec2{-5.0f, -14.0f},
      glm::vec2{-06.0f, -10.0f}, glm::vec2{-15.5f, -15.5f},
      glm::vec2{-15.5f, -11.0f}, glm::vec2{-05.2f, 1.0f},
      glm::vec2{-05.2f, +08.0f},

      // Cannon left
      glm::vec2{-12.5f, -00.5f}, glm::vec2{-12.5f, -08.0f},
      glm::vec2{-09.5f, -08.0f}, glm::vec2{-09.5f, -00.5f},

      // Cannon right
      glm::vec2{+12.5f, -00.5f}, glm::vec2{+12.5f, -08.0f},
      glm::vec2{+09.5f, -08.0f}, glm::vec2{+09.5f, -00.5f}
      };

  // Normalize
  for (auto &position :positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  std::array indices{0, 1, 12,
                     1, 12, 11,
                     1, 2, 11,
                     11, 10, 9,
                     9, 11, 8,
                     8, 11, 5,
                     11, 2, 5,
                     8, 7, 5,
                     7, 5, 6,
                     2, 5, 4,
                     4, 3, 2,
                     // Cannons
                     13, 14, 15,
                     13, 15, 16,
                     17, 18, 19,
                     17, 19, 20
                     };

  // Generate VBO
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void Ship::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glUniform1f(m_rotationLoc, m_rotation);
  glUniform2f(m_translationLoc, m_translation.x, m_translation.y);

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glUniform2f(m_translationLoc, m_translation.x, m_translation.y);
  glDrawElements(GL_TRIANGLES, 15 * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);

  glUseProgram(0);
}

void Ship::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void Ship::update(const GameData &gameData, float deltaTime) {
  // Move
  if (gameData.m_input[static_cast<size_t>(Input::Left)]){
    if (m_translation.x >= -0.85f){
      m_translation += glm::vec2{-m_diagonal_velocity*deltaTime, 0.0f};
    }      
  }
  if (gameData.m_input[static_cast<size_t>(Input::Right)]){
    if (m_translation.x <= 0.85f){
      m_translation += glm::vec2{m_diagonal_velocity*deltaTime, 0.0f}; 
    }
  }
    

  // Wrap-around
  if (m_translation.x < -1.0f) m_translation.x += 2.0f;
  if (m_translation.x > +1.0f) m_translation.x -= 2.0f;
  if (m_translation.y < -1.0f) m_translation.y += 2.0f;
  if (m_translation.y > +1.0f) m_translation.y -= 2.0f;

  // Set enemy aim
  m_ship_center = m_translation;

}
