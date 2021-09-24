#include "Gamer.h"

class Player : public Gamer {
public:
    void buyItem(std::unique_ptr<GameObject> item) override;

    std::unique_ptr<GameObject> sellItem(GameObject::Id itemId, GameObject::Cost newCost) override;
};