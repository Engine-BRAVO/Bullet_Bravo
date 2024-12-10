#include "PlayerPrefab.h"

#include "AudioBehaviourScript.h"
#include <GameObject.h>
#include <NetworkObject.h>

const int spriteWidth = 16;	 // Width of each sprite
const int spriteHeight = 25; // Height of each sprite

const Point playerIdleBackPosition = {21, 95};
SpriteDef firstFramePlayerIdleBack = {
	"enter_the_gungeon_spritesheet.png",
	Rect{playerIdleBackPosition.x, playerIdleBackPosition.y, spriteWidth, spriteHeight}, spriteWidth, spriteHeight};

const Point playerIdleSidePosition = {21, 66};
SpriteDef firstFramePlayerIdleSize = {
	"enter_the_gungeon_spritesheet.png",
	Rect{playerIdleSidePosition.x, playerIdleSidePosition.y, spriteWidth, spriteHeight}, spriteWidth, spriteHeight};

const Point playerIdleFrontPosition = {21, 37};
SpriteDef firstFramePlayerIdleFront = {
	"enter_the_gungeon_spritesheet.png",
	Rect{playerIdleFrontPosition.x, playerIdleFrontPosition.y, spriteWidth, spriteHeight}, spriteWidth, spriteHeight};

const Point playerWalkingFrontPosition = {22, 187};
SpriteDef firstFramePlayerWalkingFront = {
	"enter_the_gungeon_spritesheet.png",
	Rect{playerWalkingFrontPosition.x, playerWalkingFrontPosition.y, spriteWidth, spriteHeight}, spriteWidth,
	spriteHeight};

const Point playerWalkingBackPosition = {20, 251};
SpriteDef firstFramePlayerWalkingBack = {
	"enter_the_gungeon_spritesheet.png",
	Rect{playerWalkingBackPosition.x, playerWalkingBackPosition.y, spriteWidth, spriteHeight}, spriteWidth,
	spriteHeight};

const Point playerWalkingBackSidePosition = {158, 187};
SpriteDef firstFramePlayerWalkingBackSide = {
	"enter_the_gungeon_spritesheet.png", Rect{playerWalkingBackSidePosition.x, playerWalkingBackSidePosition.y, 17, 27},
	17, 27};

const Point playerWalkingFrontSidePosition = {160, 220};
SpriteDef firstFramePlayerWalkingFrontSide = {
	"enter_the_gungeon_spritesheet.png",
	Rect{playerWalkingFrontSidePosition.x, playerWalkingFrontSidePosition.y, 17, 27}, 17, 27};

GameObject* PlayerPrefabFactory::createPlayerPrefab()
{
	GameObject* defaultPlayerPrefab = new GameObject;
	defaultPlayerPrefab->setName("Player");
	setTransform(defaultPlayerPrefab);
	// if (EngineBravo::getInstance().getNetworkManager().isNetworked()) {
	//     defaultPlayerPrefab->addComponent<NetworkObject>();
	//     addNetworkTransform(defaultPlayerPrefab);
	//     addNetworkBehaviourScript(defaultPlayerPrefab);
	// } else {
	addPlayerBehaviourScript(defaultPlayerPrefab);
	// }

	setTag(defaultPlayerPrefab);
	addAnimations(defaultPlayerPrefab);
	addParticleEmitter(defaultPlayerPrefab);
	addRigidBody(defaultPlayerPrefab);
	addCollider(defaultPlayerPrefab);
	addSound(defaultPlayerPrefab);

	return defaultPlayerPrefab;
}

void PlayerPrefabFactory::setTransform(GameObject* gameObject)
{
	Transform objectTransform;
	objectTransform.position.x = 80;
	objectTransform.position.y = 100;
	gameObject->setTransform(objectTransform);
}

void PlayerPrefabFactory::addPlayerBehaviourScript(GameObject* gameObject)
{
	gameObject->addComponent<PlayerBehaviourScript>();
}

void PlayerPrefabFactory::addNetworkBehaviourScript(GameObject* gameObject)
{
	gameObject->addComponent<PlayerNetworkBehaviourScript>();
}

