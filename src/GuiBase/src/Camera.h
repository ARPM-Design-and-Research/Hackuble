#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include <iostream>

#include "Event.h"

#include <memory>

/* The Camera class is a singleton class. So only one instance
* of the class should be present at all times. To get the instance, simply
* include this header and call Camera::GetInstance() to get the
* pointer to the instance.
* 
* The Camera class defines the view of the current canvas and calculates
* the transformation matrices for OpenGL accordingly.
* 
* You can set the current position of the camera, and zoom the camera.
* 
*/

namespace SynGUI {
	class Camera : public EventHandler {
	protected:

		static Camera* camera_;

		Camera();

		glm::vec2 windowSize;

		bool panning = false;
		glm::vec3 position;

		glm::vec2 prevMousePos;
	public:
		
		glm::vec3 zoom;

		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 zoomMat;

		glm::mat4 mvp;

		Camera(Camera& other) = delete;
		/**
		 * Singletons should not be assignable.
		 */
		void operator=(const Camera&) = delete;
		/**
		 * This is the static method that controls the access to the singleton
		 * instance. On the first run, it creates a singleton object and places it
		 * into the static field. On subsequent runs, it returns the client existing
		 * object stored in the static field.
		 */

		static Camera* GetInstance();

		void update();
		void init();
		void deinit();

		glm::mat4 getProjectionMatrix();
		glm::mat4 getMVP();
		glm::mat4 getViewMatrix();
		glm::mat4 getZoomMatrix();

		glm::vec3 getPosition();

		void setPosition(glm::vec3 _p);
		void setZoom(glm::vec3 zoom);

		void setWindowSize(glm::vec2 size);

		void OnMouseMove(std::shared_ptr<MouseEvent> eventArgs);
		void OnMouseDown(std::shared_ptr<MouseEvent>  eventArgs);
		void OnMouseUp(std::shared_ptr<MouseEvent>  eventArgs);
		void OnMouseWheel(std::shared_ptr<MouseEvent>  eventArgs);

		//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}



