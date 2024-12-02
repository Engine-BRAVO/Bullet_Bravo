#include "DemoManagerBehaviourScript.h"
#include "DemoButtonBehaviourScript.h"
#include "DemoEndOfLevelTriggerBehaviourScript.h"
#include "Input.h"
#include "LevelCreatorBehaviourScript.h"
#include "LevelManagerBehaviourScript.h"
#include "LevelManagerPrefab.h"
#include "MainMenuPrefab.h"
#include "PlayerPrefab.h"
#include <EngineBravo.h>
#include <GameObject.h>
#include <Scene.h>
#include <SceneManager.h>
#include <iostream>

SpriteDef buttonUpSpriteDef = {"Dungeontileset/0x72_DungeonTilesetII_v1.7.png", Rect{16, 208, 16, 16}, 16, 16};
SpriteDef buttonDownSpriteDef = {"Dungeontileset/0x72_DungeonTilesetII_v1.7.png", Rect{32, 208, 16, 16}, 16, 16};

void DemoManagerBehaviourScript::createFirstScene()
{
	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();

	Scene* scene = sceneManager.createScene("DemoScene1");
	if (scene == nullptr)
	{
		exit(1);
	}

	Camera* camera = new Camera;
	camera->setTag("MainCamera");
	camera->setActive(true);

	camera->setTransform(Transform(Vector2(80, 96)));
	camera->setWidth(16 * 30);
	camera->setHeight(9 * 30);

	scene->addGameObject(camera);

	std::string path = FSConverter().getResourcePath("LevelDefs/demoLevel1.json");

	TileMapParser tileMapParser(path);
	tileMapParser.parse();
	const TileMapData& tileMapData = tileMapParser.getTileMapData();

	LevelCreatorBehaviourScript().createLevel(scene, tileMapData);
	GameObject* defaultPlayerPrefab = PlayerPrefabFactory().createPlayerPrefab();

	defaultPlayerPrefab->setTransform(Transform(Vector2(40, 40)));

	scene->addPersistentGameObject(defaultPlayerPrefab);

	GameObject* button = new GameObject;
	button->setTransform(Transform(Vector2(208, 128)));

	Sprite* buttonDownSprite = EngineBravo::getInstance().getResourceManager().createSprite(buttonDownSpriteDef);
	buttonDownSprite->setLayer(1);
	buttonDownSprite->setTag("ButtonDownSprite");
	buttonDownSprite->setActive(false);
	Sprite* buttonUpSprite = EngineBravo::getInstance().getResourceManager().createSprite(buttonUpSpriteDef);
	buttonUpSprite->setLayer(1);
	buttonUpSprite->setTag("ButtonUpSprite");
	buttonDownSprite->setActive(true);

	button->addComponent(buttonDownSprite);
	button->addComponent(buttonUpSprite);

	button->addComponent<DemoButtonBehaviourScript>();
	button->addComponent<RigidBody>();

	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->setWidth(buttonUpSprite->getWidth());
	boxCollider->setHeight(buttonUpSprite->getHeight());
	boxCollider->setTrigger(true);

	button->addComponent(boxCollider);

	scene->addGameObject(button);

	GameObject* endOfLevelTrigger = new GameObject;
	endOfLevelTrigger->setTransform(Transform(Vector2(260, 64)));
	endOfLevelTrigger->setTag("EndOfLevelTrigger");
	endOfLevelTrigger->addComponent<DemoEndOfLevelTriggerBehaviourScript>();

	endOfLevelTrigger->addComponent<RigidBody>();
	BoxCollider* endOfLevelTriggerCollider = new BoxCollider();
	endOfLevelTriggerCollider->setWidth(16);
	endOfLevelTriggerCollider->setHeight(16);
	endOfLevelTriggerCollider->setTrigger(true);
	endOfLevelTrigger->addComponent(endOfLevelTriggerCollider);

	scene->addGameObject(endOfLevelTrigger);

	sceneManager.requestSceneChange("DemoScene1");
}

