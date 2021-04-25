#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "model.hpp"
#include "camera.hpp"
#include "threebodies.hpp"
#include "trackball.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};
  float m_tiltSpeed{0.0f};

  Model m_model;

  ThreeBodies m_bodies{};
  int m_system{1};

  bool m_restart_simulation{false};
  bool m_system_changed{false};

  glm::mat4 m_viewMatrix{1.0f};

  TrackBall m_trackBallLight;

  std::vector<Model> m_models;
  std::vector<GLuint> m_textures;

  // Shaders
  std::vector<const char*> m_shaderNames{"texture", "blinnphong"};
  std::vector<const char*> m_textureShaderNames{"Sun", "Earth", "Jupiter"};
  std::vector<const char*> m_blinnphongShaderNames{"Red", "Green", "Blue"};
  std::vector<const char*> m_textureFiles{"sun.png", "worldmap.jpeg", "jupiter.jpg"};
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_Id{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_Is{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 m_Ka{0.1f, 0.1f, 0.1f, 1.0f};
  glm::vec4 m_Kd{0.7f, 0.7f, 0.7f, 1.0f};
  glm::vec4 m_Ks{1.0f, 1.0f, 1.0f, 1.0f};
  float m_shininess{25.0f};

  // Skybox
  const std::string m_skyShaderName{"skybox"};
  GLuint m_skyVAO{};
  GLuint m_skyVBO{};
  GLuint m_skyProgram{};

  // clang-format off
  const std::array<glm::vec3, 36>  m_skyPositions_standard{
    // Front
    glm::vec3{-1, -1, +1}, glm::vec3{+1, -1, +1}, glm::vec3{+1, +1, +1},
    glm::vec3{-1, -1, +1}, glm::vec3{+1, +1, +1}, glm::vec3{-1, +1, +1},
    // Back
    glm::vec3{+1, -1, -1}, glm::vec3{-1, -1, -1}, glm::vec3{-1, +1, -1},
    glm::vec3{+1, -1, -1}, glm::vec3{-1, +1, -1}, glm::vec3{+1, +1, -1},
    // Right
    glm::vec3{+1, -1, -1}, glm::vec3{+1, +1, -1}, glm::vec3{+1, +1, +1},
    glm::vec3{+1, -1, -1}, glm::vec3{+1, +1, +1}, glm::vec3{+1, -1, +1},
    // Left
    glm::vec3{-1, -1, +1}, glm::vec3{-1, +1, +1}, glm::vec3{-1, +1, -1},
    glm::vec3{-1, -1, +1}, glm::vec3{-1, +1, -1}, glm::vec3{-1, -1, -1},
    // Top
    glm::vec3{-1, +1, +1}, glm::vec3{+1, +1, +1}, glm::vec3{+1, +1, -1},
    glm::vec3{-1, +1, +1}, glm::vec3{+1, +1, -1}, glm::vec3{-1, +1, -1},
    // Bottom
    glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, -1}, glm::vec3{+1, -1, +1},
    glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, +1}, glm::vec3{-1, -1, +1}
  };
  // clang-format on

  std::array<glm::vec3, 36>  m_skyPositions{
    // Front
    glm::vec3{-1, -1, +1}, glm::vec3{+1, -1, +1}, glm::vec3{+1, +1, +1},
    glm::vec3{-1, -1, +1}, glm::vec3{+1, +1, +1}, glm::vec3{-1, +1, +1},
    // Back
    glm::vec3{+1, -1, -1}, glm::vec3{-1, -1, -1}, glm::vec3{-1, +1, -1},
    glm::vec3{+1, -1, -1}, glm::vec3{-1, +1, -1}, glm::vec3{+1, +1, -1},
    // Right
    glm::vec3{+1, -1, -1}, glm::vec3{+1, +1, -1}, glm::vec3{+1, +1, +1},
    glm::vec3{+1, -1, -1}, glm::vec3{+1, +1, +1}, glm::vec3{+1, -1, +1},
    // Left
    glm::vec3{-1, -1, +1}, glm::vec3{-1, +1, +1}, glm::vec3{-1, +1, -1},
    glm::vec3{-1, -1, +1}, glm::vec3{-1, +1, -1}, glm::vec3{-1, -1, -1},
    // Top
    glm::vec3{-1, +1, +1}, glm::vec3{+1, +1, +1}, glm::vec3{+1, +1, -1},
    glm::vec3{-1, +1, +1}, glm::vec3{+1, +1, -1}, glm::vec3{-1, +1, -1},
    // Bottom
    glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, -1}, glm::vec3{+1, -1, +1},
    glm::vec3{-1, -1, -1}, glm::vec3{+1, -1, +1}, glm::vec3{-1, -1, +1}
  };

  void initializeSkybox();
  void renderSkybox();
  void terminateSkybox();

  void updateSkyBoxWalls(glm::vec3 eye, float distance);

  void loadModel(std::string_view path, std::string file = "pattern.png");
  void loadModel(Model& model, std::string_view path, std::string file = "pattern.png");
  void update();
};

#endif