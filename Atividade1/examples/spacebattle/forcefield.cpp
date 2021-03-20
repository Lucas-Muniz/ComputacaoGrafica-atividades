#include "forcefield.hpp"

#include <cppitertools/itertools.hpp>
#include "abcg.hpp"

void ForceField::initializeGL(GLuint program){
    m_program = program;
    m_colorLoc = glGetUniformLocation(m_program, "color");
    m_rotationLoc = glGetUniformLocation(m_program, "rotation");
    m_scaleLoc = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");

    // Create geometry
    std::vector<glm::vec2> positions(0);
    positions.emplace_back(0.0f, 0.0f);
    auto step{2 * M_PI/ m_circleSides};
    for (auto angle : iter::range(0.0, M_PI * 2, step)) {
        positions.emplace_back(std::cos(angle), std::sin(angle));
    }
    positions.push_back(positions.at(1));

    // Generate VBO
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Get location of attributes in the program
    GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

    // Create VAO
    glGenVertexArrays(1, &m_vao);

    // Bind vertex attributes to current VAO
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(positionAttribute);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // End of binding to current VAO
    glBindVertexArray(0);
}

void ForceField::paintGL(Ship &ship){
  if (m_active){
    if(m_activationTimer.elapsed() < m_activationTime){
      glUseProgram(m_program);
      glBindVertexArray(m_vao);
      glUniform1f(m_rotationLoc, 0);
      glUniform1f(m_scaleLoc, m_fieldscale * ship.m_scale);
      glUniform2f(m_translationLoc, ship.m_translation.x,
                  ship.m_translation.y);

      glm::vec4 color;
      if (m_activationTimer.elapsed() < m_activationTime/3){
        color = glm::vec4{0.43f, 0.73f, 0.82f, 0.4f};
      } else if(m_activationTimer.elapsed() < m_activationTime*(2/3)) {
        color = glm::vec4{0.52f, 0.8f, 0.92f, 0.2f};
      } else {
        color = glm::vec4{0.83f, 0.93f, 0.97f, 0.1f};
      }

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glUniform4fv(m_colorLoc, 1, &color.r);

      glDrawArrays(GL_TRIANGLE_FAN, 0, m_circleSides+2);

      glDisable(GL_BLEND);

      glBindVertexArray(0);

      glUseProgram(0);
    } else{
      m_active = false;
      m_forcefieldLoadingTimer.restart();
    }
  } 

  
}

void ForceField::terminateGL(){
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

void ForceField::update(const GameData &gameData){
  // Create a pair of bullets
  if (gameData.m_input[static_cast<size_t>(Input::Field)] &&
      gameData.m_state == State::Playing) {
    // At least 5 seconds must have passed since the last force field activation
    if (m_forcefieldLoadingTimer.elapsed() > 5.0) {
      m_activationTimer.restart();
      m_active = true;
    }
  } else if (gameData.m_state != State::Playing){
    m_active = false;
  }
}