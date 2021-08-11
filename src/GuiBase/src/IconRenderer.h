#pragma once

#include <fstream>
#include <iostream>
#include "glm.hpp"
#include "Enum.h"
#include <string>
#include <map>
#include <vector>
#include "Error.h"
#include "Camera.h"

#include "Shaders.h"
#include "stb_image.h"



/* The IconRenderer class is a singleton class. So only one instance
* of the class should be present at all times. To get the instance, simply
* include this header and call IconRenderer::GetInstance() to get the
* pointer to the instance.
* 
* The IconRenderer handles all icon and texture related rendering. It requires
* to have one big Atlas texture with all textures combined into it for efficiency.
* The metadata to read into the atlas is also required in a specific .json format.
* This is to be placed in the icons/ folder of the application with the names
* iconAtlas.png and iconAtlas.json.
* 
* Currently the atlas has to be manually made outside the program. I have used open source
* software - AtlasBuilder by peteward44 : https://github.com/peteward44/atlasbuilder
* 
* I used it in standalone mode, but it is possible to integrate it within one program.
*/

namespace quicktype {
	class FrameValue;
};

namespace SynGUI {

	class Icon;

	class IconRenderer {
	private:
		IconInfo getIconInfo(const std::string& iconName);

		std::map<std::string, quicktype::FrameValue> iconMetadata;

	protected:
		static IconRenderer* iconRenderer_;
		IconRenderer();

		unsigned int atlasTextureBuffer;
		unsigned char* atlasBuffer;
		int bpp;

		unsigned int vao;
		unsigned int vbo;

		std::vector<float> iconVertices;
		std::vector<Icon*> icons;

		unsigned int iconShader;

		void addIconToBuffer(Icon* icon);

	public:

		IconRenderer(IconRenderer& other) = delete;
		/**
		 * Singletons should not be assignable.
		 */
		void operator=(const IconRenderer&) = delete;
		/**
		 * This is the static method that controls the access to the singleton
		 * instance. On the first run, it creates a singleton object and places it
		 * into the static field. On subsequent runs, it returns the client existing
		 * object stored in the static field.
		 */

		static IconRenderer* GetInstance();

		void init();
		void deinit();
		void render();

		Icon* addIcon(const std::string& iconName, glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size = glm::vec2(10.0f), Pivot pivot = Pivot::TOP_LEFT, float zDepth = 0.0f);

		void updateIcon(Icon* icon);

		glm::vec2 atlasSize;
	};
}
