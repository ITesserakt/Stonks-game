#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include "utils.h"
#include <gtest/gtest.h>
#include "widgets/Group.h"

TEST(logic, get_height_of_text) {
    ASSERT_EQ(getHeight("Hello"), 1);
    ASSERT_EQ(getHeight("Hello\n\n"), 3);
    ASSERT_EQ(getHeight("\nHel\nlo\n"), 4);
}

TEST(api, widget_creation) {
    auto g = Group("Test");

    ASSERT_EQ(g.getName(), "Test");
    ASSERT_EQ(g.getChildren().size(), 0);
    ASSERT_EQ(g.getParent().lock(), nullptr);
    ASSERT_EQ(g.getCanvas(), nullptr);
}

TEST(api, right_child_creation) {
    auto g1 = std::make_shared<Group>("Main");
    auto g2 = Group("Test");

    ASSERT_NO_THROW({
                        g1->bind(std::move(g2));
                    });
    ASSERT_EQ(g1->getName(), "Main");

    auto g1Children = g1->getChildren();
    ASSERT_EQ(g1Children.size(), 1);
    ASSERT_EQ(g1Children[0]->getName(), "Test");
    ASSERT_EQ(g1Children[0]->getParent().lock(), g1);
}

TEST(api, wrong_child_creation) {
    auto g1 = Group("Main");
    auto g2 = Group("Test");

    g1.bind(std::move(g2));
    ASSERT_NO_THROW({
                        ASSERT_EQ(g1.getChildren()[0]->getParent().lock(), nullptr);
                    });
}

/*This main is for testing GUI module*/
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#pragma clang diagnostic pop