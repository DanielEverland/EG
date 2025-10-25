// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once
#include <assert.h>
#include <filesystem>

class DirectoryHelpers
{
public:
    // Returns true if child is a subpath of parent
    // TRUE: parent:a/b, child:a/
    // FALSE: parent:b/, child a/
    static bool IsSubPath(const std::filesystem::path& parent, const std::filesystem::path& child)
    {
        const std::string& parentPath = parent.string();
        const std::string& childPath = child.string();
        
        auto pIter = parentPath.begin();
        auto cIter = childPath.begin();

        while (cIter != childPath.end() && (*pIter) == (*cIter))
        {
            ++pIter;
            ++cIter;
        }

        return cIter == childPath.end();
    }
};