void PlayerPrefabFactory::setTag(GameObject* gameObject) { gameObject->setTag("Player"); }

void PlayerPrefabFactory::addNetworkTransform(GameObject* gameObject)
{
	gameObject->addComponent<NetworkTransform>(true, true, false, false, false);
}

void PlayerPrefabFactory::addAnimations(GameObject* gameObject)
{
	Animation* playerIdleFrontAnimation = nullptr;
	Animation* playerIdleSideAnimation = nullptr;
	Animation* playerIdleBackAnimation = nullptr;

	Animation* playerWalkingFrontAnimation = nullptr;
	Animation* playerWalkingBackAnimation = nullptr;
	Animation* playerWalkingBackSideAnimation = nullptr;
	Animation* playerWalkingFrontSideAnimation = nullptr;

	{
		std::vector<SpriteDef> playerIdleBackAnimationFrames =
			SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerIdleBack, 3);

		std::vector<SpriteDef> playerAnimationIdleBack = {
			playerIdleBackAnimationFrames[0], playerIdleBackAnimationFrames[1], playerIdleBackAnimationFrames[2],
			playerIdleBackAnimationFrames[1]};

		playerIdleBackAnimation =
			EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationIdleBack, 200, true);
	}
	{
		std::vector<SpriteDef> playerIdleSideAnimationFrames =
			SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerIdleSize, 3);

		std::vector<SpriteDef> playerAnimationIdleSide = {
			playerIdleSideAnimationFrames[0], playerIdleSideAnimationFrames[1], playerIdleSideAnimationFrames[2],
			playerIdleSideAnimationFrames[1]};

		playerIdleSideAnimation =
			EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationIdleSide, 200, true);
	}
	{
		std::vector<SpriteDef> playerIdleFrontAnimationFrames =
			SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerIdleFront, 3);

		std::vector<SpriteDef> playerAnimaionIdleFront = {
			playerIdleFrontAnimationFrames[0], playerIdleFrontAnimationFrames[1], playerIdleFrontAnimationFrames[2],
			playerIdleFrontAnimationFrames[1]};

		playerIdleFrontAnimation =
			EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimaionIdleFront, 200, true);
	}

	{
		std::vector<SpriteDef> playerWalkingBackAnimationFrames =
			SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerWalkingBack, 6);

		std::vector<SpriteDef> playerAnimationWalkingBack = playerWalkingBackAnimationFrames;

		playerWalkingBackAnimation =
			EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationWalkingBack, 100, true);
	}
	{
		std::vector<SpriteDef> playerWalkingFrontAnimationFrames =
			SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerWalkingFront, 6);

		std::vector<SpriteDef> playerAnimationWalkingFront = playerWalkingFrontAnimationFrames;

		playerWalkingFrontAnimation =
			EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationWalkingFront, 100, true);
	}
	{
		std::vector<SpriteDef> playerWalkingBackSideAnimationFrames =
			SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerWalkingBackSide, 6);

		std::vector<SpriteDef> playerAnimationWalkingBackSide = playerWalkingBackSideAnimationFrames;

		playerWalkingBackSideAnimation =
			EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationWalkingBackSide, 100, true);
	}
	{
		std::vector<SpriteDef> playerWalkingFrontSideAnimationFrames =
			SpriteDefUtil::extrapolateSpriteDef(firstFramePlayerWalkingFrontSide, 6);

		std::vector<SpriteDef> playerAnimationWalkingFrontSide = playerWalkingFrontSideAnimationFrames;

		playerWalkingFrontSideAnimation =
			EngineBravo::getInstance().getResourceManager().loadAnimation(playerAnimationWalkingFrontSide, 100, true);
	}

	playerIdleBackAnimation->setTag("playerIdleBack");
	playerIdleSideAnimation->setTag("playerIdleSide");
	playerIdleFrontAnimation->setTag("playerIdleFront");

	playerWalkingFrontAnimation->setTag("playerWalkingFront");
	playerWalkingBackAnimation->setTag("playerWalkingBack");
	playerWalkingBackSideAnimation->setTag("playerWalkingBackSide");
	playerWalkingFrontSideAnimation->setTag("playerWalkingFrontSide");

	gameObject->addComponent(playerIdleFrontAnimation);
	gameObject->addComponent(playerIdleSideAnimation);
	gameObject->addComponent(playerIdleBackAnimation);

	gameObject->addComponent(playerWalkingFrontAnimation);
	gameObject->addComponent(playerWalkingBackAnimation);
	gameObject->addComponent(playerWalkingBackSideAnimation);
	gameObject->addComponent(playerWalkingFrontSideAnimation);

	for (auto animation : gameObject->getComponents<Animation>())
	{
		animation->setActive(false);
		animation->setLayer(2);
	}

	playerIdleFrontAnimation->setActive(true);
}

