#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <exception>
#include <ObjectFactory.h>
#include <World.h>
#include <Player.h>

TEST(game_logic, config_parsing) {
    auto json = R"({
    "Objects" : [{
        "name" : "car",
        "descriptions": [{
            "value" : "red",
            "multiplier" : 1.05
        }],
        "cost": 10000.0
    }]
})";
    auto f = ObjectFactory(nlohmann::json::parse(json), 3);
    ASSERT_EQ(f.generateNext(), (GameObject{"car", {}, 0, 10000}));
}

TEST(game_logic, empty_factory) {
    ASSERT_NO_FATAL_FAILURE({
                                auto f = ObjectFactory::empty();
                                ASSERT_THROW({f.generateNext();}, std::runtime_error);
                            });
}

TEST(game_logic, objects_generating) {
    auto json = R"({
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
        "name": "laba, tipovik",
        "descriptions": [],
        "cost": 1000000
    }]
})"_json;
    auto f = ObjectFactory(json, 0);
    ASSERT_EQ(f.generateNext(), (GameObject{"car", {"dirty", "shiny", "red"}, 0, 15750}));
    ASSERT_EQ(f.generateNext(), (GameObject{"car", {"dirty", "red", "shiny"}, 1, 15750}));
    ASSERT_EQ(f.generateNext(), (GameObject{"laba, tipovik", {}, 2, 1000000}));
}

TEST(game_logic, objects_cost) {
    auto json = R"({
    "Objects" : [{
        "name" : "car",
        "descriptions": [],
        "cost": 10000.0
    }, {
        "name": "dress",
        "descriptions": [],
        "cost": 10
    }, {
        "name": "laptop",
        "descriptions": [],
        "cost": 700
    }]
})";
    auto f = ObjectFactory::fromText(json);
    ASSERT_FLOAT_EQ(f.getCostForKind("car"), 10000);
    ASSERT_FLOAT_EQ(f.getCostForKind("dress"), 10);
    ASSERT_FLOAT_EQ(f.getCostForKind("laptop"), 700);
}

TEST(game_logic, profitness) {
    auto json = R"({
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
    auto f = ObjectFactory(json, 9);
    auto w = World(std::move(f));
    std::vector<double> expected = {0, 0, 1.29603174, 2.8466666, 0};

    for (int x: {0, 1, 2, 3, 4}) {
        auto next = w.factory.generateNext();
        w.container[next.id] = std::make_unique<GameObject>(next);
        w.availableSlots--;
        auto p = w.getProfitness(x);
        ASSERT_NEAR(p, expected[x], 1e-5) << next << ", profitness: " << p << std::endl;
    }
    EXPECT_THAT(w.getSlots(), (testing::ElementsAreArray({0, 1, 2, 3, 4})));
}

TEST(game_logic, non_existing_gameobject) {
    auto w = World();
    ASSERT_THROW({
                     w.getProfitness(1);
                 }, std::runtime_error);
    ASSERT_THROW({
                     w.viewItem(1);
                 }, std::runtime_error);
    ASSERT_THROW({
                     w.takeItem(1);
                 }, std::runtime_error);
}

TEST(game_logic, trade_properties) {
    auto w = World(ObjectFactory(R"({
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
})"_json));
    w.fillUp();
    auto p = std::make_shared<Player>();
    w.addGamer(p);
    p->buyItem(w.takeItem(0));
    ASSERT_THROW({ w.takeItem(0); }, std::runtime_error);
    ASSERT_THROW({ w.viewItem(0); }, std::runtime_error);
    auto item = p->Gamer::sellItem(0);
    ASSERT_EQ(item->timesSold, 1);
    ASSERT_EQ(item->lastSeller, p);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#pragma clang diagnostic pop