#include "DemoLevel2Behaviour.h"
#include <EngineBravo.h>
#include <Animation.h>
#include <EnemyBehaviourScript.h>

DemoLevel2Behaviour::DemoLevel2Behaviour() : mMovingUp(true), mInitialY(0.0f) {}

void DemoLevel2Behaviour::onStart() {
    mGameObject->setTag("DemoLevel2");

    EngineBravo& engine = EngineBravo::getInstance();
    Scene* scene = engine.getSceneManager().getCurrentScene();

    GameObject* enemy = scene->getGameObjectsWithTag("EnemyMoving")[0];
    if (enemy != nullptr) {
        mInitialY = enemy->getTransform().position.y;
        if (enemy->hasComponent<RigidBody>()) {
            enemy->getComponents<RigidBody>()[0]->setActive(false);
        }
    }

    rotateEnemy();

    GameObject* enemyStatic = scene->getGameObjectsWithTag("EnemyStatic")[0];
    if (enemyStatic != nullptr) {
        if (enemyStatic->hasComponent<EnemyBehaviourScript>()) {
            enemyStatic->getComponents<EnemyBehaviourScript>()[0]->deactivateAllAnimations();
        }
    }
}

void DemoLevel2Behaviour::onUpdate() {
    moveEnemy();
    //scaleEnemy();
}

void DemoLevel2Behaviour::onCollide(GameObject* aGameObject) {
}

void DemoLevel2Behaviour::moveEnemy() {
    EngineBravo& engine = EngineBravo::getInstance();
    Scene* scene = engine.getSceneManager().getCurrentScene();

    GameObject* enemy = scene->getGameObjectsWithTag("EnemyMoving")[0];
    if (enemy == nullptr) {
        std::cout << "Enemy not found" << std::endl;
        return;
    }

    Transform transform = enemy->getTransform();
    Animation* idleAnimation = enemy->getComponents<Animation>()[0];
    Animation* walkingAnimation = enemy->getComponents<Animation>()[1];

    if (mMovingUp) {
        transform.position.y += 1.0f;
        if (transform.position.y >= mInitialY + 10.0f) {
            mMovingUp = false;
        }
    } else {
        transform.position.y -= 1.0f;
        if (transform.position.y <= mInitialY - 10.0f) {
            mMovingUp = true;
        }
    }

    if (abs(transform.position.y - mInitialY) > 0.1f) {
        idleAnimation->setActive(false);
        walkingAnimation->setActive(true);
    } else {
        idleAnimation->setActive(true);
        walkingAnimation->setActive(false);
    }

    enemy->setTransform(transform);
}

void DemoLevel2Behaviour::scaleEnemy() {
    EngineBravo& engine = EngineBravo::getInstance();
    Scene* scene = engine.getSceneManager().getCurrentScene();

    GameObject* enemy = scene->getGameObjectsWithTag("EnemyStatic")[0];
    if (enemy == nullptr) {
        std::cout << "Enemy not found" << std::endl;
        return;
    }

    Transform transform = enemy->getTransform();
    static bool scalingUp = true;

    if (scalingUp) {
        transform.scale.x += 0.01f;
        transform.scale.y += 0.01f;
        if (transform.scale.x >= 1.5f) {
            scalingUp = false;
        }
    } else {
        transform.scale.x -= 0.01f;
        transform.scale.y -= 0.01f;
        if (transform.scale.x <= 0.5f) {
            scalingUp = true;
        }
    }

    enemy->setTransform(transform);
}

void DemoLevel2Behaviour::rotateEnemy() {
    EngineBravo& engine = EngineBravo::getInstance();
    Scene* scene = engine.getSceneManager().getCurrentScene();

    GameObject* enemy = scene->getGameObjectsWithTag("EnemyStatic")[0];
    if (enemy == nullptr) {
        std::cout << "Enemy not found" << std::endl;
        return;
    }

    RigidBody* rigidBody = enemy->getComponents<RigidBody>()[0];
    if (rigidBody == nullptr) {
        std::cout << "RigidBody not found" << std::endl;
        return;
    }
    else {
        rigidBody->addTorque(5000.0f);
    }
}