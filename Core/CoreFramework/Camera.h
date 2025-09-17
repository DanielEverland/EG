// Copyright (c) 2025 Daniel Everland
// All rights reserved.

#pragma once

#include "DataStructrues/Vector.h"
#include "Utilities/SingletonHelpers.h"

class Camera
{
public:
    static Camera& Get()
    {
        return SingletonHelper::Impl<Camera>();
    }

    IntVector2D& GetPosition() { return CameraPosition; }
    IntVector2D ToViewSpace(const IntVector2D& worldPosition) const;

private:
    IntVector2D CameraPosition = IntVector2D(0, 0);
};
