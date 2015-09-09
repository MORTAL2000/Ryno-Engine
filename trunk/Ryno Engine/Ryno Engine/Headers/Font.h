/* 
    Original code from Benjamin Arnold from seed of andromeda.
	I just modified a few things.
	What the class does is the following:
	Loads info about chars and their sizes from the ttf.
	Then it creates a texture big enough to contain them, and place them in it by optimizing the space.
	When required to draw, the textured is sampled depending on the uvs (calculated from the position of the glyphs).
	Each sampled value is written in a quad 
*/

#pragma once


#include <GLM/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include <list>
#include "Structures.h"


namespace Ryno {

	class Text;
	class Batch2DFont;

    struct CharGlyph {
    public:
        C character;
        glm::vec4 uvRect;
        glm::vec2 size;
    };

#define FIRST_PRINTABLE_CHAR ((C)32)
#define LAST_PRINTABLE_CHAR ((C)126)

	class FontGlyph{


	public:
		FontGlyph();
		~FontGlyph();
		void generate_model_matrix();
		U32 get_texture_id() const { return texture.id; };

		glm::mat3 model_matrix;
		glm::vec2 position;
		glm::vec2 scale;
		glm::vec4 uv;
		ColorRGBA color;
		Texture texture;

		static std::list<FontGlyph*> font_glyphs;


	};
	

    class Font {

		friend class FontGlyph;

    public:
		Font(std::string font, U32 size, C cs, C ce);
		Font(std::string font, U32 size) :
			Font(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR) {
        }

		Texture t;



        /// Destroys the font resources
        void dispose();

        int getFontHeight() const {
            return _fontHeight;
        }
        /// Measures the dimensions of the text
        glm::vec2 measure(const char* s);

		void draw(Batch2DFont* batch, Text* message);

		void anchoring(Text* message, glm::vec2 &tp);

		void anchor_y(Text* message, glm::vec2 &tp);

		void anchor_x(Text* message, glm::vec2 &tp);



	private:
        static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

        int _regStart, _regLength;
        CharGlyph* _glyphs;
        int _fontHeight;

        unsigned int _texID;
    };

}
