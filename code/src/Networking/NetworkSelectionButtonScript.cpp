#include "NetworkSelectionButtonScript.h"
#include "Button.h"
#include "Engine/EngineBravo.h"
#include "LevelManagerBehaviourScript.h"
#include "Network/NetworkManager.h"
#include "PlayerPrefab.h"
#include "Text.h"

#include <iostream>

NetworkSelectionButtonScript::NetworkSelectionButtonScript() {}

NetworkSelectionButtonScript::~NetworkSelectionButtonScript() {}

void NetworkSelectionButtonScript::onButtonPressed()
{
	Button* button = dynamic_cast<Button*>(mGameObject);
	if (button)
	{
		EngineBravo& engine = EngineBravo::getInstance();
		NetworkManager& networkManager = engine.getNetworkManager();

		if (button->getTag() == "ServerButton")
		{
			std::cout << "Server selected" << std::endl;
			if (networkManager.isConnected())
			{
				std::cout << "Network is already connected" << std::endl;
			}
			else
			{
				networkManager.setRole(NetworkRole::SERVER);
				networkManager.startNetwork();
			}
			networkManager.setDefaultPlayerPrefab(PlayerPrefabFactory::createPlayerPrefab());
			for (GameObject* object :
				 EngineBravo::getInstance().getSceneManager().getCurrentScene()->getGameObjectsWithTag("LevelManager"))
			{
				if (object->hasComponent<LevelManagerBehaviourScript>())
				{
					object->getComponents<LevelManagerBehaviourScript>()[0]->beginDemoNetworkingGame();
				}
			}
			setButtonsVisibility();
		}
		else if (button->getTag() == "ClientButton")
		{
			std::cout << "Client selected" << std::endl;
			if (networkManager.isConnected())
			{
				std::cout << "Network is already connected" << std::endl;
			}
			else
			{
				networkManager.setRole(NetworkRole::CLIENT);
				networkManager.startNetwork();
			}
			setButtonsVisibility();
		}
		else if (button->getTag() == "HostButton")
		{
			std::cout << "Host selected" << std::endl;
			if (networkManager.isConnected())
			{
				std::cout << "Network is already connected" << std::endl;
			}
			else
			{
				networkManager.setRole(NetworkRole::HOST);
				networkManager.startNetwork();
			}
			setButtonsVisibility();
		}
		else if (button->getTag() == "SearchButton")
		{
			std::cout << "Search selected" << std::endl;
			NetworkClient& networkClient = engine.getNetworkManager().getClient();
			networkClient.discoverServers();
		}
		else
		{
			std::cout << "Unknown tag" << button->getTag() << std::endl;
		}
	}
	else
	{
		std::cout << "Parent object not a button" << std::endl;
	}
}

void NetworkSelectionButtonScript::onButtonReleased() {}

void NetworkSelectionButtonScript::setButtonsVisibility()
{
	EngineBravo& engine = EngineBravo::getInstance();
	SceneManager& sceneManager = engine.getSceneManager();
	Scene* scene = sceneManager.getCurrentScene();

	for (auto& gameObject : scene->getGameObjects())
	{
		Button* button = dynamic_cast<Button*>(gameObject);
		Text* text = dynamic_cast<Text*>(gameObject);
		if (button)
		{
			button->setInteractable(!button->interactable());
		}
		if (text)
		{
			text->setActive(!text->isActive());
		}
	}
}

ConnectButtonScript::ConnectButtonScript(Text* aTextObject) : mTextObject(aTextObject) {}

ConnectButtonScript::~ConnectButtonScript() {}

void ConnectButtonScript::onButtonPressed()
{
	std::cout << "Connect button pressed" << std::endl;
	NetworkClient& networkClient = EngineBravo::getInstance().getNetworkManager().getClient();
	networkClient.setServerAddress(mTextObject->getText());
	networkClient.connectToServer();
}

void ConnectButtonScript::onButtonReleased() {}
