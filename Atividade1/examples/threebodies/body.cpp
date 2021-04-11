#include "body.hpp"
#include <math.h>   
#include <iostream>
using namespace std;


void Body::initializeGL(GLuint program){
    m_program = program;
    m_modelMatrixLoc = glGetUniformLocation(m_program, "modelMatrix");
    m_colorLoc = glGetUniformLocation(m_program, "color");
}

void Body::render(Model& model, GLuint modelMatrixLoc, GLuint colorLoc){
    computeModelMatrix();
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
    glUniform4fv(colorLoc, 1, &m_color.r);
    model.render();
}

void Body::update(const Body& body, float deltaTime){
    updatePosition(deltaTime);
    updateVelocity(deltaTime);
    updateAcceleration(body);
}

void Body::update(float deltaTime){
    updatePosition(deltaTime);
    updateVelocity(deltaTime);
}

// Update position
void Body::updatePosition(float deltaTime){
    m_position += m_velocity*deltaTime;
}

// Update velocity
void Body::updateVelocity(float deltaTime){
    m_velocity += m_acceleration*deltaTime;
}

// Update acceleration
void Body::updateAcceleration(const Body& body){
    glm::vec3 r = body.m_position - m_position;
    float c = K*body.m_mass / pow(glm::length(r), 3.0f);
    m_acceleration += c*r;
}

glm::vec3 Body::getAcceleration(const Body body){
    glm::vec3 r = body.m_position - m_position;
    float c = K*body.m_mass / pow(glm::length(r), 3.0f);
    return c*r;
}

void Body::setAcceleration(glm::vec3 acc){
    m_acceleration = acc;
}


void Body::computeModelMatrix(){
  m_modelMatrix = glm::mat4(1.0);
  m_modelMatrix = glm::translate(m_modelMatrix, m_position);
  float mass_scale{0.05f};
  if (m_mass != 0){
      mass_scale = m_mass/10.0f;
  }
  m_modelMatrix = glm::scale(m_modelMatrix, mass_scale*m_scale);
}

