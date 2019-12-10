#include "player_manager.h"
#include "timer_manager.h"
#include "serialize.hpp"

size_t Player::GetByteSize()
{
    //pb len + pb bytes
    return sizeof(size_t) + base::ByteSize() + sizeof(_connect_head);
}

bool Player::Serialize(uint8* buf, size_t size, size_t& offset)
{
    if (!SerializeBaseType(buf, size, offset, _connect_head))   return false;
    if (!SerializeProtoBuf(buf, size, offset, *this))           return false;

    return true;
}

bool Player::UnSerialize(uint8* buf, size_t size, size_t& offset)
{
    if (!UnSerializeBaseType(buf, size, offset, _connect_head)) return false;
    if (!UnSerializeProtoBuf(buf, size, offset, *this))         return false;

    return true;
}

void Player::RefreshTimer()
{
    Timer* ptimer = TimerManager::Instance()->GetTimer(base::timer_id());
    if (ptimer)
    {
        uint32 left_msec = (ptimer->timeout_msec() > GetTimeMsec()) ? (ptimer->timeout_msec() - GetTimeMsec()) : 0;
        uint32 timer_id = TimerManager::Instance()->CreateTimerMsec(left_msec + 1000, ptimer->msg());
        TimerManager::Instance()->ReleaseTimer(base::timer_id());
        base::set_timer_id(timer_id);
    }
}

bool PlayerManager::Init(uint32 size, uint32 shmkey)
{
    if (!base::Init(size, shmkey)) return false;

    // 重建索引
    for (auto iter = base::Begin(); iter != base::End(); ++iter)
    {
        _index[iter->player_id()] = iter.GetPos();
    }

    return true;
}

Player* PlayerManager::CreatePlayer(uint64 uid)
{
    int32 pos = base::Malloc();
    if (pos == -1)
        return NULL;

    Player* pplayer = base::Get(pos);
    pplayer->set_player_id(uid);

    _index[uid] = pos;

    return pplayer;
}

Player* PlayerManager::GetPlayer(uint64 uid)
{
    if (_index.find(uid) == _index.end())
        return NULL;

    return base::Get(_index[uid]);
}

bool PlayerManager::ReleasePlayer(uint64 uid)
{
    if (_index.find(uid) == _index.end())
        return false;

    Player* pplayer = base::Get(_index[uid]);
    pplayer->Clear();

    base::Free(_index[uid]);
    _index.erase(uid);

    return true;
}


