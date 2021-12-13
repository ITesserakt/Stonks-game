#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include "utils.h"
#include <gtest/gtest.h>
#include "widgets/Group.h"
#include "Canvas.h"

TEST(logic, get_height_of_text) {
    ASSERT_EQ(getHeight("Hello"), 1);
    ASSERT_EQ(getHeight("Hello\n\n"), 3);
    ASSERT_EQ(getHeight("\nHel\nlo\n"), 4);
}

TEST(logic, get_max_width_of_text) {
    ASSERT_EQ(getWidth("4let"), 4);
    ASSERT_EQ(getWidth("4let\n"
                       "8letters"), 8);
    ASSERT_EQ(getWidth("4let\n"
                       "8letters\n"
                       "10letters "), 10);
    ASSERT_EQ(getWidth(""), 0);
}

TEST(logic, split_sentence_by_lines) {
    ASSERT_EQ(splitBySentence("Aboba", 0), "Aboba");
    ASSERT_EQ(splitBySentence("reset\nconfig", 0), "reset ");
    ASSERT_EQ(splitBySentence("Aboba\nBob", 1), "Bob  ");
    ASSERT_EQ(splitBySentence("\n\n\n", 1), "");
    ASSERT_EQ(splitBySentence("Hey\nBro\nHo", 1), "Bro");
}

TEST(api, widget_creation) {
    auto g = Group("Test");

    ASSERT_EQ(g.getName(), "Test");
    ASSERT_EQ(g.getChildren().size(), 0);
    ASSERT_EQ(g.getParent().lock(), nullptr);
}

TEST(api, right_child_creation) {
    auto g1 = std::make_shared<Group>("Main");
    auto g2 = std::make_shared<Group>("Test");

    ASSERT_NO_THROW({
                        g1->bind(g2);
                    });
    ASSERT_EQ(g1->getName(), "Main");

    auto g1Children = g1->getChildren();
    ASSERT_EQ(g1Children.size(), 1);
    ASSERT_EQ(g1Children[0]->getName(), "Test");
    ASSERT_EQ(g1Children[0]->getParent().lock(), g1);
}

TEST(api, wrong_child_creation) {
    auto g1 = Group("Main");
    auto g2 = std::make_shared<Group>("Test");

    g1.bind(g2);
    ASSERT_NO_THROW({
                        ASSERT_EQ(g1.getChildren()[0]->getParent().lock(),
                                  nullptr);
                    });
}

TEST(api, get_canvas) {
    auto can = std::make_shared<Canvas>("Main menu", Centered);
    auto but = std::make_shared<Group>("Group");
    can->bind(but);

    ASSERT_EQ(can->getParent().lock(), nullptr);
}

/*This main is for testing GUI module*/
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#pragma clang diagnostic pop