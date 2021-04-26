#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 15.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::returnInitialPosition(){
  m_eye = glm::vec3(0.0f, 0.0f, 2.5f);  
  m_at = glm::vec3(0.0f, 0.0f, 0.0f);
  m_up = glm::vec3(0.0f, 1.0f, 0.0f);
  computeViewMatrix();
}

void Camera::dolly(float speed) {
  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_at - m_eye);

  // Move eye and center forward (speed > 0) or backward (speed < 0)
  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_at - m_eye);
  // Compute vector to the left
  glm::vec3 left = glm::cross(m_up, forward);

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}

void Camera::pan(float speed) {
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}

void Camera::tilt(float speed){
  glm::mat4 transform{glm::mat4(1.0f)};
  glm::vec3 x_axis = glm::vec3(1.0f, 0.0f, 0.0f);

  // Rotate camera around its local x axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, x_axis);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}