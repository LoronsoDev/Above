#pragma once
#include <random>

#include "Above/Core/Input.h"
#include "Above/Core/Keycodes.h"
#include "Above/Scene/Components.h"
#include "Above/Scene/ScriptableEntity.h"

namespace Above
{
	class CarController : public ScriptableEntity
	{
	public:
		void OnCreate()
		{
		}

		void OnDestroy()
		{
		}
		void OnUpdate(Above::Timestep ts)
		{
			auto& translation = GetComponent<TransformComponent>().Translation;
			float speed = 5.f;

			translation.x += speed * ts;

			if (Input::IsKeyPressed(AB_KEY_D))
			{
				translation.y -= speed * ts;
			}
			if (Input::IsKeyPressed(AB_KEY_A))
			{
				translation.y += speed * ts;
			}

			translation.y = glm::clamp(translation.y, -2.f, 2.f);
		}
	};

	class CameraController : public ScriptableEntity
	{
	public:
		float speed = 5.f;

		void OnCreate()
		{
		}

		void OnDestroy()
		{
		}
		void OnUpdate(Timestep ts)
		{
			auto& translation = GetComponent<TransformComponent>().Translation;

			translation.x += speed * ts;
		}
	};

	class EnemyCarController : public ScriptableEntity
	{
	public:
		void OnCreate()
		{
			std::random_device rd; // obtain a random number from hardware
			std::mt19937 gen(rd()); // seed the generator
			std::uniform_int_distribution<> distr(-2, 2); // define the range

			speed = distr(gen) * 0.09f;
		}

		void OnDestroy()
		{
		}
		void OnUpdate(Timestep ts)
		{
			auto& translation = GetComponent<TransformComponent>().Translation;

			translation.y += speed * ts;
			translation.x -= forwardSpeed * ts;
			;
			translation.y = glm::clamp(translation.y, -1.5f, 1.5f);
		}

	private:
		float speed = 0.f;
		float forwardSpeed = 3.f;
	};
}
