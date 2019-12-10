#include "shm_pipe.h"

ShareMem* ShmPipe::pshm = NULL;

ShmPipe::ShmPipe(bool exist)
{
    if (!exist)
        Initialize();
    else
        Reuse();
}

void* ShmPipe::operator new(size_t size)
{
    if (!pshm)
        return NULL;
    if (size > pshm->GetByteSize())
        return NULL;

    return pshm->GetPtr();
}

void ShmPipe::Initialize()
{
    _begin = 0;
    _end = 0;
    _size = pshm->GetByteSize() - sizeof(*this);
    pshm = NULL;
}

void ShmPipe::Reuse()
{
    _size = pshm->GetByteSize() - sizeof(*this);
    pshm = NULL;
}

// 
void ShmPipe::GetIndex(uint32& begin, uint32& end)
{
    begin = _begin;
    end = _end;
}

void ShmPipe::SetIndex(uint32* begin, uint32* end)
{
    if (begin && *begin < _size)
        _begin = *begin;
    if (end && *end < _size)
        _end = *end;
}

uint32 ShmPipe::GetLeftSize()
{
    uint32 begin, end;
    GetIndex(begin, end);

    uint32 size = 0;
    if (begin == end)
        size = _size;
    else if (begin > end)
        size = begin - end;
    else
        size = _size - end + begin;

    size -= 1;  // 预留字节防止首尾相接
    return size;
}

uint32 ShmPipe::GetUsedSize()
{
    uint32 begin, end;
    GetIndex(begin, end);

    if (begin == end)
        return 0;
    else if (begin < end)
        return end - begin;
    else
        return _size + begin - end;
}

bool ShmPipe::AppendOneData(const char* buf, uint32 buf_len)
{
    uint32 left = GetLeftSize();
    if (buf_len + sizeof(uint32) > left) return false;

    char* tmp_buf = (char*)this + sizeof(ShmPipe);
    uint32 tmp_begin, tmp_end;
    GetIndex(tmp_begin, tmp_end);
    // 4字节总长
    for (uint32 i = 0; i < sizeof(uint32); ++i)
    {
        tmp_buf[tmp_end] = ((char*)&buf_len)[i];
        tmp_end = (tmp_end + 1)% _size;
    }
    if (tmp_begin > tmp_end)
    {
        memcpy(&tmp_buf[tmp_end], buf, buf_len);
    }
    else
    {
        uint32 pipe_left = _size - tmp_end;
        if (pipe_left >= buf_len)
        {
            memcpy(&tmp_buf[tmp_end], buf, buf_len);
        }
        else
        {
            memcpy(&tmp_buf[tmp_end], buf, pipe_left);
            memcpy(&tmp_buf[0], buf + pipe_left, buf_len - pipe_left);
        }
    }

    tmp_end = (tmp_end + buf_len)% _size;
    SetIndex(NULL, &tmp_end);

    return true;
}

bool ShmPipe::GetOneData(char* buf, uint32& buf_len)
{
    uint32 max_buf_len = buf_len;
    char* tmp_buf = (char*)this + sizeof(ShmPipe);
    uint32 tmp_begin, tmp_end;
    GetIndex(tmp_begin, tmp_end);

    uint32 used = GetUsedSize();
    if (used == 0) return false;
    if (used < sizeof(uint32))
    {
        tmp_begin = tmp_end;
        SetIndex(&tmp_begin, NULL);
        return false;
    }

    for (uint32 i = 0; i < sizeof(uint32); ++i)
    {
        ((char*)&buf_len)[i] = tmp_buf[tmp_begin];
        tmp_begin = (tmp_begin + 1)% _size;
    }
    if (buf_len + sizeof(uint32) > used)
    {
        tmp_begin = tmp_end;
        SetIndex(&tmp_begin, NULL);
        return false;
    }
    if (buf_len > max_buf_len) return false;
    if (tmp_begin < tmp_end)
    {
        memcpy(buf, &tmp_buf[tmp_begin], buf_len);
    }
    else
    {
        uint32 pipe_left = _size - tmp_begin;
        if (pipe_left >= buf_len)
        {
            memcpy(buf, &tmp_buf[tmp_begin], buf_len);
        }
        else
        {
            memcpy(buf, &tmp_buf[tmp_begin], pipe_left);
            memcpy(buf + pipe_left, &tmp_buf[0], buf_len - pipe_left);
        }
    }

    tmp_begin = (tmp_begin + buf_len)% _size;
    SetIndex(&tmp_begin, NULL);

    return true;
}

