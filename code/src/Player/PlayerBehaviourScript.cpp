#include "PlayerBehaviourScript.h"

#include <iostream>

#include "BulletBehaviourScript.h"
#include "BulletPrefab.h"
#include <Animation.h>
#include <Configuration.h>
#include <EngineBravo.h>
#include <GameObject.h>
#include <Input.h>
#include <InputStructs.h>
#include <Network/NetworkObject.h>
#include <ParticleEmitter.h>
#include <Sprite.h>
#include <SpriteDef.h>
#include <SpriteDefUtil.h>

std::string PlayerBehaviourScript::currentActiveAnimationTag()
{
	for (auto animation : mGameObject->getComponents<Animation>())
	{
		if (animation->isActive())
		{
			return animation->getTag();
		}
	}
	return "";
}

void PlayerBehaviourScript::setFlipX(bool aState)
{
	if (mGameObject->hasComponent<Animation>())
	{
		for (auto animation : mGameObject->getComponents<Animation>())
		{
			animation->setFlipX(aState);
		}
	}
	else if (mGameObject->hasComponent<Sprite>())
	{
		for (auto sprite : mGameObject->getComponents<Sprite>())
		{
			sprite->setFlipX(aState);
		}
	}
}

void PlayerBehaviourScript::setFlipY(bool aState)
{
	if (mGameObject->hasComponent<Animation>())
	{
		for (auto animation : mGameObject->getComponents<Animation>())
		{
			animation->setFlipY(aState);
		}
	}
	else if (mGameObject->hasComponent<Sprite>())
	{
		for (auto sprite : mGameObject->getComponents<Sprite>())
		{
			sprite->setFlipY(aState);
		}
	}
}

void PlayerBehaviourScript::deactivateAllAnimations()
{
	for (auto animation : mGameObject->getComponents<Animation>())
	{
		animation->setActive(false);
	}
}

void PlayerBehaviourScript::setAnimationActive(std::string aAnimationTag, bool aState)
{
	for (auto animation : mGameObject->getComponents<Animation>())
	{
		if (animation->getTag() == aAnimationTag)
		{
			animation->setActive(aState);
		}
	}
}

void PlayerBehaviourScript::onStart() {}

void PlayerBehaviourScript::handleAnimations()
{
	if (mGameObject->hasComponent<NetworkObject>())
	{
		NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
		if (!networkObject)
		{
			return;
		}
		if (!networkObject->isOwner())
		{
			return;
		}
	}
	static Transform previousTransform = this->mGameObject->getTransform();
	Transform currentTransform = this->mGameObject->getTransform();

	if (abs(previousTransform.position.x - currentTransform.position.x) < 0.1 &&
		abs(previousTransform.position.y - currentTransform.position.y) < 0.1)
	{
		if ((currentActiveAnimationTag() == "playerIdleFront") || (currentActiveAnimationTag() == "playerIdleBack") ||
			(currentActiveAnimationTag() == "playerIdleSide"))
		{
		}
		else
		{
			if (currentActiveAnimationTag() == "playerWalkingFront")
			{
				deactivateAllAnimations();
				setAnimationActive("playerIdleFront", true);
			}
			else if (currentActiveAnimationTag() == "playerWalkingBack")
			{
				deactivateAllAnimations();
				setAnimationActive("playerIdleBack", true);
			}
			else if (currentActiveAnimationTag() == "playerWalkingFrontSide")
			{
				deactivateAllAnimations();
				setAnimationActive("playerIdleSide", true);
			}
			else if (currentActiveAnimationTag() == "playerWalkingBackSide")
			{
				deactivateAllAnimations();
				setAnimationActive("playerIdleBack", true);
			}
			else
			{
				deactivateAllAnimations();
				setAnimationActive("playerIdleBack", true);
			}
		}

		if (previousTransform.position.x > currentTransform.position.x)
		{
			setFlipX(true);
		}
		else if (currentTransform.position.x > previousTransform.position.x)
		{
			setFlipX(false);
		}
	}
	else
	{
		if (currentTransform.position.y < previousTransform.position.y)
		{
			if (abs(currentTransform.position.x - previousTransform.position.x) > 0.1)
			{
				deactivateAllAnimations();
				setAnimationActive("playerWalkingBackSide", true);
			}
			else if (abs(currentTransform.position.x - previousTransform.position.x) < 0.1)
			{
				deactivateAllAnimations();
				setAnimationActive("playerWalkingBack", true);
			}
		}
		else
		{
			if (abs(currentTransform.position.x - previousTransform.position.x) > 0.1)
			{
				deactivateAllAnimations();
				setAnimationActive("playerWalkingFrontSide", true);
			}
			else if (abs(currentTransform.position.x - previousTransform.position.x) < 0.1)
			{
				deactivateAllAnimations();
				setAnimationActive("playerWalkingFront", true);
			}
		}

		if ((currentTransform.position.x - previousTransform.position.x) < -0.1)
		{
			setFlipX(false);
		}
		else if ((currentTransform.position.x - previousTransform.position.x) > 0.1)
		{
			setFlipX(true);
		}
	}

	previousTransform = this->mGameObject->getTransform();
}

