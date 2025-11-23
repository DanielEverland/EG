// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include <memory>

#include "NavigationGraphTraverser.h"

class Level;

class LevelNavigationGraphTraverser : public NavigationGraphTraverser
{
public:
    explicit LevelNavigationGraphTraverser(std::shared_ptr<Level> level);
    
    bool IsValidMove(Entity entity, const IntVector& from, const IntVector& to) override;
    bool TryGetTraversalDifficulty(const IntVector& from, const IntVector& to, uint8_t& outTraversalDifficulty) override;

private:
    std::shared_ptr<Level> Level;
};
