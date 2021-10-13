#include <gtest/gtest.h>

#include "rws_rapid_test.h"

namespace abb :: rws
{

    TEST(RAPIDArrayTest, testSimpleArrayStringConstruction)
    {
        RAPIDArray<MockRAPIDAtomic> test_array {
            "mock_data",
            std::vector<MockRAPIDAtomic> {
                MockRAPIDAtomic {"data_1"},
                MockRAPIDAtomic {"data_2"},
                MockRAPIDAtomic {"data_3"}
            }
        };

        std::string expected_constructed_string = "[data_1,data_2,data_3]";

        EXPECT_EQ(test_array.constructString(), expected_constructed_string);
    }

    TEST(RAPIDArrayTest, testDoubleArrayStringConstruction)
    {
        RAPIDArray<RAPIDArray<MockRAPIDAtomic>> test_double_array {
            "array",
            2,
            RAPIDArray<MockRAPIDAtomic> {"bool",
            std::vector<MockRAPIDAtomic> {
                MockRAPIDAtomic {"data_1"},
                MockRAPIDAtomic {"data_2"},
                MockRAPIDAtomic {"data_3"}
            }
            }
        };
        std::string expected_constructed_string =
            "["
              "[data_1,data_2,data_3],"
              "[data_1,data_2,data_3]"
            "]";

        EXPECT_EQ(test_double_array.constructString(), expected_constructed_string);
    }

    TEST(RAPIDArrayTest, testParseStringSimple)
    {
        RAPIDArray<MockRAPIDAtomic> test_simple_array {"mock_data", 4};
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

    TEST(RAPIDArrayTest, testParseStringDoubleArray)
    {
        RAPIDArray<RAPIDArray<MockRAPIDAtomic>> test_double_array {
            "array",
            2,
            RAPIDArray<MockRAPIDAtomic> {"mock_data", 5}
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

        test_double_array.parseString(rapid_message);

        for(size_t i = 0 ; i < 2 ; ++i)
        {
            for(size_t j = 0 ; j < 5 ; ++j)
            {
                EXPECT_EQ((std::string)test_double_array.at(i).at(j), test_data.at(i).at(j));
            }
        }

        EXPECT_EQ(test_double_array.constructString(), rapid_message);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}