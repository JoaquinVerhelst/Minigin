#pragma once



#include "Component.h"
#include <glm/glm.hpp>
#include "Subject.h"


namespace dae
{

	class GameObject;

	class ScoreComponent final : public Component, public Subject
	{
	public:
		bool IsRenderer() override { return false; }

		void Update() override;

		ScoreComponent(GameObject* owner);
		~ScoreComponent() = default;

		ScoreComponent(const ScoreComponent& other) = delete;
		ScoreComponent(ScoreComponent&& other) = delete;
		ScoreComponent& operator=(const ScoreComponent& other) = delete;
		ScoreComponent& operator=(ScoreComponent&& other) = delete;

		int GetScore();
		void AddScore(int score);

		void AddEmeraldScore(int score);
		//void AddGoldScore(int score);
		//void AddNobbinScore(int score);

	private:

		int m_CurrentScore;

		int m_EmeraldsInARow;
	};
}