void PlayerBehaviourScript::handleMovement()
{
	static const float movementSpeed = 10000.0f;

	if (mGameObject->hasComponent<NetworkObject>())
	{
		NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
		if (!networkObject)
		{
			return;
		}
		if (!networkObject->isOwner())
		{
			return;
		}
	}

	Input& input = Input::getInstance();

	Transform parentTransform = this->mGameObject->getTransform();

	if (input.GetKey(Key::Key_E))
	{
		deactivateAllAnimations();
	}

	if (input.GetKey(Key::Key_R))
	{
		mGameObject->getComponents<RigidBody>()[0]->addTorque(500);
		// parentTransform.rotate(1.0f);
	}
	if (input.GetKey(Key::Key_T))
	{
		parentTransform.rotate(-1.0f);
	}

	if (input.GetKey(Key::Key_W))
	{
		mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(0, -movementSpeed * Time::deltaTime));
	}
	if (input.GetKey(Key::Key_A))
	{
		mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(-movementSpeed * Time::deltaTime, 0));
	}
	if (input.GetKey(Key::Key_S))
	{
		mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(0, movementSpeed * Time::deltaTime));
	}
	if (input.GetKey(Key::Key_D))
	{
		mGameObject->getComponents<RigidBody>()[0]->addForce(Vector2(movementSpeed * Time::deltaTime, 0));
	}
	this->mGameObject->setTransform(parentTransform);
}

void PlayerBehaviourScript::hanldeCameraMovement()
{
	if (mGameObject->hasComponent<NetworkObject>())
	{
		NetworkObject* networkObject = mGameObject->getComponents<NetworkObject>()[0];
		if (!networkObject)
		{
			return;
		}
		if (!networkObject->isOwner())
		{
			return;
		}
	}

	Camera* currentCam = EngineBravo::getInstance().getSceneManager().getCurrentScene()->getCameraWithTag("MainCamera");

	Transform playerTransform = this->mGameObject->getTransform();

	currentCam->setTransform(playerTransform);
}

void PlayerBehaviourScript::handleAspectRatioTest()
{
	Input& input = Input::getInstance();

	if (input.GetKeyDown(Key::Key_I))
	{
		EngineBravo& engine = EngineBravo::getInstance();
		engine.getRenderSystem().setAspectRatio(Point(16, 9));
	}
	if (input.GetKeyDown(Key::Key_O))
	{
		EngineBravo& engine = EngineBravo::getInstance();
		engine.getRenderSystem().setAspectRatio(Point(1, 1));
	}
}

