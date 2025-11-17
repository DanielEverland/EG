// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "NavResult.h"

struct NavRequest;

class Navigation
{    
public:
    [[nodiscard]] NavResult TryCalculatePath(const NavRequest& request) const;

    static double GetHeuristic(const IntVector& pos, const IntVector& target);

    void SetMaxIterations(int32_t maxIterations) { MaxIterations = maxIterations; }

private:
    int32_t MaxIterations = 500;
};