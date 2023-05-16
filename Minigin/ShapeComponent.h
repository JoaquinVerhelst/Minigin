#pragma once


#include "Component.h"
#include <glm/glm.hpp>

#include "SDL.h"
#include "Subject.h"
#include "GameObject.h"
#include "Command.h"


namespace dae
{

	class ShapeComponent final:  public Component
	{
	public:
		ShapeComponent(GameObject* owner);

		void Update() override;

		void Init() override;
		bool IsRenderer() override { return false; }

		void CalculateShapeComponent(int x, int y, int width, int height);

		ShapeComponent(const ShapeComponent& other) = delete;
		ShapeComponent(ShapeComponent&& other) = delete;
		ShapeComponent& operator=(const ShapeComponent& other) = delete;
		ShapeComponent& operator=(ShapeComponent&& other) = delete;


		void SetCenter(glm::ivec2 center);
		glm::ivec2 GetCenter();

		void SetShape(glm::ivec2 shape);
		glm::ivec2 GetShape();


	protected:

		glm::ivec2 m_CenterPoint;
		glm::ivec2 m_Shape;


	private:




	};
}
