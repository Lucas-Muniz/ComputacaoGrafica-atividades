#include "enemies.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cppitertools/itertools.hpp>
#include "abcg.hpp"

#include <iostream>
using namespace std;

void Enemies::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");
  

 float step = (2.0f/(m_horde.size()+1.0f));
 float pos;
 int index = 1;
 m_level += 1;
 for (auto &enemy : m_horde){
    pos = (index*step) - 1.0f;
    enemy.m_translation = glm::vec2{pos, 0.8f};
    enemy.m_dead = false;
    enemy.m_color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f};
    enemy.m_life = 4 + m_level;
    index += 1;
  }

  // clang-format off
  std::array<glm::vec2, 7> positions{
      // Ship body
      glm::vec2{0.0f, -15.5f}, glm::vec2{-11.5f, +12.5f},
      glm::vec2{0.0f, +07.75f}, glm::vec2{+11.5f, +12.5f},
      glm::vec2{0.0f, 15.5f}, glm::vec2{-05.75f, 10.125f},
      glm::vec2{+05.75f, 10.125f}
      };

  // Normalize
  for (auto &position :positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  std::array indices{0, 1, 2,
                     2, 3, 0,
                     2, 4, 5,
                     0, 4, 6};

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

void Enemies::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glm::vec4 dark_color{};  

  for (auto &enemy : m_horde){
      if(!enemy.m_dead){
        glUniform2f(m_translationLoc, enemy.m_translation.x, enemy.m_translation.y);
        glUniform1f(m_rotationLoc, enemy.m_rotation);
        dark_color = (1 - 0.5f)*enemy.m_color + 0.5f*glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
        glUniform4fv(m_colorLoc, 1, &dark_color.r);
        glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_INT, nullptr);
        glUniform2f(m_translationLoc, enemy.m_translation.x, enemy.m_translation.y);
        glUniform1f(m_rotationLoc, enemy.m_rotation);
        glUniform4fv(m_colorLoc, 1, &enemy.m_color.r);
        glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);
      }    
  }

  glBindVertexArray(0);

  glUseProgram(0);
}

void Enemies::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void Enemies::update(Ship &ship, const GameData &gameData, float deltaTime) {
  // Move
  glm::vec2 ship_center = ship.m_ship_center;
  float life_percentage;
  for (auto &enemy : m_horde){
    glm::vec2 direction{glm::vec2{ship_center.x - enemy.m_translation.x,
                                  ship_center.y - enemy.m_translation.y}};
    enemy.m_rotation = std::atan2(direction.y, direction.x) - 3*M_PI_2;
    if (gameData.m_input[static_cast<size_t>(Input::Left)])
        enemy.m_rotation = glm::wrapAngle(enemy.m_rotation - 4.0f * deltaTime);
    if (gameData.m_input[static_cast<size_t>(Input::Right)])
        enemy.m_rotation = glm::wrapAngle(enemy.m_rotation + 4.0f * deltaTime);

    life_percentage = enemy.m_life/(4.0 + m_level);
    if (life_percentage <= 0.0f){
      enemy.m_dead = true;
    } else if (life_percentage <= 0.25f){
      enemy.m_color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f};
    } else if(life_percentage <= 0.5f){
      enemy.m_color = glm::vec4{1.0f, 0.68f, 0.0f, 1.0f};
    } else if (life_percentage <= 0.75f) {
      enemy.m_color = glm::vec4{1.0f, 1.0f, 0.0f, 1.0f};
    }
  }
}
