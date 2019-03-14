
// Engine Include
#include "Engine.h"

CTextLabel::CTextLabel(const char* _fontFamily) :
	m_position(glm::vec3()),
	m_scale(1.0f),
	m_color(glm::vec3(1.0f, 1.0f, 1.0f))
{
	m_textObj = CAssetMgr::Get()->GetFont(_fontFamily);
}

CTextLabel::CTextLabel(const char* _fontFamily, std::string _text, glm::vec2 _position) :
	m_text(_text),
	m_position(_position),
	m_scale(1.0f),
	m_color(glm::vec3(1.0f, 1.0f, 1.0f))
{
	m_textObj = CAssetMgr::Get()->GetFont(_fontFamily);
}

CTextLabel::~CTextLabel()
{}

void CTextLabel::RenderTextLabel()
{
	m_textObj->SetText(m_text);
	m_textObj->SetPosition(m_position);
	m_textObj->SetScale(m_scale);
	m_textObj->SetColor(m_color);

	m_textObj->Render();
}

void CTextLabel::SetText(std::string _text)
{
	m_text = _text;
}

void CTextLabel::SetPosition(glm::vec2 _position)
{
	m_position = _position;
}

void CTextLabel::SetColor(glm::vec3 _color)
{
	m_color = _color;
}

void CTextLabel::SetScale(GLfloat _scale)
{
	m_scale = _scale;
}
