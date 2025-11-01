#pragma once
#include "Transform.hpp"
struct ObjectBase {
    int Priority = 3;
    int nID = 0;
    bool Death = false;

    void SetPriority(const int Pri) { Priority = Pri; }
    void SetFlag(const bool Flag) { Death = Flag; }

    const int GetPriority(void) { return Priority; }
    const int GetID(void) { return nID; }
    const bool GetFlag(void) { return Death; }
};