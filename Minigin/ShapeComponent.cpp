#include "ShapeComponent.h"

dae::ShapeComponent::ShapeComponent(GameObject* owner)
	:Component(owner)
	, m_CenterPoint{}
	, m_Shape{}
{
}

void dae::ShapeComponent::Update()
{}

void dae::ShapeComponent::Init()
{}

void dae::ShapeComponent::CalculateShapeComponent(int x, int y, int width, int height)
{

	SetCenter(glm::vec2(x + width / 2, y + height / 2));
	SetShape(glm::vec2( width, height ));


}

void dae::ShapeComponent::SetCenter(glm::ivec2 center)
{
	m_CenterPoint = center;
}

glm::ivec2 dae::ShapeComponent::GetCenter()
{
	return m_CenterPoint;
}

void dae::ShapeComponent::SetShape(glm::ivec2 shape)
{
	m_Shape = shape;
}

glm::ivec2  dae::ShapeComponent::GetShape()
{
	return m_Shape;
}
