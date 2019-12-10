#pragma once

#include "object_manager.hpp"
#include "singleton.hpp"
#include "base.h"
#include "proto_base.h"

class Timer: public PBTimer
{
private:
    typedef PBTimer base;
public:
    size_t GetByteSize();
    bool Serialize(uint8* buf, size_t size, size_t& offset);
    bool UnSerialize(uint8* buf, size_t size, size_t& offset);
};

class TimerManager:
    public ObjectManager<Timer>,
    public Singleton<TimerManager>
{
public:
    typedef ObjectManager<Timer> base;
    typedef std::unordered_map<uint32, uint32> index;
public:
    bool Init(uint32 size, uint32 shmkey = 0);
    void Tick();

    int CreateTimer(uint32 sec, const PBInternalMsg& internal_msg);
    int CreateTimerMsec(uint32 msec, const PBInternalMsg& internal_msg);
    Timer* GetTimer(uint64 timer_id);
    bool ReleaseTimer(uint32 timer_id);

private:
    index   _index;
    uint32  _cur_max_id;
};


