#pragma once

// GLobal Include
#include "Utility.h"

// Forward Decalre
class CClothComponent;
class CClothPoint;

class CClothLink
{
public:

	CClothLink(
		CClothPoint* _point1, 
		CClothPoint* _point2, 
		CClothComponent* _owner,
		float _restDistance);
	~CClothLink();

	void ResolveLink();
	void DestroyLink();

	void Update();

	CClothPoint* GetPoint_1() const;
	CClothPoint* GetPoint_2() const;

	bool IsActiveLink() const;

private:

	CClothComponent* m_componentOwner;
	CClothPoint* m_point1;
	CClothPoint* m_point2;
	float m_restDistance;

	bool m_bActive;
};