
// Engine Include
#include "Engine.h"

// Special Model Include
#include "ssAnimatedModel.h"

CAnimationComponent::CAnimationComponent()
{
	m_animModel = nullptr;
}

void CAnimationComponent::SetModel(std::string _path, GLuint _program, CCamera* _camera)
{
	m_animModel = new ssAnimatedModel(_path, "", _camera, _program);
}

ssAnimatedModel* CAnimationComponent::GetModel() const
{
	return m_animModel;
}

CAnimationComponent::CAnimationComponent::~CAnimationComponent()
{
	delete m_animModel;
	m_animModel = nullptr;
}

void CAnimationComponent::CAnimationComponent::BeginPlay()
{
	__super::BeginPlay();

	if (m_animModel == nullptr)
	{
		CDebug::Log("There is no model");
		return;
	}

	// Set root animation
	m_animModel->setCurrentAnimation(0, 30);
}

void CAnimationComponent::CAnimationComponent::Update()
{
	__super::Update();
	

}

void CAnimationComponent::CAnimationComponent::Render(CCamera* _camera)
{
	float deltaTime = CTime::GetInstance()->GetDeltaTime();
	m_animModel->render(deltaTime);
}

