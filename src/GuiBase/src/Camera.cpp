#pragma once

#include "Camera.h"

#include <sstream>


using namespace SynGUI;

std::shared_ptr<Camera> Camera::camera_ = nullptr;

//Called when window is resized to recalculate the matrices
void Camera::setWindowSize(glm::vec2 size) {

	//TODO: Need to fix camera position when window is resized
	//position.x += (-size.x + windowSize.x)/zoom.x;
	//position.y += (-size.y + windowSize.y )/ zoom.x;

	windowSize = size;
	proj = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f, -1.0f, 1.0f);
}

std::shared_ptr<Camera> Camera::GetInstance()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (camera_ == nullptr) {
		camera_ = std::make_shared<Camera>();
	}
	return camera_;
}

Camera::Camera() {
	
}

void Camera::deinit() {

	camera_ = nullptr;
	//delete this;
}

//Called automatically by GUIRenderer
void Camera::init() {
	position = glm::vec3(0.0f);
	zoom = glm::vec3(1.0f);

	//Setup projection matrix with 16:9 aspect ratio
	proj = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f, -1.0f, 1.0f);

	zoomMat = glm::scale(glm::mat4(1.0f), zoom);
	//zoomMat = glm::translate(glm::mat4(1.0f), center) * glm::scale(glm::mat4(1.0f), zoom) * glm::translate(glm::mat4(1.0f), -center);

	view = glm::translate(glm::mat4(1.0f), -position) * zoomMat;
	mvp = proj * view;
}

//Called automatically by GUIRenderer
void Camera::update() {

	//center = glm::vec3(windowSize.x/2.0f, windowSize.y/2.0f, 0.0f) + position;

	view = glm::translate(glm::mat4(1.0f), -position) * zoomMat;

	//zoomMat = glm::scale(glm::mat4(1.0f), zoom)
	
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

void Camera::OnMouseMove(std::shared_ptr<MouseEvent>  eventArgs) {

	if (panning) {
		glm::vec2 l = glm::inverse(view) * glm::vec4(eventArgs->rawDelta.x * 2.0f, eventArgs->rawDelta.y * 2.0f, 0.0f, 0.0f);
		position += zoom * glm::vec3(-l.x,-l.y,0.0f);
	}
}

void Camera::OnMouseDown(std::shared_ptr<MouseEvent> eventArgs) {

	if (eventArgs->button == MouseButton::RIGHT) {
		panning = true;
	}
}

void Camera::OnMouseUp(std::shared_ptr<MouseEvent>  eventArgs) {

	if (eventArgs->button == MouseButton::RIGHT) {
		panning = false;
	}
}

void Camera::OnMouseWheel(std::shared_ptr<MouseEvent>  eventArgs) {

	if (!panning) {
		zoom += eventArgs->wheelDelta / 480.0f;

		zoom = glm::clamp(zoom, 0.5f, 5.0f);

		glm::vec4 tempProj = zoomMat * glm::vec4(eventArgs->pos.x, eventArgs->pos.y, 0.0f, 1.0f);
		zoomMat = glm::translate(glm::mat4(1.0f), glm::vec3(eventArgs->pos.x, eventArgs->pos.y, 0.0f)) * glm::scale(glm::mat4(1.0f), zoom) * glm::translate(glm::mat4(1.0f), -glm::vec3(eventArgs->pos.x, eventArgs->pos.y, 0.0f));
		glm::vec4 tempProj1 = zoomMat * glm::vec4(eventArgs->pos.x, eventArgs->pos.y, 0.0f, 1.0f);

		glm::vec4 offset = tempProj1 - tempProj;

		position += glm::vec3(offset.x, offset.y, offset.z);
	}
}