void PlayerBehaviourScript::fireBullet(Point mousePosition)
{
	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();

	// Get the camera
	Camera* currentCam = sceneManager.getCurrentScene()->getCameraWithTag("MainCamera");
	Vector2 cameraOrigin = currentCam->getOrigin();

	// Get window dimensions
	int windowWidth = EngineBravo::getInstance().getRenderSystem().getWindow().getSize().x;
	int windowHeight = EngineBravo::getInstance().getRenderSystem().getWindow().getSize().y;

	// Convert mouse position to Vector2 and calculate relative to screen center
	Vector2 mousePositionVector(mousePosition.x, mousePosition.y);

	// Transform to world space, accounting for camera position and window dimensions
	Vector2 worldMousePosition =
		cameraOrigin + Vector2(mousePositionVector.x * (currentCam->getWidth() / static_cast<float>(windowWidth)),
							   mousePositionVector.y * (currentCam->getHeight() / static_cast<float>(windowHeight)));

	// Get player position and calculate direction
	Vector2 playerPosition = this->mGameObject->getTransform().position;
	Vector2 direction = worldMousePosition - playerPosition;

// Debug output
#ifdef DEBUG_SHOOTING
	std::cout << "Mouse Position: " << mousePositionVector.x << ", " << mousePositionVector.y << std::endl;
	std::cout << "Camera Origin: " << cameraOrigin.x << ", " << cameraOrigin.y << std::endl;
	std::cout << "World Mouse: " << worldMousePosition.x << ", " << worldMousePosition.y << std::endl;
	std::cout << "Player Position: " << playerPosition.x << ", " << playerPosition.y << std::endl;
	std::cout << "Pre-normalized Direction: " << direction.x << ", " << direction.y << std::endl;
#endif

	// Normalize the direction vector
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 0.0001f)
	{
		direction.x /= length;
		direction.y /= length;
	}

	// Create bullet slightly offset from player position in the firing direction
	Vector2 spawnOffset = direction * 20.0f;
	Vector2 bulletSpawnPosition = playerPosition + spawnOffset;

	// Create and setup the bullet
	GameObject* bulletObject = BulletPrefabFactory().createBulletPrefab(*this->mGameObject);

	// Set bullet's initial position
	bulletObject->getTransform().position = bulletSpawnPosition;

	// Add force to bullet
	RigidBody* bulletRigidBody = bulletObject->getComponents<RigidBody>()[0];
	float bulletSpeed = 1000.0f;
	bulletRigidBody->addForce(direction * bulletSpeed);

	sceneManager.getCurrentScene()->addGameObject(bulletObject);
}

void PlayerBehaviourScript::onUpdate()
{
	Input& input = Input::getInstance();

	handleMovement();
	handleAnimations();
	handleAspectRatioTest();

	hanldeCameraMovement();

	if (input.GetKeyDown(Key::Key_C))
	{
		Camera* mainCam =
			EngineBravo::getInstance().getSceneManager().getCurrentScene()->getCameraWithTag("MainCamera");

		mainCam->getDebugOverlayRef().renderColliders = !mainCam->getDebugOverlayRef().renderColliders;
	}

	if (input.GetKeyDown(Key::Key_F))
	{
		Camera* mainCam =
			EngineBravo::getInstance().getSceneManager().getCurrentScene()->getCameraWithTag("MainCamera");

		mainCam->getDebugOverlayRef().showFPS = !mainCam->getDebugOverlayRef().showFPS;
	}

	if (input.GetMouseButtonDown(MouseButton::LEFT))
	{
		fireBullet(input.MousePosition());
	}

	if (mGameObject->hasComponent<ParticleEmitter>())
	{
		static bool emitterMode = false;

		ParticleEmitter* emitter = mGameObject->getComponents<ParticleEmitter>()[0];

		if (input.GetKeyDown(Key::Key_P))
		{
			emitter->setActive(!emitter->isActive());
		}

		if (input.GetKeyDown(Key::Key_Space))
		{
			emitterMode = !emitterMode;
		}

		if (emitterMode)
		{
			emitter->setAngle(0, 45);
			emitter->getRelativeTransform().rotation += 1.0f * Time::deltaTime;
		}
		else
		{
			emitter->setAngle(0, 360);
			emitter->getRelativeTransform().rotation = 0.0f;
		}
	}
}

void PlayerBehaviourScript::onCollide(GameObject* aGameObject)
{
	if (aGameObject != nullptr)
	{
		// std::cout << "Player collided with " << aGameObject->getName() <<
		// std::endl;
	}
}

float PlayerBehaviourScript::getHealth() { return mHealth; }

float PlayerBehaviourScript::getMaxHealth() { return mMaxHealth; }

int PlayerBehaviourScript::getECCount() { return mECCount; }

int PlayerBehaviourScript::getBSCount() { return mBSCount; }

void PlayerBehaviourScript::setHealth(float aHealth) { mHealth = aHealth; }

void PlayerBehaviourScript::setMaxHealth(float aMaxHealth) { mMaxHealth = aMaxHealth; }

void PlayerBehaviourScript::setECCount(int aECCount) { mECCount = aECCount; }

void PlayerBehaviourScript::setBSCount(int aBSCount) { mBSCount = aBSCount; }
