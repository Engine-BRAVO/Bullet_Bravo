#ifndef NETWORKSELECTIONBEHAVIOURSCRIPT_H
#define NETWORKSELECTIONBEHAVIOURSCRIPT_H

#include "Components/IBehaviourScript.h"
#include "UI/Button.h"

#include <list>
#include <vector>

class NetworkSelectionBehaviourScript : public IBehaviourScript {
public:
    NetworkSelectionBehaviourScript() {}
    ~NetworkSelectionBehaviourScript() {}

    std::unique_ptr<Component> clone() const override { return std::make_unique<NetworkSelectionBehaviourScript>(*this); }

    void onStart() override;

    void onUpdate() override;

private:
    Button* mServerButton{nullptr};
    Button* mClientButton{nullptr};
    Button* mHostButton{nullptr};
    Button* mSearchButton{nullptr};

    std::list<std::string> mServerAddresses;
};
#endif // NETWORKSELECTIONBEHAVIOURSCRIPT_H