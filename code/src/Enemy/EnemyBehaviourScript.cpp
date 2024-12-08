#include "EnemyBehaviourScript.h"
#include "BulletBehaviourScript.h"
#include "BSCoinPrefab.h"
#include "ECCoinPrefab.h"
#include <RigidBody.h>
#include <Animation.h>
#include <GameObject.h>
#include <EngineBravo.h>
#include <SceneManager.h>
#include <iostream>
#include <Sprite.h>
#include <Time.h>

// EnemyBehaviourScript::EnemyBehaviourScript(std::unique_ptr<Pathfinding>&& aPathfinding, int aMapWidth, int aMapHeight)
//     : mHealth(100.0f), mIsDead(false), mPathfinding(std::move(aPathfinding)), mMapWidth(aMapWidth), mMapHeight(aMapHeight), mCurrentPathIndex(0) {
//     // Debug statements to verify map dimensions
//     std::cout << "EnemyBehaviourScript initialized with Map Width: " << mMapWidth << ", Map Height: " << mMapHeight << std::endl;
// }

EnemyBehaviourScript::EnemyBehaviourScript(float aHealth)
    : mHealth(aHealth) {
}

EnemyBehaviourScript::EnemyBehaviourScript(const EnemyBehaviourScript& other)
    : mHealth(other.mHealth), mIsDead(other.mIsDead), mMapWidth(other.mMapWidth), mMapHeight(other.mMapHeight),
      mCurrentPathIndex(other.mCurrentPathIndex), mPathUpdateTime(other.mPathUpdateTime), mPreviousPlayerPosition(other.mPreviousPlayerPosition) {
    if (other.mPathfinding) {
        mPathfinding = std::make_unique<Pathfinding>(*other.mPathfinding);
    }
}

void EnemyBehaviourScript::onStart() {
    mIsDead = false;
    mPreviousPlayerPosition = Vector2(0, 0);
}

void EnemyBehaviourScript::onUpdate() {

}

void EnemyBehaviourScript::onCollide(GameObject* aGameObject) {
    if (mIsDead) {
        return;
    }

    if (aGameObject->getTag() == "Bullet") {
        BulletBehaviourScript* bullet = aGameObject->getComponents<BulletBehaviourScript>().at(0);
        if (bullet != nullptr) {
            takeDamage(bullet->getDamage());
        }
        else{
            std::runtime_error("BulletBehaviourScript not found in EnemyBehaviourScript::onCollide");
        }
    }
}

void EnemyBehaviourScript::toggleAnimaionEnabled() {
}

void EnemyBehaviourScript::setAnimationActive(std::string aAnimationTag, bool aState) {
}

void EnemyBehaviourScript::deactivateAllAnimations() {
    for (auto animation : mGameObject->getComponents<Animation>()) {
        animation->setActive(false);
    }
}

