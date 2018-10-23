/*
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name    : Textabel.h
// Description	:
// Author       : Richard Wulansari & Jacob Dewse
// Mail         : richard.wul7481@mediadesign.school.nz, jacob.dew7364@mediadesign.school.nz
*/

#ifndef TEXTLABEL_H
#define TEXTLABEL_H

// OpenGL Library Include
#include <glew.h>
#include <freeglut.h>

#include <glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// STD Include
#include <string>

// Foward Declare
class Text;

class CTextLabel
{
public:
	CTextLabel(const char* _fontFamily);
	CTextLabel(const char* _fontFamily, std::string _text, glm::vec2 _position);
	~CTextLabel();

	void RenderTextLabel();

	void SetText(std::string _text);
	void SetPosition(glm::vec2 _position);
	void SetColor(glm::vec3 _color);
	void SetScale(GLfloat _scale);

private:
	Text* m_textObj;

	std::string m_text;
	glm::vec2 m_position;
	GLfloat m_scale;
	glm::vec3 m_color;
};


#endif // !TEXTLABEL_H