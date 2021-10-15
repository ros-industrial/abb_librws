#include <gtest/gtest.h>

#include "rws_rapid_test.h"

namespace abb :: rws
{
    TEST(RAPIDArrayTest, testCtorThrowsWhenInitialiserListHasDifferentSize)
    {
        bool has_thrown = false;
        try {
            RAPIDArray<MockRAPIDAtomic, 1> test_array
            {
                MockRAPIDAtomic {"data_1"},
                MockRAPIDAtomic {"data_2"}
            };
        }
        catch (std::invalid_argument)
        {
            has_thrown = true;
        }

        EXPECT_TRUE(has_thrown);
        has_thrown = false;

        try {
            RAPIDArray<MockRAPIDAtomic, 2> test_array
            {
                MockRAPIDAtomic {"data_1"}
            };
        }
        catch (std::invalid_argument)
        {
            has_thrown = true;
        }

        EXPECT_TRUE(has_thrown);
    }


    TEST(RAPIDArrayTest, testConstructStringOneDimensionArray)
    {
        RAPIDArray<MockRAPIDAtomic, 3> test_array
        {
            MockRAPIDAtomic {"data_1"},
            MockRAPIDAtomic {"data_2"},
            MockRAPIDAtomic {"data_3"}
        };

        std::string expected_constructed_string = "[data_1,data_2,data_3]";

        EXPECT_EQ(test_array.constructString(), expected_constructed_string);
    }

    TEST(RAPIDArrayTest, testConstructStringNestedArray)
    {
        RAPIDArray<RAPIDArray<MockRAPIDAtomic, 3>, 2> test_nested_array 
        {
            RAPIDArray<MockRAPIDAtomic, 3>
            {
                MockRAPIDAtomic {"data_1"},
                MockRAPIDAtomic {"data_2"},
                MockRAPIDAtomic {"data_3"}
            },
            RAPIDArray<MockRAPIDAtomic, 3>
            {
                MockRAPIDAtomic {"data_1"},
                MockRAPIDAtomic {"data_2"},
                MockRAPIDAtomic {"data_3"}
            }
        };
        std::string expected_constructed_string =
            "["
              "[data_1,data_2,data_3],"
              "[data_1,data_2,data_3]"
            "]";

        EXPECT_EQ(test_nested_array.constructString(), expected_constructed_string);
    }

    TEST(RAPIDArrayTest, testParseStringOneDimensionArray)
    {
        RAPIDArray<MockRAPIDAtomic, 4> test_simple_array;
        const std::vector<std::string> test_data = 
        {
            "I", "am", "your", "father"
        };

        std::string rapid_message = "[I,am,your,father]";
        test_simple_array.parseString(rapid_message);

        for(size_t i = 0; i < 4 ; ++i) 
        {
            EXPECT_EQ((std::string)test_simple_array.at(i), test_data.at(i));
        }
        EXPECT_EQ(test_simple_array.constructString(), rapid_message);
    }

    TEST(RAPIDArrayTest, testParseStringThrowsWhenPassingBadValueString)
    {
        RAPIDArray<MockRAPIDAtomic, 3> test_array;
        std::string rapid_message = "[I,am,your,father]";

        EXPECT_THROW(
            {
                test_array.parseString(rapid_message);
            }
            , std::invalid_argument);
    }

    TEST(RAPIDArrayTest, testParseStringNestedArray)
    {
        RAPIDArray<RAPIDArray<MockRAPIDAtomic, 5>, 2> test_nested_array
        {
            RAPIDArray<MockRAPIDAtomic, 5>(),
            RAPIDArray<MockRAPIDAtomic, 5>()
        };

        const std::vector<std::vector<std::string>> test_data =
        {
            {"No","I", "am", "your", "father"},
            {"Sator","Apero","Tenet","Opera","Rotas"}
        };
        std::string rapid_message = 
            "["
              "[No,I,am,your,father],"
              "[Sator,Apero,Tenet,Opera,Rotas]"
            "]";

        test_nested_array.parseString(rapid_message);

        for(size_t i = 0 ; i < 2 ; ++i)
        {
            for(size_t j = 0 ; j < 5 ; ++j)
            {
                EXPECT_EQ((std::string)test_nested_array.at(i).at(j), test_data.at(i).at(j));
            }
        }

        EXPECT_EQ(test_nested_array.constructString(), rapid_message);
    }

    TEST(RAPIDArrayTest, testGetType)
    {
        RAPIDArray<MockRAPIDAtomic, 1> test_array;
        EXPECT_EQ(test_array.getType(), MockRAPIDAtomic().getType());

        RAPIDArray<RAPIDArray<MockRAPIDAtomic, 1>, 1> test_nested_array;
        EXPECT_EQ(test_nested_array.getType(), MockRAPIDAtomic().getType());

    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}