void EnemyBehaviourScript::setFlipX(bool aState) {
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

void EnemyBehaviourScript::setFlipY(bool aState) {
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

void EnemyBehaviourScript::takeDamage(float aDamage) {
    if (mHealth <= 0) {
        onDeath();
    }
    else {
        mHealth -= aDamage;
    }
}

void EnemyBehaviourScript::onDeath()
{
    if (mIsDead) {
        return;
    }
    mIsDead = true;

    EngineBravo& engine = EngineBravo::getInstance();
    SceneManager& sceneManager = engine.getSceneManager();
    if (mGameObject != nullptr) {
      //  std::cout << "Creating Coins" << std::endl;
       // BSCoinPrefab coinPrefab;
       // GameObject* coin = coinPrefab.createBSCoinPrefab(*mGameObject);
       // sceneManager.getCurrentScene()->addGameObject(coin);

      //  ECCoinPrefab coinPrefab2;
       // GameObject* coin2 = coinPrefab2.createECCoinPrefab(*mGameObject);
      //  sceneManager.getCurrentScene()->addGameObject(coin2);

        std::cout << "Removing GameObject: " << mGameObject->getTag() << std::endl;
        sceneManager.getCurrentScene()->requestGameObjectRemoval(mGameObject);

    } 
    else {
        std::cerr << "Error: mGameObject is already null!" << std::endl;
    }
}

int EnemyBehaviourScript::getGridPosition(const Vector2& position) const {
    int gridX = static_cast<int>(position.x) / 16;
    int gridY = static_cast<int>(position.y) / 16;

    // Ensure gridX and gridY are within valid bounds
    if (gridX < 0 || gridX >= mMapWidth || gridY < 0 || gridY >= mMapHeight) {
        return -1; // Return an invalid position
    }

    int gridPosition = gridY * mMapWidth + gridX;

    return gridPosition;
}

bool EnemyBehaviourScript::isValidPosition(int position) const {
    return mPathfinding->getAdjacencyList().find(position) != mPathfinding->getAdjacencyList().end();
}

float EnemyBehaviourScript::vectorLength(const Vector2& vec) { return std::sqrt(vec.x * vec.x + vec.y * vec.y); }

Vector2 EnemyBehaviourScript::normalizeVector(const Vector2& vec)
{
	float length = vectorLength(vec);
	if (length != 0)
	{
		return Vector2(vec.x / length, vec.y / length);
	}
	return vec;
}

void EnemyBehaviourScript::moveWithPathfinding() {
    EngineBravo& engine = EngineBravo::getInstance();
    Scene* scene = engine.getSceneManager().getCurrentScene();
    GameObject* enemy = mGameObject;
    
    if (enemy == nullptr) {
        std::cout << "Enemy not found" << std::endl;
        return;
    }
    
    GameObject* player = scene->getGameObjectsWithTag("Player")[0];
    if (player == nullptr) {
        std::cout << "Player not found" << std::endl;
        return;
    }
    
    Vector2 playerPosition = player->getTransform().position;
    int enemyPosition = getGridPosition(enemy->getTransform().position);
    int playerGridPosition = getGridPosition(playerPosition);
    
    if (enemyPosition == -1 || playerGridPosition == -1 ||
        !isValidPosition(enemyPosition) || !isValidPosition(playerGridPosition)) {
        return;
    }
    
    // Recalculate path if the player has moved significantly or after a certain time interval
    float distanceToPreviousPosition = vectorLength(playerPosition - mPreviousPlayerPosition);
    mPathUpdateTime += Time::deltaTime;
    
    if (distanceToPreviousPosition > 16.0f || mPathUpdateTime > 1.0f) {
        mPath = mPathfinding->findPath(enemyPosition, playerGridPosition);
        mCurrentPathIndex = 0;
        mPreviousPlayerPosition = playerPosition;
        mPathUpdateTime = 0.0f;
    }
    
    if (!mPath.empty() && mCurrentPathIndex < mPath.size() - 1) {
        int nextPosition = mPath[mCurrentPathIndex + 1];
        if (!isValidPosition(nextPosition)) {
            return;
        }
        
        int nextX = nextPosition % mMapWidth;
        int nextY = nextPosition / mMapWidth;
        
        Vector2 currentPosition = enemy->getTransform().position;
        Vector2 targetPosition(nextX * 16, nextY * 16);
        
        // Get Rigidbody component
        if (enemy->hasComponent<RigidBody>() == false) {
            return;
        }
        
        RigidBody* rigidbody = enemy->getComponents<RigidBody>()[0];
        
        // Compute direction and distance
        Vector2 direction = targetPosition - currentPosition;
        float distanceToTarget = vectorLength(direction);
        
        // Normalize direction
        direction = normalizeVector(direction);
        
        // Compute force with proportional control
        float maxForce = 10000.0f;  // Maximum force to apply
        float arrivalThreshold = 4.0f;  // Distance to start slowing
        
        // Proportional force - slow down as we approach the target
        float forceMagnitude = maxForce;
        if (distanceToTarget < arrivalThreshold) {
            // Linearly reduce force as we get closer
            forceMagnitude = maxForce * (distanceToTarget / arrivalThreshold);
        }
        
        // Apply force
        Vector2 force = direction * forceMagnitude * Time::deltaTime;
        rigidbody->addForce(force);
        
        // Check if we've reached the next path point
        if (distanceToTarget <= 16.0f) {
            mCurrentPathIndex++;
            
            //If very close to target, snap to exact position
            if (distanceToTarget <= 1.0f) {
               // enemy->getTransform().position = targetPosition;
                Transform enemyTransform = enemy->getTransform();
                enemyTransform.position = targetPosition;
                enemy->setTransform(enemyTransform);
            }
        }
    }
}

void EnemyBehaviourScript::setPathfinding(std::unique_ptr<Pathfinding>&& aPathfinding) {
    mPathfinding = std::move(aPathfinding);
}

void EnemyBehaviourScript::setMapWidth(int aMapWidth) {
    mMapWidth = aMapWidth;
}

void EnemyBehaviourScript::setMapHeight(int aMapHeight) {
    mMapHeight = aMapHeight;
}

int EnemyBehaviourScript::getMapWidth() const {
    return mMapWidth;
}

int EnemyBehaviourScript::getMapHeight() const {
    return mMapHeight;
}

std::unique_ptr<Pathfinding> EnemyBehaviourScript::getPathfinding() const {
    return std::make_unique<Pathfinding>(*mPathfinding);
}
