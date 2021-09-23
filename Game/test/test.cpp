#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <exception>
#include <ObjectFactory.h>
#include <World.h>

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
    auto f = ObjectFactory::fromText(json);
    ASSERT_EQ(f.generateNext(), (GameObject{"car", {}, 0, 10000, 0}));
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
    ASSERT_EQ(f.generateNext(), (GameObject{"car", {}, 0, 10000, 0}));
    ASSERT_EQ(f.generateNext(), (GameObject{"car", {"dirty"}, 1, 7500, 0}));
    ASSERT_EQ(f.generateNext(), (GameObject{"laba, tipovik", {}, 2, 1000000, 0}));
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
    auto f = ObjectFactory(json, 6);
    auto w = World(std::move(f));
    std::vector<double> expected = {0.455, 2.2275, 0.355, 0.101667, 0.1275};

    for (int x: {0, 1, 2, 3, 4}) {
        w.slots.push_back(std::make_unique<GameObject>(w.factory.generateNext()));
        w.availableSlots--;
        ASSERT_NEAR(w.getProfitness(x), expected[x], 1e-5);
    }
    EXPECT_THAT(w.getSlots(), (testing::ElementsAreArray({0, 1, 2, 3, 4})));
}

TEST(game_logic, non_existing_gameobject) {
    auto json = R"({"Objects":[]})"_json;
    auto f = ObjectFactory(json);
    auto w = World(std::move(f));
    ASSERT_THROW({
        w.getProfitness(1);
    }, std::runtime_error);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#pragma clang diagnostic pop