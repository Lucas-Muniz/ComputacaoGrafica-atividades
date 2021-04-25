#include "body.hpp"
#include <math.h>   
#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>
using namespace std;


void Body::initializeGL(GLuint program){
    m_program = program;
    m_modelMatrixLoc = glGetUniformLocation(m_program, "modelMatrix");
    m_colorLoc = glGetUniformLocation(m_program, "color");
    computeModelMatrix();
}

void Body::render(Model& model, GLuint modelMatrixLoc, GLint normalMatrixLoc, glm::mat4 m_viewMatrix, GLint KaLoc, GLint KdLoc){
    computeModelMatrix();
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
    if (model.getHasTexture()){
        glm::vec4 Ka = {0.2f, 0.2f, 0.2f, 1.0f};
        glm::vec4 Kd = {0.8f, 0.8f, 0.8f, 1.0f};
        glUniform4fv(KaLoc, 1, &Ka.r);
        glUniform4fv(KdLoc, 1, &Kd.r);
    } else {
        auto Ka_body = 0.5f*m_color;
        glUniform4fv(KaLoc, 1, &Ka_body.r);
        glUniform4fv(KdLoc, 1, &m_color.r);
    }

    auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
    model.render();
}

void Body::render(Model& model, GLuint program, glm::mat4 m_viewMatrix){
    GLint modelMatrixLoc{glGetUniformLocation(program, "modelMatrix")};
    GLint normalMatrixLoc{glGetUniformLocation(program, "normalMatrix")};
    GLint KaLoc{glGetUniformLocation(program, "Ka")};
    GLint KdLoc{glGetUniformLocation(program, "Kd")};

    computeModelMatrix();
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
     if (m_sourceLight){
        glm::vec4 Ka = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec4 Kd = model.getKd();
        glUniform4fv(KaLoc, 1, &Ka.r);
        glUniform4fv(KdLoc, 1, &Kd.r);
     } else if (model.getHasTexture()){
        glm::vec4 Ka = model.getKa();
        glm::vec4 Kd = model.getKd();
        glUniform4fv(KaLoc, 1, &Ka.r);
        glUniform4fv(KdLoc, 1, &Kd.r);
    } else {
        auto Ka_body = 0.5f*m_color;
        glUniform4fv(KaLoc, 1, &Ka_body.r);
        glUniform4fv(KdLoc, 1, &m_color.r);
    }

    auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);
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

