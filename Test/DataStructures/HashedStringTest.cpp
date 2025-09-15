#include "DataStructrues/HashedString.h"
#include "gtest/gtest.h"

TEST(HashedString, UniqueHash)
{
    constexpr char minChar = '!';
    constexpr int32_t uniqueCharacters = '~' - minChar;
    
    std::set<size_t> hashes;
    for (int i = 0; i < 10000; ++i)
    {
        std::string str;
        for (int j = 1; j <= i / uniqueCharacters + 1; ++j)
        {
            str += (i / j) % uniqueCharacters + minChar;
        }
        size_t hash = HashedString(str);
        EXPECT_FALSE(hashes.contains(hash));
        hashes.emplace(hash);
    }
}