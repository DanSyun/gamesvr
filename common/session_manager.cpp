#include "session_manager.h"
#include "serialize.hpp"

size_t Session::GetByteSize()
{
    return sizeof(size_t) + base::ByteSize() + sizeof(_msg_head);
}

bool Session::Serialize(uint8* buf, size_t size, size_t& offset)
{
    if (!SerializeProtoBuf(buf, size, offset, *this))       return false;
    if (!SerializeBaseType(buf, size, offset, _msg_head))   return false;

    return true;
}

bool Session::UnSerialize(uint8* buf, size_t size, size_t& offset)
{
    if (!UnSerializeProtoBuf(buf, size, offset, *this))     return false;
    if (!UnSerializeBaseType(buf, size, offset, _msg_head)) return false;

    return true;
}

void Session::AddQueryKey(uint64 uid, uint32 key)
{
    // uid 和key 都必须去重
    bool find_uid = false;
    for (auto i = 0; i < base::db_processs_size(); ++i)
    {
        auto& process = *base::mutable_db_processs(i);
        if (process.uid() == uid)
        {
            find_uid = true;
            bool find_key = false;
            for (auto j = 0; j < process.keys_size(); ++j)
            {
                if (process.keys(j) == key)
                {
                    find_key = true;
                    break;
                }
            }
            if (!find_key)
            {
                process.add_keys(key);
            }
            break;
        }
    }
    if (!find_uid)
    {
        auto& process = *base::add_db_processs();
        process.set_uid(uid);
        process.add_keys(key);
    }
}

bool Session::HaveAllDBData()
{
    for (auto i = 0; i < base::db_processs_size(); ++i)
    {
        if (!base::db_processs(i).has_player())
            return false;
    }
    return true;
}

int Session::GetDBProcessIndex(uint64 uid)
{
    for (auto i = 0; i < base::db_processs_size(); ++i)
    {
        if (uid == base::db_processs(i).uid())
            return i;
    }
    return -1;
}

PBEvent& Session::AddEvent()
{
    return *base::add_events();
}

void Session::ClearEvents()
{
    base::clear_events();
}

bool SessionManager::Init(uint32 size, uint32 shmkey)
{
    if (!base::Init(size, shmkey)) return false;

    // session id 从0开始
    _cur_max_id = 0;

    // 重建索引
    for (auto iter = base::Begin(); iter != base::End(); ++iter)
    {
        uint32 session_id = iter->session_id();
        _index[session_id] = iter.GetPos();
        if (session_id > _cur_max_id)
            _cur_max_id = session_id;
    }

    return true;
}

Session* SessionManager::CreateSession(uint32 msg_id)
{
    int32 pos = base::Malloc();
    if (pos == -1)
        return NULL;

    // 生成session_id
    uint32 session_id;
    uint32 count = 0;
    while (true)
    {
        session_id = ++_cur_max_id;
        if (_index.find(session_id) == _index.end())
            break;
        if (++count >= 10)
            return NULL;
    }
    _cur_max_id = _cur_max_id & 0xfffffff;

    Session* psession = base::Get(pos);
    psession->set_session_id(session_id);
    psession->set_msg_id(msg_id);
    psession->set_update_time(GetTime());
    psession->set_session_state(EN_Session_On_Message);

    _index[session_id] = pos;

    return psession;
}

Session* SessionManager::GetSession(uint32 session_id)
{
    if (_index.find(session_id) == _index.end())
        return NULL;

    return base::Get(_index[session_id]);
}

bool SessionManager::ReleaseSession(uint32 session_id)
{
    if (_index.find(session_id) == _index.end())
        return false;

    Session* psession = base::Get(_index[session_id]);
    psession->Clear();

    base::Free(_index[session_id]);
    _index.erase(session_id);

    return true;
}

void SessionManager::CheckTimeOut()
{
    std::vector<uint32> ids;
    for (auto iter = base::Begin(); iter != base::End(); ++iter)
    {
        if (iter->update_time() + EN_Session_Timeout_Sec <= GetTime())
            ids.push_back(iter->session_id());
    }
    for (auto iter = ids.begin(); iter != ids.end(); ++iter)
    {
        ReleaseSession(*iter);
    }
}


