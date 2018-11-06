
// This Include
#include "ClothLink.h"

// Component Include
#include "ClothPoint.h"
#include "ClothComponent.h"

CClothLink::CClothLink(CClothPoint* _point1, CClothPoint* _point2,
	CClothComponent* _owner, float _restDistance, CClothPoint* _pointMiddle)
{
	m_componentOwner = _owner;
	m_point1 = _point1;
	m_point2 = _point2;
	m_pointMiddle = _pointMiddle;
	m_restDistance = _restDistance;
	m_bActive = true;
}

CClothLink::~CClothLink()
{}

void CClothLink::ResolveLink()
{
	glm::vec3 delta = m_point1->m_localPosition - m_point2->m_localPosition;


	float deltaLength = glm::length(delta);

	float difference = (deltaLength - m_restDistance) / deltaLength;

	glm::vec3 correctionVec = delta * (1 - m_restDistance / deltaLength);
	glm::vec3 correctionVecHalf = correctionVec * 0.5f;

	float im1 = 1 / m_point1->m_mass;
	float im2 = 1 / m_point2->m_mass;

	glm::vec3 moveAmount_point1 = delta * (im1 / (im1 + im2)) * m_componentOwner->m_stiffness * difference;
	glm::vec3 moveAmount_point2 = delta * (im2 / (im1 + im2)) * m_componentOwner->m_stiffness * difference;

	//if (m_point1->m_bMoveable) m_point1->m_localPosition -= moveAmount_point1;
	//if (m_point2->m_bMoveable) m_point2->m_localPosition += moveAmount_point2;

	if (m_point1->m_bMoveable)
	{
		m_point1->m_localPosition -= moveAmount_point1;

		if (m_point2->m_bMoveable)
		{
			m_point2->m_localPosition += moveAmount_point2;
		}
		else
		{
			m_point1->m_localPosition -= moveAmount_point2;
		}
	}
	else if (m_point2->m_bMoveable)
	{
		m_point2->m_localPosition += moveAmount_point2 + moveAmount_point1;
	}
}

void CClothLink::DestroyLink()
{

}

void CClothLink::Update()
{
	// Check if the points are active

	if (m_pointMiddle != nullptr)
	{
		if (m_pointMiddle->m_bActive == false)
		{
			m_bActive = false;
			return;
		}
	}

	if (m_point1->m_bActive == false || m_point2->m_bActive == false)
	{	
		m_bActive = false;
		return;
	}
}

CClothPoint* CClothLink::GetPoint_1() const
{
	return m_point1;
}

CClothPoint* CClothLink::GetPoint_2() const
{
	return m_point2;
}

bool CClothLink::IsActiveLink() const
{
	return m_bActive;
}
