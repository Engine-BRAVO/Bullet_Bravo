#ifndef DEMOMANAGERBEHAVIOURSCRIPT_H
#define DEMOMANAGERBEHAVIOURSCRIPT_H

#include <Components/IBehaviourScript.h>
#include <GameObject.h>
#include <TileMapParser.h>

class DemoManagerBehaviourScript : public IBehaviourScript {
public:
    void onStart() override;
    void onUpdate() override;

    void createDemoStartScene();
    void createFirstScene();
    void createSecondScene();
    void createStressTest();
    void nextScene(const std::string& aSceneName);

    std::unique_ptr<Component> clone() const override { return std::make_unique<DemoManagerBehaviourScript>(*this); }

private:
    void saveGame();
    void loadGame();
    void handleSaveGame();

    
private:
    int mCurrentScene;
    bool mPlayerPositionSet = false;
    TileMapData mTileMapData;
};

#endif
