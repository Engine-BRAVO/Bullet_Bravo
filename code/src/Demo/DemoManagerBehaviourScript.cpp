#include "DemoManagerBehaviourScript.h"
#include "DemoButtonBehaviourScript.h"
#include "DemoButtonPrefab.h"
#include "DemoEndOfLevelTriggerBehaviourScript.h"
#include "DemoLevel2Behaviour.h"
#include "DemoMusicButtonBehaviourScript.h"
#include "DemoSFXButtonBehaviourScript.h"
#include "EnemyPrefab.h"
#include "Input.h"
#include "LevelCreatorBehaviourScript.h"
#include "LevelManagerBehaviourScript.h"
#include "LevelManagerPrefab.h"
#include "MainMenuPrefab.h"
#include "Pathfinding.h"
#include "PlayerPrefab.h"
#include <EngineBravo.h>
#include <GameObject.h>
#include <MapToGraph.h>
#include <Scene.h>
#include <SceneManager.h>
#include <Text.h>
#include <iostream>

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

	camera->setBackgroundColor(Color(0, 0, 0));

	camera->setRenderOrder(0);

	camera->setTransform(Transform(Vector2(80, 96)));
	camera->setWidth(16 * 30);
	camera->setHeight(9 * 30);

	scene->addGameObject(camera);

	Camera* miniMapCamera = new Camera;
	miniMapCamera->setTag("MiniMapCamera");
	miniMapCamera->setActive(true);

	miniMapCamera->setRenderOrder(1);

	miniMapCamera->setViewport(FRect{0.7, 0.7, 0.3, 0.3});

	miniMapCamera->getDebugOverlayRef().renderCameraViewport = true;
	miniMapCamera->getDebugOverlayRef().renderColliders = true;

	miniMapCamera->setBackgroundColor(Color(0, 0, 150, 100));

	miniMapCamera->setTransform(Transform(Vector2(170, 96)));
	miniMapCamera->setWidth(16 * 20);
	miniMapCamera->setHeight(9 * 20);

	scene->addGameObject(miniMapCamera);

	FSConverter fsconverter;
	std::string path = fsconverter.getResourcePath("LevelDefs/demoLevel1.json");
	TileMapParser tileMapParser(path);
	tileMapParser.parse();
	mTileMapData = tileMapParser.getTileMapData();

	LevelCreatorBehaviourScript().createLevel(scene, mTileMapData);
	GameObject* defaultPlayerPrefab = PlayerPrefabFactory().createPlayerPrefab();

	// defaultPlayerPrefab->setTransform(Transform(Vector2(40, 40)));

	defaultPlayerPrefab->setTransform(Transform(Vector2(40, 90)));

	scene->addPersistentGameObject(defaultPlayerPrefab);

	GameObject* button = DemoButtonPrefab().createButtonPrefab();
	button->setTransform(Transform(Vector2(208, 128)));

	button->addComponent<DemoButtonBehaviourScript>();

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

	mPlayerPositionSet = false;
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
	playerObject->setTransform(Transform(Vector2(40, 40)));
	//  playerObject->setTransform(Transform(Vector2(40, 40)));

	Camera* camera = new Camera;
	camera->setTag("MainCamera");
	camera->setActive(true);

	camera->setTransform(Transform(Vector2(80, 96)));
	camera->setWidth(16 * 30);
	camera->setHeight(9 * 30);

	scene->addGameObject(camera);

	FSConverter fsconverter;
	std::string path = fsconverter.getResourcePath("LevelDefs/demoLevel2.json");
	TileMapParser tileMapParser(path);
	tileMapParser.parse();
	mTileMapData = tileMapParser.getTileMapData();

	LevelCreatorBehaviourScript().createLevel(scene, mTileMapData);

	MapToGraph mapToGraph(mTileMapData);
	mapToGraph.convertToGraph();
	auto graph = mapToGraph.getAdjacencyList();

	// Add button for starting and stopping music
	GameObject* buttonMusic = DemoButtonPrefab().createButtonPrefab();
	buttonMusic->setTag("ButtonStartStopMusic");
	buttonMusic->setTransform(Transform(Vector2(540, 174)));
	buttonMusic->addComponent<DemoMusicButtonBehaviourScript>();

	Text* text = new Text("Play/Stop", "Arial", Color(255, 255, 255), Vector2(0, 17), Vector2(0.4, 0.4));
	text->setLayer(5);
	text->setTag("ButtonStartStopMusicText");
	text->setParent(buttonMusic);

	scene->addGameObject(text);
	scene->addGameObject(buttonMusic);

	// Add button for resetting music
	GameObject* buttonResetMusic = DemoButtonPrefab().createButtonPrefab();
	buttonResetMusic->setTransform(Transform(Vector2(620, 174)));
	buttonResetMusic->addComponent<DemoMusicButtonBehaviourScript>();
	buttonResetMusic->setTag("ButtonResetMusic");

	Text* textReset = new Text("Reset", "Arial", Color(255, 255, 255), Vector2(0, 17), Vector2(0.4, 0.4));
	textReset->setLayer(5);
	textReset->setTag("ButtonResetMusicText");
	textReset->setParent(buttonResetMusic);

	scene->addGameObject(textReset);
	scene->addGameObject(buttonResetMusic);

	// Add button for SFX
	GameObject* buttonSFX = DemoButtonPrefab().createButtonPrefab();
	buttonSFX->setTag("ButtonSFX");
	buttonSFX->setTransform(Transform(Vector2(540, 56)));
	buttonSFX->addComponent<DemoSFXButtonBehaviourScript>();

	Text* textSFX = new Text("SFX", "Arial", Color(255, 255, 255), Vector2(0, 17), Vector2(0.4, 0.4));
	textSFX->setLayer(5);
	textSFX->setTag("ButtonSFXText");
	textSFX->setParent(buttonSFX);

	scene->addGameObject(textSFX);
	scene->addGameObject(buttonSFX);

	// Add enemies
	GameObject* enemyMoving = EnemyPrefab().createEnemyPrefab();
	GameObject* enemyStatic = EnemyPrefab().createEnemyPrefab();
	GameObject* enemyWithCollider = EnemyPrefab().createEnemyPrefab();
	GameObject* enemyWithPathfinding = EnemyPrefab().createEnemyPrefab();

	enemyMoving->setTransform(Transform(Vector2(100, 112)));
	enemyMoving->setTag("EnemyMoving");
	if (enemyMoving->hasComponent<RigidBody>())
		enemyMoving->removeComponent(enemyMoving->getComponents<RigidBody>()[0]);
	if (enemyMoving->hasComponent<BoxCollider>())
		enemyMoving->removeComponent(enemyMoving->getComponents<BoxCollider>()[0]);

	enemyStatic->setTransform(Transform(Vector2(40, 84)));
	enemyStatic->setTag("EnemyStatic");
	if (enemyStatic->hasComponent<RigidBody>())
		enemyStatic->removeComponent(enemyStatic->getComponents<RigidBody>()[0]);
	if (enemyStatic->hasComponent<BoxCollider>())
		enemyStatic->removeComponent(enemyStatic->getComponents<BoxCollider>()[0]);

	enemyWithCollider->setTransform(Transform(Vector2(320, 112)));
	enemyWithCollider->setTag("EnemyWithCollider");

	enemyWithPathfinding->setTransform(Transform(Vector2(560, 560)));
	enemyWithPathfinding->setTag("EnemyWithPathfinding");

	if (enemyStatic->hasComponent<RigidBody>())
	{
		enemyStatic->getComponents<RigidBody>().at(0)->setCanRotate(true);
	}
	else
	{
		std::cout << "Enemystatic does not have a RigidBody component" << std::endl;
	}

	if (enemyWithCollider->hasComponent<RigidBody>())
	{
		enemyWithCollider->getComponents<RigidBody>().at(0)->setCanRotate(true);
	}
	else
	{
		std::cout << "Enemystatic does not have a RigidBody component" << std::endl;
	}

	scene->addGameObject(enemyMoving);
	scene->addGameObject(enemyStatic);
	scene->addGameObject(enemyWithCollider);
	scene->addGameObject(enemyWithPathfinding);

	std::unique_ptr<Pathfinding> pathfinding = std::make_unique<Pathfinding>(graph, 50, 50);

	GameObject* level2 = new GameObject;
	level2->addComponent<DemoLevel2Behaviour>(std::move(pathfinding), 50, 50);

	scene->addGameObject(level2);

	sceneManager.requestSceneChange("DemoScene2");

	mPlayerPositionSet = true;

	saveGame();
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
	// createSecondScene();
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

	handleSaveGame();

	if (!mPlayerPositionSet)
	{
		EngineBravo& engine = EngineBravo::getInstance();
		SceneManager& sceneManager = engine.getSceneManager();
		Scene* currentScene = sceneManager.getCurrentScene();

		if (currentScene != nullptr)
		{
			std::vector<GameObject*> persistentObjects = currentScene->getPersistentGameObjects();
			auto playerIt = std::find_if(persistentObjects.begin(), persistentObjects.end(),
										 [](GameObject* obj) { return obj->getTag() == "Player"; });

			if (playerIt != persistentObjects.end())
			{
				LevelCreatorBehaviourScript().setPlayerStartPosition(currentScene, mTileMapData);
				mPlayerPositionSet = true; // Set the flag to true
			}
		}
	}
}

