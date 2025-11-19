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

    IntVector GetPosition() const { return CameraPosition; }
    void SetPosition(const IntVector& position);

    void SetSnapToPossessed(bool newVal) { SnapToPossessed = newVal; }
    bool GetSnapToPossessed() const { return SnapToPossessed; }
    
    IntVector ToViewSpace(const IntVector& worldPosition) const;

private:
    IntVector CameraPosition = IntVector(0, 0, 0);
    bool SnapToPossessed = true;
};
