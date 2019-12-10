#pragma once

#include "base.h"
#include "share_mem.hpp"

class ShmPipe
{
public:
    ShmPipe(bool exist);
    void* operator new(size_t size);
    void Initialize();
    void Reuse();

    void GetIndex(uint32& begin, uint32& end);
    void SetIndex(uint32* begin, uint32* end);
    uint32 GetLeftSize();
    uint32 GetUsedSize();
    bool AppendOneData(const char* buf, uint32 buf_len);
    bool GetOneData(char* buf, uint32& buf_len);
private:
    uint32 _begin;
    uint32 _end;
    uint32 _size;
public:
    static ShareMem* pshm;
};