void DemoManagerBehaviourScript::createSecondScene()
{
	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();

	Scene* scene = sceneManager.createScene("DemoScene2");
	if (scene == nullptr)
	{
		exit(1);
	}

	// set starting pos for player in this scene
	// GameObject* playerObject = sceneManager.getCurrentScene()->getGameObjectsWithTag("Player").at(0);
	// playerObject->setTransform(Transform(Vector2(40, 40)));
	GameObject* playerObject =
		EngineBravo::getInstance().getSceneManager().getCurrentScene()->getGameObjectsWithTag("Player").at(0);
	// std::cout << "Setting player transform" << std::endl;
	playerObject->setTransform(Transform(Vector2(40, 40)));

	Camera* camera = new Camera;
	camera->setTag("MainCamera");
	camera->setActive(true);

	camera->setTransform(Transform(Vector2(80, 96)));
	camera->setWidth(16 * 30);
	camera->setHeight(9 * 30);

	scene->addGameObject(camera);

	std::string path = FSConverter().getResourcePath("LevelDefs/demoLevel2.json");

	TileMapParser tileMapParser(path);
	tileMapParser.parse();
	const TileMapData& tileMapData = tileMapParser.getTileMapData();

	LevelCreatorBehaviourScript().createLevel(scene, tileMapData);
	// GameObject* defaultPlayerPrefab = PlayerPrefabFactory().createPlayerPrefab();
	//
	// defaultPlayerPrefab->setTransform(Transform(Vector2(40, 40)));
	//
	// scene->addPersistentGameObject(defaultPlayerPrefab);

	// GameObject* button = new GameObject;
	// button->setTransform(Transform(Vector2(208, 128)));
	//
	// Sprite* buttonDownSprite = EngineBravo::getInstance().getResourceManager().createSprite(buttonDownSpriteDef);
	// buttonDownSprite->setLayer(1);
	// buttonDownSprite->setTag("ButtonDownSprite");
	// buttonDownSprite->setActive(false);
	// Sprite* buttonUpSprite = EngineBravo::getInstance().getResourceManager().createSprite(buttonUpSpriteDef);
	// buttonUpSprite->setLayer(1);
	// buttonUpSprite->setTag("ButtonUpSprite");
	// buttonDownSprite->setActive(true);
	//
	// button->addComponent(buttonDownSprite);
	// button->addComponent(buttonUpSprite);
	//
	// button->addComponent<DemoButtonBehaviourScript>();
	// button->addComponent<RigidBody>();
	//
	// BoxCollider* boxCollider = new BoxCollider();
	// boxCollider->setWidth(buttonUpSprite->getWidth());
	// boxCollider->setHeight(buttonUpSprite->getHeight());
	// boxCollider->setTrigger(true);
	//
	// button->addComponent(boxCollider);
	//
	// scene->addGameObject(button);

	GameObject* endOfLevelTrigger = new GameObject;
	endOfLevelTrigger->setTransform(Transform(Vector2(125, 65)));
	endOfLevelTrigger->setTag("EndOfLevelTrigger");
	endOfLevelTrigger->addComponent<DemoEndOfLevelTriggerBehaviourScript>();

	endOfLevelTrigger->addComponent<RigidBody>();
	BoxCollider* endOfLevelTriggerCollider = new BoxCollider();
	endOfLevelTriggerCollider->setWidth(16);
	endOfLevelTriggerCollider->setHeight(16);
	endOfLevelTriggerCollider->setTrigger(true);
	endOfLevelTrigger->addComponent(endOfLevelTriggerCollider);

	scene->addGameObject(endOfLevelTrigger);

	sceneManager.requestSceneChange("DemoScene2");
}

void DemoManagerBehaviourScript::nextScene()
{
	mCurrentScene++;
	switch (mCurrentScene)
	{
	case 1:
		createSecondScene();
		break;
	default:
		break;
	}
}

void DemoManagerBehaviourScript::onStart()
{
	std::cout << "DemoInitBehaviourScript::onStart()" << std::endl;

	mCurrentScene = 0;
	createFirstScene();
}

void DemoManagerBehaviourScript::onUpdate()
{
	Input& input = Input::getInstance();

	if (input.GetKeyDown(Key::Key_Space))
	{
		GameObject* playerObject =
			EngineBravo::getInstance().getSceneManager().getCurrentScene()->getGameObjectsWithTag("Player").at(0);
		std::cout << "Setting player transform" << std::endl;
		playerObject->setTransform(Transform(Vector2(40, 40)));
	}
}
