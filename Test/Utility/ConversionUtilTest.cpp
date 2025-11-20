#include <optional>

#include "gtest/gtest.h"
#include "Utilities/ConversionUtil.h"

TEST(ConversionUtil, TryToInteger)
{
    {
        std::optional<int> val = ConversionUtil::TryToInteger<int>("0");
        ASSERT_TRUE(val.has_value());
        ASSERT_EQ(val.value(), 0);
    }
    {
        std::optional<int> val = ConversionUtil::TryToInteger<int>("f");
        ASSERT_FALSE(val.has_value());
    }
    {
        std::optional<uint8_t> val = ConversionUtil::TryToInteger<uint8_t>("255");
        ASSERT_TRUE(val.has_value());
        ASSERT_EQ(val.value(), 255);
    }
    {
        std::optional<uint8_t> val = ConversionUtil::TryToInteger<uint8_t>("256");
        ASSERT_FALSE(val.has_value());
    }
}