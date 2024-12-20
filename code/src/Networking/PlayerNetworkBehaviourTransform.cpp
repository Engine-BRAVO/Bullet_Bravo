#include "PlayerNetworkBehaviourTransform.h"

// Register the type
REGISTER_NETWORK_SERIALIZABLE(transformSerializable);

PlayerNetworkBehaviourTransform::PlayerNetworkBehaviourTransform()
	: mTransformSerialize{this}, INetworkBehaviour("PlayerNetworkBehaviourTransform")
{
}

PlayerNetworkBehaviourTransform::PlayerNetworkBehaviourTransform(const PlayerNetworkBehaviourTransform& other)
	: INetworkBehaviour(other), mTransformSerialize{this}
{
}

void PlayerNetworkBehaviourTransform::onUpdate()
{
	if (isServer())
	{
		mTransformSerialize.getValue().setPosition(mGameObject->getTransform().position); // Set the network variable
		mTransformSerialize.getValue().setRotation(mGameObject->getTransform().rotation); // Set the network variable
	}
	else
	{
		Transform networkTransform(mTransformSerialize.getValue().getPosition());
		networkTransform.rotation = mTransformSerialize.getValue().getRotation();
		mGameObject->setTransform(networkTransform); // Read the network variable
													 // Transform localTransform(mGameObject->getTransform());
													 // Transform avgTransform;
													 // avgTransform = (networkTransform + localTransform) / 2;
		// mGameObject->setTransform(avgTransform); // Read the network variable
	}
}