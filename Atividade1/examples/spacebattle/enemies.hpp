#ifndef ENEMIES_HPP_
#define ENEMIES_HPP_

#include <list>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "ship.hpp"

class OpenGLWindow;
class Asteroids;
class Bullets;
class StarLayers;

class Enemies {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void update(Ship &ship, const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;
  friend Asteroids;
  friend Bullets;
  friend StarLayers;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  int m_level{0};
  
  struct Enemy {
    int m_life{4};
    bool m_dead{false};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec4 m_color{glm::vec4{0.0f, 1.0f, 0.0f, 1.0f}};
    float m_rotation{0.0f};
  };

  float m_scale{0.07f};

  std::array<Enemy, 10> m_horde;

  abcg::ElapsedTimer m_bulletCoolDownTimer;
};

#endif