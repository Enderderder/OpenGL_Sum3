
// Engine Include
#include "Engine.h"

Text::Text(std::string font) :
	color(1.0f, 1.0f, 1.0f),
	text(""),
	scale(1.0f)
{
	m_Program = CAssetMgr::GetInstance()->GetProgramID("TextProgram");

	glm::mat4 proj = glm::ortho(0.0f, (GLfloat)util::SCR_WIDTH, 0.0f, (GLfloat)util::SCR_HEIGHT);
	glUseProgram(m_Program);
	glUniformMatrix4fv(glGetUniformLocation(m_Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

	// Initiate the font Lib
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
	// Each character is called a glyph and Face is the collection of glyphs
	FT_Face face;
	// Load font as face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to Load font" << std::endl;
	}
	// Set size to load glyph as
	FT_Set_Pixel_Sizes(face, 0, 48);
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load the first 128 characters of the ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load the character glyph into face
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// Generate texture for character
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now to store character for later use
		Character character = { texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we are finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	//Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Text::~Text()
{

}

void Text::Render()
{
	glm::vec2 textPos = position;

	// Enable blending
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate corresponding render state
	glUseProgram(m_Program);
	glUniform3f(glGetUniformLocation(m_Program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	// Iterate through the Characters
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = textPos.x + ch.Bearing.x * scale;
		GLfloat ypos = textPos.y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },{ xpos + w, ypos + h, 1.0, 0.0 },{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos, ypos + h, 0.0, 0.0 },{ xpos + w, ypos, 1.0, 1.0 },{ xpos, ypos, 0.0, 1.0 },
		};

		// Render the glyph texture over the quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for the next glyph
		textPos.x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}