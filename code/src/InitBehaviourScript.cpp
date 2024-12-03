#include "InitBehaviourScript.h"
#include "LevelManagerBehaviourScript.h"
#include "LevelManagerPrefab.h"
#include "MainMenuPrefab.h"
#include <EngineBravo.h>
#include <GameObject.h>
#include <Scene.h>
#include <SceneManager.h>
#include <iostream>

void InitBehaviourScript::onStart() { createLevelManager(); }

void InitBehaviourScript::onUpdate() {}

void InitBehaviourScript::createLevelManager()
{
	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();
	Scene* scene = sceneManager.getCurrentScene();

	GameObject* LevelManagerObject = LevelManagerFactory().createLevelManagerPrefab();

	scene->addPersistentGameObject(LevelManagerObject);

	createMainMenu();
	// LevelManagerBehaviourScript* levelManager =
	// LevelManagerObject->getComponents<LevelManagerBehaviourScript>()[0];
	// levelManager->beginGame();
}

void InitBehaviourScript::createMainMenu()
{
	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();
	Scene* scene = sceneManager.createScene("MainMenuScene");

	int menuStartX = 240 + 13;
	int menuStartY = 135 + 5;

	GameObject* MainMenuObject = MainMenuPrefabFactory().createMainMenuPrefab();

	MainMenuPrefabFactory().createDefaultButton(MainMenuObject, scene, "Play", "PlayButton", "Play", menuStartX,
												menuStartY);
	menuStartY += 20;

	MainMenuPrefabFactory().createDefaultButton(MainMenuObject, scene, "Multiplayer", "MultiplayerButton",
												"Multiplayer", menuStartX, menuStartY);
	menuStartY += 20;

	MainMenuPrefabFactory().createDefaultButton(MainMenuObject, scene, "Demo", "DemoButton", "Demo", menuStartX,
												menuStartY);
	menuStartY += 20;

	MainMenuPrefabFactory().createDefaultButton(MainMenuObject, scene, "Exit", "ExitButton", "Exit", menuStartX,
												menuStartY);

	scene->addGameObject(MainMenuObject);
	// int cameraID = scene->addCamera();
	// scene->setActiveCamera(cameraID);
	//
	// scene->getActiveCamera().setTransform(Transform(Vector2((16 * 30) / 2, (9 * 30) / 2)));
	// scene->getActiveCamera().setWidth(16 * 30);
	// scene->getActiveCamera().setHeight(9 * 30);

	Camera* camera = new Camera;
	camera->setTag("MainCamera");
	camera->setActive(true);

	camera->setTransform(Transform(Vector2((16 * 30) / 2, (9 * 30) / 2)));
	camera->setWidth(16 * 30);
	camera->setHeight(9 * 30);

	scene->addGameObject(camera);

	sceneManager.requestSceneChange("MainMenuScene");
}
