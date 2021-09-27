#pragma once

#include <vector>
#include <list>
#include "Gamer.h"
#include "ObjectFactory.h"
#include "ViewableContainer.h"

class World: public ViewableContainer<GameObject, GameObject::Id> {
    friend class game_logic_profitness_Test;

private:
    std::vector<std::shared_ptr<Gamer>> players;
    unsigned int availableSlots;
    ObjectFactory factory;

public:
    explicit World(ObjectFactory &&factory = ObjectFactory::empty(), unsigned int maxSlots = 10);

    [[nodiscard]] double getProfitness(const GameObject::Id& itemId) const;

    std::vector<GameObject::Id> getSlots() const;

    void addGamer(std::shared_ptr<Gamer> &&gamer);

    void fillUp();

    std::unique_ptr<GameObject> takeItem(GameObject::Id itemId) override;
};