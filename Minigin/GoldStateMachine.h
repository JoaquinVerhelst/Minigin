#pragma once
#include "GameObject.h"


namespace dae
{

	class GoldComponent;

	enum class GoldStateType
	{
		Idle,
		Falling,
		Pushed,
		Broken
	};

	class GoldState
	{
	public:


		GoldState() = default;
		virtual ~GoldState() = default;
		virtual bool HandleCollision(GameObject* player, GameObject* gold) = 0;
		virtual void Update(GameObject*, GoldComponent*) {};

		virtual GoldStateType GetType() { return m_Type; }

	protected:
		GoldStateType m_Type{ GoldStateType::Idle };

	};

	class IdleGoldState final: public GoldState
	{
	public:
		IdleGoldState()
		{
			m_Type = GoldStateType::Idle;
		}

		~IdleGoldState() = default;
		bool HandleCollision(GameObject* player, GameObject* gold) override;
		void Update(GameObject* goldActor, GoldComponent* goldComp) override;
	};

	class FallingGoldState final : public GoldState
	{
	public:
		FallingGoldState()
		{
			m_Type = GoldStateType::Falling;
		}

		~FallingGoldState() = default;
		bool HandleCollision(GameObject* player, GameObject* gold) override;
		void Update(GameObject* goldActor, GoldComponent* goldComp) override;
	};

	class PushedRightGoldState final : public GoldState
	{
	public:
		PushedRightGoldState()
		{
			m_Type = GoldStateType::Pushed;
		}

		~PushedRightGoldState() = default;
		bool HandleCollision(GameObject* player, GameObject* gold) override;
		void Update(GameObject* goldActor, GoldComponent* goldComp) override;
	};

	class PushedLeftGoldState final : public GoldState
	{
	public:
		PushedLeftGoldState()
		{
			m_Type = GoldStateType::Pushed;
		}

		~PushedLeftGoldState() = default;
		bool HandleCollision(GameObject* player, GameObject* gold) override;
		void Update(GameObject* goldActor, GoldComponent* goldComp) override;
	};

	class BrokenGoldState final : public GoldState
	{
	public:
		BrokenGoldState()
		{
			m_Type = GoldStateType::Broken;
		}

		~BrokenGoldState() = default;
		bool HandleCollision(GameObject* player, GameObject* gold) override;
		void Update(GameObject* goldActor, GoldComponent* goldComp) override;
	};
}

