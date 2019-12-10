#pragma once

#include "object_manager.hpp"
#include "singleton.hpp"
#include "base.h"
#include "proto_base.h"

class Machine: public PBMachine
{
private:
    typedef PBMachine base;
public:
    size_t GetByteSize();
    bool Serialize(uint8* buf, size_t size, size_t& offset);
    bool UnSerialize(uint8* buf, size_t size, size_t& offset);

    stConnectHead& GetConnectHead() { return _connect_head; }
    void SetConnectHead(const stConnectHead& head) { _connect_head = head; }

    void RefreshTimer();
private:
    stConnectHead _connect_head;    // 实现消息推送
};

class MachineManager:
    public ObjectManager<Machine>,
    public Singleton<MachineManager>
{
public:
    typedef ObjectManager<Machine> base;
    typedef std::unordered_map<uint64, uint32> index;
public:
    bool Init(uint32 size, uint32 shmkey = 0);

    Machine* CreateMachine(uint64 machine_id);
    Machine* GetMachine(uint64 machine_id);
    bool ReleaseMachine(uint64 machine_id);

private:
    index _index;
};


