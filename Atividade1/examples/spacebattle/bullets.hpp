#ifndef BULLETS_HPP_
#define BULLETS_HPP_

#include <list>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "ship.hpp"
#include "enemies.hpp"

class OpenGLWindow;

enum class Origin { Player, Enemy };

class Bullets {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(Ship &ship, const GameData &gameData, float deltaTime);
  void update(Enemies &enemies, const GameData &gameData, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};

  struct Bullet {
    Origin m_origin{};
    bool m_dead{false};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
    glm::vec4 m_color{glm::vec4(1)};
  };

  float m_scale{0.015f};

  std::vector<Bullet> m_bullets;
};

#endif