void PlayerPrefabFactory::addParticleEmitter(GameObject* gameObject)
{
	EmitterMode emitterMode = EmitterMode::Continuous;
	float speed = 50.0f;
	float acceleration = 0.0f;
	int minLifeTimeMs = 100;
	int maxLifeTimeMs = 1000;
	Vector2 startSize = Vector2(5, 5);
	Vector2 endSize = Vector2(0, 0);
	float rotation = 0.0f;
	float rotationSpeed = 0.0f;
	float rotationAcceleration = 0.0f;

	std::vector<Color> colors = {Color(255, 49, 3, 170), Color(255, 100, 3, 150), Color(0, 0, 0, 90),
								 Color(0, 0, 0, 90)};

	ParticleEmitter* emitter =
		new ParticleEmitter(emitterMode, speed, acceleration, minLifeTimeMs, maxLifeTimeMs, startSize, endSize,
							rotation, rotationSpeed, rotationAcceleration, colors);

	emitter->setParticlesPerSecond(300);
	emitter->setAngle(0, 45);
	emitter->setLayer(4);
	gameObject->addComponent(emitter);
}

void PlayerPrefabFactory::addRigidBody(GameObject* gameObject)
{
	RigidBody* rigidBody = new RigidBody();

	rigidBody->setCanRotate(true);
	rigidBody->setHasGravity(false);
	rigidBody->setIsMoveableByForce(true);
	rigidBody->setDensity(1.0f);
	rigidBody->setFriction(0.6f);
	rigidBody->setRestitution(0.0f);
	rigidBody->setGravityScale(1.0f);
	rigidBody->setMass(10.0f);
	rigidBody->setLinearDamping(0.5f);
	rigidBody->setAngularDamping(0.5f);

	gameObject->addComponent(rigidBody);
}

void PlayerPrefabFactory::addCollider(GameObject* gameObject)
{
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->setWidth(firstFramePlayerIdleFront.width);
	boxCollider->setHeight(firstFramePlayerIdleFront.height);

	BoxCollider* boxCollider2 = new BoxCollider();
	boxCollider2->setWidth(16);
	boxCollider2->setHeight(25);

	BoxCollider* boxCollider3 = new BoxCollider();
	boxCollider3->setWidth(16);
	boxCollider3->setHeight(25);
	Transform transform2;
	transform2.position.x = 4;
	transform2.position.y = 4;

	boxCollider3->setTransform(transform2);

	CircleCollider* circleCollider = new CircleCollider();
	circleCollider->setRadius(10);

	gameObject->addComponent(boxCollider3);
	gameObject->addComponent(boxCollider2);
	// gameObject->addComponent(boxCollider2);
	//  gameObject->addComponent(circleCollider);
}

void PlayerPrefabFactory::addSound(GameObject* gameObject)
{
	AudioManager& audioManager = EngineBravo::getInstance().getAudioManager();
	// Add sound effects
	AudioSource* sound = new AudioSource("Audio/gun1.wav");
	sound->setPlayOnWake(false);
	sound->setVolume(90);
	sound->setXDirection(0);
	sound->setTag("gun");
	gameObject->addComponent(sound);
	audioManager.loadSound(*sound);

	AudioSource* step = new AudioSource("Audio/Steps_tiles-002.wav");
	step->setPlayOnWake(false);
	step->setVolume(30);
	step->setXDirection(0);
	step->setTag("step");
	gameObject->addComponent(step);
	gameObject->addComponent<AudioBehaviourScript>();
	audioManager.loadSound(*step);
}
