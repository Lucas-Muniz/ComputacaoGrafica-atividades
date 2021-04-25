#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>

#include <iostream>
using namespace std;


void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP) m_dollySpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_DOWN) m_dollySpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_LEFT) m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT) m_truckSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_a) m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_d) m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_w) m_tiltSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_s) m_tiltSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if (ev.key.keysym.sym == SDLK_UP && m_dollySpeed > 0) m_dollySpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_DOWN && m_dollySpeed < 0) m_dollySpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_LEFT && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT && m_truckSpeed > 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_a &&  m_panSpeed < 0) m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_d && m_panSpeed > 0) m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_w &&  m_tiltSpeed < 0) m_tiltSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_s && m_tiltSpeed > 0) m_tiltSpeed = 0.0f;
  }

  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (ev.type == SDL_MOUSEMOTION) {
    m_trackBallLight.mouseMove(mousePosition);
  }
  if (ev.type == SDL_MOUSEBUTTONDOWN) {
    if (ev.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mousePress(mousePosition);
    }
  }
  if (ev.type == SDL_MOUSEBUTTONUP) {
    if (ev.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mouseRelease(mousePosition);
    }
  }
}

void OpenGLWindow::initializeGL() {
  glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);

  // Create programs
  for (const auto& name : m_shaderNames) {
    auto path{getAssetsPath() + "shaders/" + name};
    auto program{createProgramFromFile(path + ".vert", path + ".frag")};
    m_programs.push_back(program);
  }

  // Create skybox program
  auto path{getAssetsPath() + "shaders/" + m_skyShaderName};
  m_skyProgram = createProgramFromFile(path + ".vert", path + ".frag");

  // Load default model
  loadModel(getAssetsPath() + "sphere.obj");
  m_mappingMode = 2;  // "spherical" option

  for (int i = 0; i < 3; i++) {
    auto texture = abcg::opengl::loadTexture(getAssetsPath() + "maps/" + m_textureFiles.at(i));
    m_textures.push_back(texture);
  }

  m_bodies.initializeSystem(m_program, m_system);

  // Load cubemap
  m_model.loadCubeTexture(getAssetsPath() + "maps/cube/");

  resizeGL(getWindowSettings().width, getWindowSettings().height);
  initializeSkybox();
}

void OpenGLWindow::initializeSkybox() {
  // Generate VBO
  glGenBuffers(1, &m_skyVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyPositions), m_skyPositions.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_skyProgram, "inPosition")};

  // Create VAO
  glGenVertexArrays(1, &m_skyVAO);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_skyVAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  glEnableVertexAttribArray(positionAttribute);
  glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void OpenGLWindow::loadModel(std::string_view path, std::string file) {
  m_model.loadDiffuseTexture(getAssetsPath() + "maps/" + file);
  m_model.loadFromFile(path);
  m_model.setupVAO(m_programs.at(m_currentProgramIndex));

  // Use material properties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}

void OpenGLWindow::loadModel(Model& model, std::string_view path, std::string file) {
  model.loadDiffuseTexture(getAssetsPath() + "maps/" + file);
  model.loadFromFile(path);
  model.setupVAO(m_programs.at(m_currentProgramIndex));

  // Use material properties from the loaded model
  m_Ka = model.getKa();
  m_Kd = model.getKd();
  m_Ks = model.getKs();
  m_shininess = model.getShininess();
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  const auto program{m_programs.at(m_currentProgramIndex)};
  glUseProgram(program);

  // Get location of uniform variables
  GLint viewMatrixLoc{glGetUniformLocation(program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(program, "projMatrix")};
  GLint lightDirLoc{glGetUniformLocation(program, "lightDirWorldSpace")};
  GLint shininessLoc{glGetUniformLocation(program, "shininess")};
  GLint IaLoc{glGetUniformLocation(program, "Ia")};
  GLint IdLoc{glGetUniformLocation(program, "Id")};
  GLint IsLoc{glGetUniformLocation(program, "Is")};
  GLint KsLoc{glGetUniformLocation(program, "Ks")};
  GLint diffuseTexLoc{glGetUniformLocation(program, "diffuseTex")};
  GLint mappingModeLoc{glGetUniformLocation(program, "mappingMode")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);

  glUniform1i(diffuseTexLoc, 0);
  glUniform1i(mappingModeLoc, m_mappingMode);

  auto lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};

  glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);

  glUniform1f(shininessLoc, m_shininess);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);

  glUniform4fv(KsLoc, 1, &m_Ks.x);

  m_bodies.render(m_model, m_textures, m_viewMatrix, program, m_currentProgramIndex);

  if (m_currentProgramIndex == 0 || m_currentProgramIndex == 1) {
    renderSkybox();
  }
  
  glUseProgram(0);
}

