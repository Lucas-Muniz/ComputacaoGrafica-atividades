#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

using namespace std;

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_RCTRL || event.key.keysym.sym == SDLK_LCTRL)
      m_gameData.m_input.set(static_cast<size_t>(Input::Field));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
    if (event.key.keysym.sym == SDLK_f || event.key.keysym.sym == SDLK_RCTRL || event.key.keysym.sym == SDLK_LCTRL)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Field));
  }

  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Fire));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.set(static_cast<size_t>(Input::Field));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Fire));
    if (event.button.button == SDL_BUTTON_RIGHT)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Field));
  }
}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the stars
  m_starsProgram = createProgramFromFile(getAssetsPath() + "stars.vert",
                                         getAssetsPath() + "stars.frag");
  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  m_starLayers.initializeGL(m_starsProgram, 25);
  restart(State::Playing);
}

void OpenGLWindow::restart(State state) {
  m_gameData.m_state = State::Playing;

  m_ship.initializeGL(m_objectsProgram);
  m_enemies.initializeGL(m_objectsProgram, state);
  m_bullets.initializeGL(m_objectsProgram);
  m_forcefield.initializeGL(m_objectsProgram);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart(m_gameData.m_state);
    return;
  }

  m_ship.update(m_gameData, deltaTime);
  m_starLayers.update(deltaTime);
  m_enemies.update(m_ship, m_gameData, deltaTime);
  m_forcefield.update(m_gameData);
  m_ship.m_fieldactive = m_forcefield.m_active;
  m_bullets.update(m_ship, m_gameData, deltaTime);
  m_bullets.update(m_enemies, m_gameData, deltaTime);

  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }
}

void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_starLayers.paintGL();
  m_ship.paintGL(m_gameData);
  m_enemies.paintGL(m_gameData);
  m_bullets.paintGL();
  m_forcefield.paintGL(m_ship);
  
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    auto size{ImVec2(300, 85)};
    auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                         (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("You Win!");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_objectsProgram);

  m_bullets.terminateGL();
  m_ship.terminateGL();
  m_starLayers.terminateGL();
  m_enemies.terminateGL();
  m_forcefield.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // Check collision between bullets and ship
  if(!m_ship.m_fieldactive){
    for (auto &bullet : m_bullets.m_bullets) {
      if (bullet.m_dead || bullet.m_origin == Origin::Player) continue;

      auto distance{glm::distance(bullet.m_translation, m_ship.m_translation)};
      if (distance < m_bullets.m_scale + m_ship.m_scale * 0.85f) {
        m_gameData.m_state = State::GameOver;
        m_restartWaitTimer.restart();
        bullet.m_dead = true;
      }
    }
  }
  

  // Check collision between bullets and enemies
  for (auto &bullet : m_bullets.m_bullets) {
    if (bullet.m_dead || bullet.m_origin == Origin::Enemy) continue;

    for (auto &enemy : m_enemies.m_horde){
      if (!enemy.m_dead){
        auto distance{glm::distance(bullet.m_translation, enemy.m_translation)};
        if (distance < m_bullets.m_scale + m_enemies.m_scale * 0.85f) {
          enemy.m_life -= 1.0;
          bullet.m_dead = true;
        }
      }
      
    }    
  }

  // Check collision between bullets
  int size = m_bullets.m_bullets.size();
  if (size > 1){
    auto first_bullet = m_bullets.m_bullets.at(0);
    for (int i = 1; i < size; i++){
      auto bullet =  m_bullets.m_bullets.at(i);
      if (first_bullet.m_origin != bullet.m_origin){
        auto distance{glm::distance(first_bullet.m_translation, bullet.m_translation)};
        if (distance < 2*m_bullets.m_scale) {
          first_bullet.m_dead = true;
          bullet.m_dead = true;
        }
      }
    }
  }

  // Check collision between bullets and the force field
  if (m_forcefield.m_active){
    for (auto &bullet : m_bullets.m_bullets) {
      if (bullet.m_dead) continue;
      auto distance{glm::distance(bullet.m_translation, m_ship.m_translation)};
      if (distance < m_bullets.m_scale + m_forcefield.m_fieldscale * m_ship.m_scale) {
        bullet.m_dead = true;
      }
    }
  }
  
}

void OpenGLWindow::checkWinCondition() {
  auto horde = m_enemies.m_horde;
  if (std::all_of(horde.begin(), horde.end(), [](const Enemies::Enemy &e) { return e.m_dead; })) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}
