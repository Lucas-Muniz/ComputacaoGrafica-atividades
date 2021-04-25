#ifndef BODY_HPP_
#define BODY_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "model.hpp"
#include "abcg.hpp"
#include <math.h>   
#include "trackball.hpp"


class OpenGLWindow;
class Model;
class ThreeBodies;
class Trajectory;


class Body {
 public:
  Body(float mass, glm::vec3 pos, glm::vec3 vel, glm::vec4 color = glm::vec4{1.0f}, bool light = false){
    m_mass = mass;
    m_position = pos;
    m_velocity = vel;
    m_color = color;
    m_sourceLight = light;
  }

  Body(){
    m_mass = 1.0f;
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
  }

  void initializeGL(GLuint program);
  void update(const Body& body, float deltaTime);
  void update(float deltaTime);
  void render(Model& model, GLuint modelMatrixLoc, GLint normalMatrixLoc, glm::mat4 m_viewMatrix,  GLint KaLoc, GLint KdLoc);
  void render(Model& model, GLuint program, glm::mat4 m_viewMatrix);
  glm::vec3 getAcceleration(const Body body);

 private:
  friend OpenGLWindow;
  friend ThreeBodies;

  float K{1.0f};

  float m_mass{1.0f};
  glm::vec3 m_position{0.0f};
  glm::vec3 m_velocity{0.0f};
  glm::vec3 m_acceleration{0.0f};

  glm::mat4 m_modelMatrix{1.0f};
  glm::vec3 m_scale{2.0f * glm::vec3(1.0f, 1.0f, 1.0f)};
  glm::vec4 m_color{glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)};

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_modelMatrixLoc{};

  bool m_sourceLight{false};


  void updatePosition(float deltaTime);
  void updateVelocity(float deltaTime);
  void updateAcceleration(const Body& body);
  void computeModelMatrix();

  void setAcceleration(glm::vec3 acc);
};

#endif