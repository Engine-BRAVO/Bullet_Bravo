#include <iostream>

#include "PlayerPrefab.h"
#include "SplashScreenBehaviourScript.h"
#include <EngineBravo.h>
#include <GameObject.h>
#include <Scene.h>
#include <SceneManager.h>

void init()
{
	EngineBravo::getInstance().initialize();
	SceneManager& sceneManager = EngineBravo::getInstance().getSceneManager();

	Scene* scene = sceneManager.createScene("SplashScreenScene");
	GameObject* initObject = new GameObject;
	initObject->addComponent<SplashScreenBehaviourScript>();

	scene->addGameObject(initObject);

	Camera* camera = new Camera;
	camera->setTag("MainCamera");
	camera->setActive(true);

	camera->setTransform(Transform(Vector2(100, 100)));
	camera->setWidth(200);
	camera->setHeight(200);

	scene->addGameObject(camera);

	sceneManager.requestSceneChange("SplashScreenScene");

	EngineBravo::getInstance().run();

	return;
}

void test()
{
	World world(Vector2(0, 10));

	GameObject* playerObject = PlayerPrefabFactory::createPlayerPrefab();

	BodyProxy bodyProxy(*playerObject);
	BodyID playerBodyID = world.createBody(bodyProxy);

	for (int i = 0; i < 100; i++)
	{
		world.executeWorldStep(1, 40);
		std::cout << "Player position: " << world.getPosition(playerBodyID).x << ", "
				  << world.getPosition(playerBodyID).y << std::endl;
	}
}

int main()
{
	init();
	// test();
	return 0;
}
