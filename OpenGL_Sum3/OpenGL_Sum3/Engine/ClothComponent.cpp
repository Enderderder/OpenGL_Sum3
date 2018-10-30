
// Engine Include
#include "Engine.h"

CClothComponent::CClothComponent()
{
	m_restDistance = 0.1f;
}

CClothComponent::~CClothComponent()
{

}

void CClothComponent::BeginPlay()
{
	__super::BeginPlay();

	// Calculate the centre point of the cloth
	float halfWidth = (m_width - 1.0f) * m_restDistance * 0.5f;
	float halfHeight = (m_height - 1.0f) * m_restDistance * 0.5f;

	// Map out all the point and create the point for the cloth in local space
	float x, y, z;
	z = -10.0f;
	for (int row = 0; row < m_height; ++row)
	{
		y = halfHeight - row * m_restDistance;
		for (int col = 0; col < m_width; ++col)
		{
			x = halfWidth - col * m_restDistance;

			CClothPoint* clothPoint = new CClothPoint({ x,y,z });
			m_clothPoints.push_back(clothPoint);
		}
	}
}

void CClothComponent::Update()
{
	__super::Update();
	
}

void CClothComponent::RenderColth(CCamera* _camera)
{
	/** Get the translate scale rotation from the game object transform */
	glm::vec3 objPos = m_owner->m_transform.position;
	glm::vec3 objRotate = m_owner->m_transform.rotation;
	glm::vec3 objScale = m_owner->m_transform.scale;

	/** Calculate the MVP matrix from the game object transform */
	glm::mat4 translate = glm::translate(glm::mat4(), objPos);
	glm::mat4 scale = glm::scale(glm::mat4(), objScale);
	glm::mat4 rotation = glm::mat4();
	rotation = glm::rotate(rotation, glm::radians(objRotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(objRotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(objRotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 model = translate * rotation * scale;
	glm::mat4 mvp = _camera->GetProj() *  _camera->GetView() * model;

	glm::vec4 viewPort(0.0f, 0.0f, 1.0f, 1.0f);

	// Set the color and line width
	glLineWidth(0.5f);
	glColor3f(1.0f, 0.0f, 0.0f);

	glm::vec3 cameraPosition = _camera->m_cameraPosition;
	for (int row = 0; row < m_height; ++row)
	{
		for (int col = 0; col < m_width; ++col)
		{
			// Get the point 1 in world space
			glm::vec3 point_self_world //= m_clothPoints[row + col * m_width]->m_localPosition * m_restDistance + m_owner->m_transform.position;
				//= LocalToWorld(m_clothPoints[row + col * m_width]->m_localPosition * m_restDistance, mvp);
				= glm::project(m_clothPoints[row + col * m_width]->m_localPosition * m_restDistance, model, _camera->GetProj(), viewPort);
			// Loop for the 4 other points related to point 1
			glm::vec3 point_other_world;
			if (IndexInBound(col - 1, row)) 
			{
				point_other_world //= m_clothPoints[row + (col - 1) * m_width]->m_localPosition * m_restDistance + m_owner->m_transform.position;;
					//= LocalToWorld(m_clothPoints[row + (col - 1) * m_width]->m_localPosition * m_restDistance, mvp);
					= glm::project(m_clothPoints[row + (col - 1) * m_width]->m_localPosition * m_restDistance, model, _camera->GetProj(), viewPort);
				DrawLine(point_self_world, point_other_world);
			}
			if (IndexInBound(col + 1, row))
			{
				point_other_world //= m_clothPoints[row + (col + 1) * m_width]->m_localPosition * m_restDistance + m_owner->m_transform.position;;
					//= LocalToWorld(m_clothPoints[row + (col + 1) * m_width]->m_localPosition * m_restDistance, mvp);
					= glm::project(m_clothPoints[row + (col + 1) * m_width]->m_localPosition * m_restDistance, model, _camera->GetProj(), viewPort);
				DrawLine(point_self_world, point_other_world);
			}
			if (IndexInBound(col, row - 1))
			{
				point_other_world //= m_clothPoints[(row - 1) + col * m_width]->m_localPosition * m_restDistance + m_owner->m_transform.position;;
					//= LocalToWorld(m_clothPoints[(row - 1) + col * m_width]->m_localPosition * m_restDistance, mvp);
					= glm::project(m_clothPoints[(row - 1) + col * m_width]->m_localPosition * m_restDistance, model, _camera->GetProj(), viewPort);
				DrawLine(point_self_world, point_other_world);
			}
			if (IndexInBound(col, row + 1))
			{
				point_other_world //= m_clothPoints[(row + 1) + col * m_width]->m_localPosition * m_restDistance + m_owner->m_transform.position;;
					//= LocalToWorld(m_clothPoints[(row + 1) + col * m_width]->m_localPosition * m_restDistance, mvp);
					= glm::project(m_clothPoints[(row + 1) + col * m_width]->m_localPosition * m_restDistance, model, _camera->GetProj(), viewPort);
				DrawLine(point_self_world, point_other_world);
			}
		}
	}

}

bool CClothComponent::IndexInBound(int _row, int _col)
{
	return
		_row >= 0 && _row <= m_height - 1
		&& _col >= 0 && _col <= m_width - 1;
}

glm::vec3 CClothComponent::LocalToWorld(glm::vec3 _localPos, glm::mat4 _mvp)
{
	//glm::vec4 viewPort(0.0f, 0.0f, (float)util::SCR_WIDTH, (float)util::SCR_HEIGHT);
	glm::vec4 halfResult = glm::vec4(_localPos, 1.0f) * _mvp;

	glm::vec3 result = glm::vec3(halfResult) / halfResult.w;
	//glm::vec3 result = glm::project(_localPos, )

	//result = ((result.xy + 1.0))

	return result;	
}

void CClothComponent::DrawLine(glm::vec3 _pos1, glm::vec3 _pos2)
{
	glm::vec3 point1 = glm::vec3(50.0f, 50.0f, 50.0f);
	glm::vec3 point2 = glm::vec3(0.0f, 0.0f, 0.0f);


	glBegin(GL_LINES);
	glVertex3fv(&point1[0]);
	glVertex3fv(&point2[0]);

	//glVertex3fv(&_pos1[0]);
	//glVertex3fv(&_pos2[0]);
	glEnd();
}