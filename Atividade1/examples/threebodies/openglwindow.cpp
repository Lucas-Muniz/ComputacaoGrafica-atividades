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
}

void OpenGLWindow::initializeGL() {
  glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = createProgramFromFile(getAssetsPath() + "depth.vert",
                                    getAssetsPath() + "depth.frag");

  // Load model
  m_model.loadFromFile(getAssetsPath() + "geosphere.obj", true);

  m_model.setupVAO(m_program);

  m_trianglesToDraw = m_model.getNumTriangles();

  m_bodies.initializeSystem(m_program, m_system);

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::paintGL() {
  update();

  // Clear color buffer and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(m_program);

  // Get location of uniform variables (could be precomputed)
  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  GLint modelMatrixLoc{glGetUniformLocation(m_program, "modelMatrix")};
  GLint colorLoc{glGetUniformLocation(m_program, "color")};

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);

  m_bodies.render(m_model, modelMatrixLoc, colorLoc);

  glUseProgram(0);
}

void OpenGLWindow::paintUI() {

  auto widgetSize{ImVec2(200, 200)};
  ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5,
                                 m_viewportHeight - widgetSize.y - 5));
  ImGui::SetNextWindowSize(widgetSize);
  auto flags{ImGuiWindowFlags_NoResize};
  ImGui::Begin("Controls", nullptr, flags);

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
      ImGui::SliderInt("Red", &body1_mass, 0, 10, "%d mass units");
      ImGui::SliderInt("Green", &body2_mass, 0, 10, "%d mass units");
      ImGui::SliderInt("Blue", &body3_mass, 0, 10, "%d mass units");
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

}

void OpenGLWindow::terminateGL() { glDeleteProgram(m_program); }

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
}
