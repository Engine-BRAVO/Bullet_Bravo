#include "DemoParticlesButtonBehaviourScript.h"
#include "Input.h"
#include <EngineBravo.h>
#include <GameObject.h>
#include <Particle.h>
#include <ParticleEmitter.h>
#include <ParticleSystem.h>
#include <Scene.h>
#include <SceneManager.h>
#include <Sprite.h>
#include <Time.h>
#include <iostream>

DemoParticlesButtonBehaviourScript::DemoParticlesButtonBehaviourScript() {}

void DemoParticlesButtonBehaviourScript::onStart()
{
	mButtonPressed = false;

	std::vector<Color> colorGradient = {
		Color(255, 0, 0, 255),	 // Red
		Color(255, 255, 0, 255), // Yellow
		Color(0, 255, 0, 255),	 // Green
		Color(0, 0, 255, 255)	 // Blue
	};
	EmitterMode emitterMode = EmitterMode::Continuous;
	float speed = 300.0f;
	float acceleration = 0.0f;
	int minLifeTimeMs = 500;
	int maxLifeTimeMs = 1000;
	Vector2 startSize = Vector2(5, 5);
	Vector2 endSize = Vector2(0, 0);
	float rotation = 0.0f;
	float rotationSpeed = 0.0f;
	float rotationAcceleration = 0.0f;

	ParticleEmitter* emitter =
		new ParticleEmitter(emitterMode, speed, acceleration, minLifeTimeMs, maxLifeTimeMs, startSize, endSize,
							rotation, rotationSpeed, rotationAcceleration, colorGradient);

	emitter->setParticlesPerSecond(15000);
	emitter->setAngle(0, 360);
	emitter->setLayer(4);
	Transform transform;
	transform.position.y = 5 * 16;
	// //std::cout << "Emitter position: (" << transform.position.x << ", " << transform.position.y << ")" << std::endl;

	emitter->setRelativeTransform(transform);

	emitter->setActive(false);
	mGameObject->addComponent(emitter);
}

void DemoParticlesButtonBehaviourScript::onUpdate()
{
	Input& input = Input::getInstance();

	if (input.GetKeyDown(Key::Key_Y))
	{
		launchParticles();
	}
}

void DemoParticlesButtonBehaviourScript::onCollide(GameObject* aGameObject)
{
	if (aGameObject == nullptr)
	{
		return;
	}
	if (aGameObject->getTag() == "Player")
	{
		updateButtonState();
	}
}

void DemoParticlesButtonBehaviourScript::updateButtonState()
{
	if (!mButtonPressed)
	{
		for (Component* component : mGameObject->getComponents<Sprite>())
		{
			if (component->getTag() == "ButtonDownSprite")
			{
				component->setActive(true);
			}
			else if (component->getTag() == "ButtonUpSprite")
			{
				component->setActive(false);
			}
		}
		launchParticles();
		mButtonPressed = true;
	}
	else if (mButtonPressed)
	{
		for (Component* component : mGameObject->getComponents<Sprite>())
		{
			if (component->getTag() == "ButtonDownSprite")
			{
				component->setActive(false);
			}
			else if (component->getTag() == "ButtonUpSprite")
			{
				component->setActive(true);
			}
		}
		removeParticles();
		mButtonPressed = false;
	}
}

void DemoParticlesButtonBehaviourScript::launchParticles()
{
	std::cout << "Particles launched" << std::endl;

	if (mGameObject->hasComponent<ParticleEmitter>())
	{
		ParticleEmitter* emitter = mGameObject->getComponents<ParticleEmitter>()[0];
		emitter->setActive(true);
		emitter->burst(300);
	}
}

void DemoParticlesButtonBehaviourScript::removeParticles()
{
	if (mGameObject->hasComponent<ParticleEmitter>())
	{
		ParticleEmitter* emitter = mGameObject->getComponents<ParticleEmitter>()[0];
		emitter->setActive(false);
	}
}