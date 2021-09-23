#include "Frontend.h"
#include <Player.h>
#include <World.h>
#include <AI.h>
#include "Utils.h"

static const nlohmann::json json = R"({
        "Objects" : [{
            "name" : "car",
            "descriptions": [{
                "value" : "red",
                "multiplier" : 1.05
            }, {
                "value": "shiny",
                        "multiplier": 2
            }, {
                "value" : "dirty",
                        "multiplier": 0.75
            }],
            "cost": 10000
        }, {
            "name": "dress",
            "descriptions": [{
                "value": "gucci",
                "multiplier": 15
            }],
            "cost": 10
        }]
})"_json;

int main() {
    auto p = std::make_shared<Player>();
    auto f = ObjectFactory(json);
    auto w = World(std::move(f));
    auto ai = std::make_shared<AI>(w);
    w.addPlayer(p);
    w.addPlayer(ai);
    w.fillUp();

    auto slots = w.getSlots();
    p->buyItem(w.takeItem(0));

    auto prediction = ai->predict();
    ai->buyItem(w.takeItem(prediction));

    p->buyItem(ai->sellItem(prediction, 100));
    std::cout << p->viewItem(0) << std::endl << p->viewItem(prediction) << std::endl;
    auto newPrediction = ai->predict();
    std::cout << std::endl;
    for (auto i : w.getSlots()) {
        std::cout << w.viewItem(i) << ", profitness: " << w.getProfitness(i) << std::endl;
    }
    std::cout << prediction << " " << newPrediction << std::endl << w.viewItem(newPrediction) << std::endl;
}