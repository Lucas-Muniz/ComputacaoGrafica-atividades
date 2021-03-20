#ifndef FORCEFIELD_HPP_
#define FORCEFIELD_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"
#include "ship.hpp"

class OpenGLWindow;

class ForceField {
 public:
  void initializeGL(GLuint program);
  void paintGL(Ship &ship);
  void terminateGL();

  void update(const GameData &gameData);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  float m_fieldscale{1.6f};
  int m_circleSides{120};
  bool m_active{false};
  float m_activationTime{3.0f};

  abcg::ElapsedTimer m_activationTimer;
  abcg::ElapsedTimer m_forcefieldLoadingTimer;
};

#endif