#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include "glm.hpp"
#include "Error.h"
#include "Shaders.h"
#include "Camera.h"
#include "Enum.h"

#include "stb_image.h"


#include <optional>
#include <stdexcept>
#include <regex>

/* The TextRenderer class is a singleton class. So only one instance
* of the class should be present at all times. To get the instance, simply
* include this header and call TextRenderer::GetInstance() to get the
* pointer to the instance.
* 
* The TextRenderer handles all the textRendering operation. An atlas exist for the font
* and metadata is parsed from the JSON. The glyph atlas and metadata is created using another
* open source application : msdfAtlasGen by Chlumsky : https://github.com/Chlumsky/msdfgen
* 
* The atlas and metadata are in the fonts/ directory. The atlas and metadata are named as
* atlas.png and atlas.json.
* 
* 
* EnableGlyphBox and EnableBoundingBox can be used for debugging purposes.
*/

class TextLabel;

namespace SynGUI {

	class TextRenderer {

	private:
		//TODO: figure out how to sequentially put vertices
		std::vector<float> glyphVertices;
		void addTextToBuffer(TextLabel* label);
		void updateBuffer();

		int shader_mvp;
		int shader_texturePos;

		bool isUpdateBuffer = false;

	protected:

		static TextRenderer* textRenderer_;

		TextRenderer();

	public:

		TextRenderer(TextRenderer& other) = delete;
		/**
		 * Singletons should not be assignable.
		 */
		void operator=(const TextRenderer&) = delete;
		/**
		 * This is the static method that controls the access to the singleton
		 * instance. On the first run, it creates a singleton object and places it
		 * into the static field. On subsequent runs, it returns the client existing
		 * object stored in the static field.
		 */

		static TextRenderer* GetInstance();


		unsigned char* atlasBuffer;
		unsigned char* bitmap;

		unsigned int textShader;
		unsigned int debugShader;

		int atlasWidth;
		int atlasHeight;
		int bpp;

		unsigned int atlasTextureBuffer;
		unsigned int vao;
		unsigned int vbo;

		bool glyphDebug = false;

		bool bbDebug = false;
		unsigned int bbVAO;
		unsigned int bbVBO;
		std::vector<glm::vec4> bbVertices;


		AtlasInfo atlasInfo;

		//std::vector<GlyphInfo> glyphs;
		std::map<unsigned short, GlyphInfo> glyphs;
		std::vector<TextLabel*> labels;

		void init();
		void deinit();
		void render();

		TextLabel* addText(const std::string& text, float fontSize = 50.0f, glm::vec2 pos = glm::vec2(0.0f, 0.0f), Pivot _pivot = Pivot::TOP_LEFT, float zDepth = 0.0f, TextLabel* label = NULL);
		void removeText(TextLabel* label);

		void enableGlyphBox();
		void enableBoundingBox();

		GlyphInfo getGlyphInfo(char c);
		AtlasInfo getAtlasInfo();

		void updateLabel(TextLabel* label);
	};
}