void DemoManagerBehaviourScript::handleSaveGame()
{
	Input& input = Input::getInstance();

	if (input.GetKeyDown(Key::Key_J))
	{

		saveGame();
	}

	if (input.GetKeyDown(Key::Key_K))
	{
		loadGame();
	}
}

void DemoManagerBehaviourScript::saveGame()
{
	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();
	Scene* currentScene = sceneManager.getCurrentScene();

	Transform playerPos;
	if (currentScene != nullptr)
	{
		std::vector<GameObject*> persistentObjects = currentScene->getPersistentGameObjects();
		auto playerIt = std::find_if(persistentObjects.begin(), persistentObjects.end(),
									 [](GameObject* obj) { return obj->getTag() == "Player"; });

		if (playerIt != persistentObjects.end())
		{
			playerPos = (*playerIt)->getTransform().position;
		}
	}

	SaveGame sg{"saves/newSave.json"};

	sg.addFloatField("PlayerX", playerPos.position.x);
	sg.addFloatField("PlayerY", playerPos.position.y);
	sg.addFloatField("PlayerRotation", playerPos.rotation);
	sg.addFloatField("PlayerScaleX", playerPos.scale.x);
	sg.addFloatField("PlayerScaleY", playerPos.scale.y);

	sg.store();
}

