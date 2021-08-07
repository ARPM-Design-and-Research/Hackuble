#pragma once

#include "Camera.h"

#include <sstream>


using namespace SynGUI;

Camera* Camera::camera_ = nullptr;

//Called when window is resized to recalculate the matrices
void Camera::setWindowSize(glm::vec2 size) {

	//TODO: Need to fix camera position when window is resized
	//position.x += (-size.x + windowSize.x)/zoom.x;
	//position.y += (-size.y + windowSize.y )/ zoom.x;

	windowSize = size;
	proj = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f, -1.0f, 1.0f);
	center = glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f, 0.0f);
}

Camera* Camera::GetInstance()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (camera_ == nullptr) {
		camera_ = new Camera();
	}
	return camera_;
}

Camera::Camera() {
	
}

void Camera::deinit() {

	camera_ = nullptr;
	delete this;
}

//Called automatically by GUIRenderer
void Camera::init() {
	position = glm::vec3(0.0f);
	zoom = glm::vec3(1.0f);

	//Setup projection matrix with 16:9 aspect ratio
	proj = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f, -1.0f, 1.0f);

	center = glm::vec3(windowSize.x / 2.0f, windowSize.y / 2.0f, 0.0f);

	view = glm::translate(glm::mat4(1.0f), -position) * glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), zoom) * glm::translate(glm::mat4(1.0f), -center);
	mvp = proj * view;
}

//Called automatically by GUIRenderer
void Camera::update() {

	center = glm::vec3(windowSize.x/2.0f, windowSize.y/2.0f, 0.0f) + position;

	zoomMat = glm::scale(glm::mat4(1.0f), zoom);

	view = glm::translate(glm::mat4(1.0f), -position) * glm::translate(glm::mat4(1.0f), center) * zoomMat * glm::translate(glm::mat4(1.0f), -center);
	
	mvp = proj * view;
}

glm::mat4 Camera::getZoomMatrix() {
	return zoomMat;
}

glm::mat4 Camera::getProjectionMatrix() {
	return proj;
}

glm::mat4 Camera::getMVP() {
	return mvp;
}

glm::mat4 Camera::getViewMatrix() {
	return view;
}

void Camera::setPosition(glm::vec3 _p) {
	position = _p;
}

void Camera::setZoom(glm::vec3 _zoom) {
	zoom = _zoom;
}

glm::vec3 Camera::getPosition() {
	return position;
}

void Camera::OnMouseMove(MouseEvent* eventArgs) {

	if (panning) {
		position += glm::vec3(-eventArgs->delta.x, -eventArgs->delta.y,0.0f);
	}
}

void Camera::OnMouseDown(MouseEvent* eventArgs) {

	if (eventArgs->button == MouseButton::RIGHT) {
		panning = true;
	}
}

void Camera::OnMouseUp(MouseEvent* eventArgs) {

	if (eventArgs->button == MouseButton::RIGHT) {
		panning = false;
	}
}

void Camera::OnMouseWheel(MouseEvent* eventArgs) {
	zoom += eventArgs->wheelDelta/480.0f;
}

//Key callback is used for now to gather key events and move the camera accordingly
//void Camera::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	/*if (key == GLFW_KEY_W) {
		GetInstance()->position.y -= 10.0f/GetInstance()->zoom.x;
	}

	if (key == GLFW_KEY_S) {
		GetInstance()->position.y += 10.0f/GetInstance()->zoom.x;
	}

	if (key == GLFW_KEY_A) {
		GetInstance()->position.x -= 10.0f/GetInstance()->zoom.x;
	}

	if (key == GLFW_KEY_D) {
		GetInstance()->position.x += 10.0f/GetInstance()->zoom.x;
	}

	if (key == GLFW_KEY_Q) {
		GetInstance()->zoom += glm::vec3(0.1f)*GetInstance()->zoom.x;
	}

	if (key == GLFW_KEY_E) {
		GetInstance()->zoom -= glm::vec3(0.1f) * GetInstance()->zoom.x;
	}*/
