#include "utils.h"
#include <gtest/gtest.h>

TEST(basic, get_height_of_text) {
    ASSERT_EQ(getHeight("Hello"), 1);
    ASSERT_EQ(getHeight("Hello\n\n"), 3);
    ASSERT_EQ(getHeight("\nHel\nlo\n"), 4);
}

/*This main is for testing GUI module*/
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}