#pragma once

#include "object_manager.hpp"
#include "singleton.hpp"
#include "base.h"
#include "proto_base.h"

class Player: public PBPlayer
{
private:
    typedef PBPlayer base;
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

class PlayerManager:
    public ObjectManager<Player>,
    public Singleton<PlayerManager>
{
public:
    typedef ObjectManager<Player> base;
    typedef std::unordered_map<uint64, uint32> index;
public:
    bool Init(uint32 size, uint32 shmkey = 0);

    Player* CreatePlayer(uint64 uid);
    Player* GetPlayer(uint64 uid);
    bool ReleasePlayer(uint64 uid);

private:
    index _index;
};