void OpenGLWindow::renderSkybox() {
  glUseProgram(m_skyProgram);

  // Get location of uniform variables
  GLint viewMatrixLoc{glGetUniformLocation(m_skyProgram, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_skyProgram, "projMatrix")};
  GLint skyTexLoc{glGetUniformLocation(m_skyProgram, "skyTex")};

  updateSkyBoxWalls(m_camera.m_eye, 10.0f);
  glDeleteBuffers(1, &m_skyVBO);
  glDeleteVertexArrays(1, &m_skyVAO);
  initializeSkybox();

  // Set uniform variables
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  glUniform1i(skyTexLoc, 1);

  glBindVertexArray(m_skyVAO);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_model.getCubeTexture());

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, m_skyPositions.size());
  glDepthFunc(GL_LESS);

  glBindVertexArray(0);
  glUseProgram(0);
}

void OpenGLWindow::updateSkyBoxWalls(glm::vec3 eye, float distance){
  for (int i = 0; i < 6*6; i++){
    m_skyPositions[i] = eye + distance*m_skyPositions_standard[i];
  }
  
}

void OpenGLWindow::paintUI() {

  auto widgetSize{ImVec2(230, 230)};
  ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5,
                                 m_viewportHeight - widgetSize.y - 5));
  ImGui::SetNextWindowSize(widgetSize);
  auto flags{ImGuiWindowFlags_NoResize};
  ImGui::Begin("Controls", nullptr, flags);

  {
      static std::size_t currentShaderIndex{};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Shader", m_shaderNames.at(currentShaderIndex))) {
        for (auto index : iter::range(m_shaderNames.size())) {
          const bool isSelected{currentShaderIndex == index};
          if (ImGui::Selectable(m_shaderNames.at(index), isSelected))
            currentShaderIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      // Set up VAO if shader program has changed
      if (static_cast<int>(currentShaderIndex) != m_currentProgramIndex) {
        m_currentProgramIndex = currentShaderIndex;
        m_model.setupVAO(m_programs.at(m_currentProgramIndex));
      }
    }

  // Three Body system combo box
    {
      static std::size_t currentIndex{};
      std::vector<std::string> comboItems{"8 Figure", "Chaotic"};

      ImGui::PushItemWidth(100);
      if (ImGui::BeginCombo("System",
                            comboItems.at(currentIndex).c_str())) {
        for (auto index : iter::range(comboItems.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      if (currentIndex == 0) {
        if (m_system != 1){
          m_system_changed = true;
          m_system = 1;
        }  
      } else {
        if (m_system != 2){
          m_system_changed = true;
          m_system = 2;
        }
      }
    }

  // Create a slider to control the body mass
    {
      ImGui::PushItemWidth(widgetSize.x-80);
      int body1_mass{(int) m_bodies.m_bodies.at(0).m_mass};
      int body2_mass{(int) m_bodies.m_bodies.at(1).m_mass};
      int body3_mass{(int) m_bodies.m_bodies.at(2).m_mass};
      if (m_currentProgramIndex == 1){
        ImGui::SliderInt("Red", &body1_mass, 0, 10, "%d mass units");
        ImGui::SliderInt("Green", &body2_mass, 0, 10, "%d mass units");
        ImGui::SliderInt("Blue", &body3_mass, 0, 10, "%d mass units");
      } else {
        ImGui::SliderInt("Sun", &body1_mass, 0, 10, "%d mass units");
        ImGui::SliderInt("Earth", &body2_mass, 0, 10, "%d mass units");
        ImGui::SliderInt("Jupiter", &body3_mass, 0, 10, "%d mass units");
      }
      
      m_bodies.m_bodies.at(0).m_mass = (float) body1_mass;
      m_bodies.m_bodies.at(1).m_mass = (float) body2_mass;
      m_bodies.m_bodies.at(2).m_mass = (float) body3_mass;
      ImGui::PopItemWidth();
    }

  if (ImGui::Button("Initial Position", ImVec2(-1, 25))){
    m_camera.returnInitialPosition();
  }

  if (ImGui::Button("Restart", ImVec2(-1, 25))){
    m_restart_simulation = true;
  }

  ImGui::End();
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
  m_trackBallLight.resizeViewport(width, height);

}

void OpenGLWindow::terminateGL() { 
  for (const auto& program : m_programs) {
    glDeleteProgram(program);
  }
  terminateSkybox(); 
}

void OpenGLWindow::terminateSkybox() {
  glDeleteProgram(m_skyProgram);
  glDeleteBuffers(1, &m_skyVBO);
  glDeleteVertexArrays(1, &m_skyVAO);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  m_bodies.updateRK(deltaTime);

  if (m_restart_simulation){
    m_restart_simulation = false;
    m_bodies.restart();
    m_camera.returnInitialPosition();
  }

  if (m_system_changed){
     m_system_changed = false;
    m_bodies.initializeSystem(m_program, m_system);
  }

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.tilt(m_tiltSpeed * deltaTime);


  if (m_currentProgramIndex == 1){
    m_model.m_hasTexture = false;
  } else {
    m_model.m_hasTexture = true;
    m_bodies.m_bodies.at(0).m_sourceLight = true;
    m_shininess = 25.0f;
  }
}
