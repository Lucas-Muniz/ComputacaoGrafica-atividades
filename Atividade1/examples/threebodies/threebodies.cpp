#include "threebodies.hpp"
#include <math.h>   

void ThreeBodies::initializeSystem(GLuint program, int system){
    if (system == 1){
        setSystem1();
        m_current_system = 1;
    } else if (system == 2){
        setSystem2();
        m_current_system = 2;
    } else {
        setSystem1();
        m_current_system = 1;
    }

    for(auto &body : m_bodies){
        body.initializeGL(program);
    }
}

// Update acceleration of all bodies
void ThreeBodies::updateAccelerations(){
    std::array<glm::vec3, 3> resultant_acc{{glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{0.0f}}};
    int num_bodies = (int) m_bodies.size();
    for (int i=0; i < num_bodies; i++){
        auto body = m_bodies[i];
        auto &acc = resultant_acc[i];
        for (int j=0; j < num_bodies; j++){
            if (i != j){
                acc += body.getAcceleration(m_bodies[j]);
            }
        }
    }

    for (int i=0; i < num_bodies; i++){
        auto &body = m_bodies[i];
        body.setAcceleration(resultant_acc[i]);
    }
}

// Numerical method based on Runge–Kutta method
void ThreeBodies::updateRK(float deltaTime){
    updateAccelerations();

    int num_bodies = (int) m_bodies.size();
    std::array<float, 4> b{{deltaTime/6, deltaTime/3, deltaTime/3, deltaTime/6}};
    std::array<glm::vec3, 3*2> s{};
    std::array<glm::vec3, 3*2> ds{};
    for(int b=0; b < num_bodies; b++){
        auto body = m_bodies[b];
        s[2*b] = body.m_position;
        s[2*b+1] = body.m_velocity;
        ds[2*b] = body.m_velocity;
        ds[2*b+1] = body.m_acceleration;
    }

    std::array<glm::vec3, 3*2> st{glm::vec3{0.0f}};

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < s.size(); i++) {
          st[i] = st[i] + b[j]*ds[i];
        }
    }

    for (int i = 0; i < s.size(); i++) {
        s[i] += st[i];
    }

    for(int b=0; b < num_bodies; b++){
        auto &body = m_bodies[b];
        body.m_position = s[2*b];
        body.m_velocity = s[2*b+1];
    }
}

void ThreeBodies::render(Model& model, GLuint modelMatrixLoc, GLuint colorLoc){
    for(auto &body : m_bodies){
        body.render(model, modelMatrixLoc, colorLoc);
    }
}

void ThreeBodies::restart(){
    if (m_current_system == 1){
        setSystem1();
    } else if (m_current_system == 2){
        setSystem2();
    } else {
        setSystem1();
    }
}

void ThreeBodies::calculateMassCenter(){
    float sumMass{0.0f}; 
    glm::vec3 weightedSum{0.0f};
    for(auto body : m_bodies){
        weightedSum += body.m_mass*body.m_position;
        sumMass += body.m_mass;
    }
    m_massCenter = weightedSum / sumMass;
}

void ThreeBodies::updateSystemPosition(){
    calculateMassCenter();
    for(auto &body : m_bodies){
        body.m_position -= m_massCenter;
    }

}

// 8 Figure configuration
void ThreeBodies::setSystem1(){
  glm::vec3 eightFigure_position{glm::vec3(0.97000436f, -0.24308753f, 0.0f)};
  glm::vec3 eightFigure_velocity{glm::vec3(-0.93240737f, -0.86473146f, 0.0f)};
  m_bodies.clear();
  m_bodies.push_back(Body(1.0f, eightFigure_position, -0.5f * eightFigure_velocity, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
  m_bodies.push_back(Body(1.0f, -1.0f * eightFigure_position, -0.5f * eightFigure_velocity, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  m_bodies.push_back(Body(1.0f, glm::vec3(0.0f, 0.0f, 0.0f), eightFigure_velocity, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
}

// Chaotic configuration
void ThreeBodies::setSystem2(){
  double angle1 = M_PI/2, angle2 = 2*M_PI/3;
  m_bodies.clear();
  m_bodies.push_back(Body(1.0f, glm::vec3(std::cos(0.0f), std::sin(0.0f), 0.0f), 0.5f*glm::vec3(std::cos(angle1), std::sin(angle1), 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
  m_bodies.push_back(Body(1.0f, glm::vec3(std::cos(angle2), std::sin(angle2), 0.0f), 0.5f*glm::vec3(std::cos(angle2 + angle1), std::sin(angle2 + angle1), 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  m_bodies.push_back(Body(1.0f, glm::vec3(std::cos(2*angle2), std::sin(2*angle2), 0.0f), 0.5f*glm::vec3(std::cos(2*angle2 + angle1), std::sin(2*angle2 + angle1), 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
}


