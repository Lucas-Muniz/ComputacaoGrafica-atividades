#ifndef THREEBODIES_HPP_
#define THREEBODIES_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "model.hpp"
#include "body.hpp"
#include "abcg.hpp"
#include <math.h>   

#include <list>

class OpenGLWindow;
class Model;
class Body;


class ThreeBodies {
 public:

  void initializeSystem(GLuint program, int system = 1);
  void updateRK(float deltaTime);
  void render(Model& model, GLuint modelMatrixLoc, GLuint colorLoc);
  void restart();

 private:
  friend OpenGLWindow;

  int m_current_system = 1;

  std::vector<Body> m_bodies;

  glm::vec3 m_massCenter{0.0f};

  void calculateMassCenter();
  void updateSystemPosition();
  void updateAccelerations();

  void setSystem1();
  void setSystem2();
};

#endif