void DemoManagerBehaviourScript::loadGame()
{
	SaveGame sg{"saves/newSave.json"};

	float playerX;
	float playerY;
	float playerRotation;
	float playerScaleX;
	float playerScaleY;

	try
	{
		playerX = sg.getFloatField("PlayerX").getValue();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		playerY = sg.getFloatField("PlayerY").getValue();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		playerRotation = sg.getFloatField("PlayerRotation").getValue();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		playerScaleX = sg.getFloatField("PlayerScaleX").getValue();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		playerScaleY = sg.getFloatField("PlayerScaleY").getValue();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();
	Scene* currentScene = sceneManager.getCurrentScene();

	Transform playerPos;
	if (currentScene != nullptr)
	{
		std::vector<GameObject*> persistentObjects = currentScene->getPersistentGameObjects();
		auto playerIt = std::find_if(persistentObjects.begin(), persistentObjects.end(),
									 [](GameObject* obj) { return obj->getTag() == "Player"; });

		if (playerIt != persistentObjects.end())
		{
			Transform playerPos = (*playerIt)->getTransform();
			playerPos.position.x = playerX;
			playerPos.position.y = playerY;
			playerPos.rotation = playerRotation;
			playerPos.scale.x = playerScaleX;
			playerPos.scale.y = playerScaleY;

			(*playerIt)->setTransform(playerPos);
		}
	}
}
