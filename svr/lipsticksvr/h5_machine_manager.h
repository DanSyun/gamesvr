#pragma once

#include "type_def.h"
#include "singleton.hpp"

class H5MachineManager: public Singleton<H5MachineManager>
{
public:
    void Init();
    int32 Alloc();
    void Release(uint64 machine_id);
private:
    char* _machine_arr;
    uint32 _size;
};
