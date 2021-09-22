#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <ObjectFactory.h>

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
    ASSERT_EQ(f.generateNext(), (GameObject{"car", {"red"}, 10500}));
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
    }]
})"_json;
    auto f = ObjectFactory(json, 0);
    ASSERT_EQ(f.generateNext(), (GameObject { "car", {"dirty"}, 7500 }));
    ASSERT_EQ(f.generateNext(), (GameObject { "car", {"dirty", "red"}, 7875 }));
    ASSERT_EQ(f.generateNext(), (GameObject { "car", {"red", "dirty"}, 7875 }));
    ASSERT_EQ(f.generateNext(), (GameObject { "car", {"red", "dirty", "shiny"}, 15750 }));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#pragma clang diagnostic pop