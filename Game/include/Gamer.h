#pragma once

#include <atomic>
#include <memory>
#include <vector>

#include "GameObject.h"
#include "TaxStrategy.h"
#include "ViewableContainer.h"

class Gamer : public std::enable_shared_from_this<Gamer>, public ViewableContainer<GameObject, GameObject::Id> {
protected:
    double money;

    // when game is restarted id will be restored
    // else id will be new
    const unsigned int  id;
    static unsigned int IdGenerator;

    std::atomic<unsigned int> availableSlots;

    Gamer(double                                                  money,
            unsigned int                                          availableSlots,
            unsigned int                                          gamerId,
            std::map<GameObject::Id, std::unique_ptr<GameObject>> container);

    Gamer();

    Gamer(Gamer &&g) noexcept;

public:
    virtual ~Gamer() = default;

    virtual void                        buyItem(std::unique_ptr<GameObject> item);
    virtual std::unique_ptr<GameObject> sellItem(GameObject::Id itemId, GameObject::Cost newCost);
    std::unique_ptr<GameObject>         sellItem(GameObject::Id itemId);
    double                              getBalance() const;
    unsigned int                        getId() const;
    bool                                couldBuy() const;
    void                                takeTaxes(TaxStrategy &strategy);
};
