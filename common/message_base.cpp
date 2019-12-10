#include "message_base.h"
#include "proto_log.hpp"

FrameServer* MessageBase::_pserver = NULL;

//////////////////////////////////////////////////////////////////////////
// 解包方法
bool MessageBase::ParseMsgFromClient(const char* buf, uint32 buf_len, PBCSMsg& cs_msg)
{
    if (!cs_msg.ParseFromArray(buf, buf_len))
    {
        return false;
    }

    TRACE_PROTO(cs_msg, cs_msg.msg_union_case(), "GetMsgFromClient");
    return true;
}

bool MessageBase::ParseMsgFromServer(const char* buf, uint32 buf_len, PBSSMsg& ss_msg)
{
    if (!ss_msg.ParseFromArray(buf, buf_len))
    {
        return false;
    }

    TRACE_PROTO(ss_msg, ss_msg.msg_union_case(), "GetMsgFromServer");
    return true;
}

bool MessageBase::ParseMsgFromInternal(const char* buf, uint32 buf_len, PBInternalMsg& internal_msg)
{
    if (!internal_msg.ParseFromArray(buf, buf_len))
    {
        return false;
    }

    TRACE_PROTO(internal_msg, internal_msg.msg_union_case(), "GetMsgFromInternal");
    return true;
}

//////////////////////////////////////////////////////////////////////////
// 发包方法
void MessageBase::SendCommonErr(stConnectHead& head, ENResult result)
{
    PBCSMsg msg;
    msg.set_result(result);
    msg.mutable_cs_notify_common_err();
    SendMsgToClient(head, msg, true);
}

bool MessageBase::SendMsgToClient(Session* psession, bool close)
{
    psession->GetCSResponse().set_cli_seq(psession->GetCSMsg().cli_seq());
    return SendMsgToClient(psession->GetConnectHead(), psession->GetCSResponse(), close);
}

bool MessageBase::SendMsgToClient(stConnectHead& head, PBCSMsg& cs_msg, bool close)
{
    cs_msg.set_cmd(cs_msg.msg_union_case());
    if (!cs_msg.has_result()) cs_msg.set_result(EN_Result_OK);
    uint32 buf_len = cs_msg.ByteSize();
    if (buf_len > MAX_CS_PB_BUF_LEN) return false;

    head._msg_type = conn_msg_normal;
    head._need_close = close;

    static char buf[MAX_CS_PB_BUF_LEN];
    cs_msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    TRACE_PROTO(cs_msg, cs_msg.msg_union_case(), "SendMsgToClient");
    return _pserver->SendMessageToClient(head, buf, buf_len);
}

bool MessageBase::SendMsgToWeb(Session* psession, bool close)
{
    psession->GetSSResponse().set_uid(psession->GetWebMsg().uid());
    return SendMsgToWeb(psession->GetConnectHead(), psession->GetSSResponse(), close);
}

bool MessageBase::SendMsgToWeb(stConnectHead& head, PBSSMsg& ss_msg, bool close)
{
    uint32 buf_len = ss_msg.ByteSize();
    if (buf_len > MAX_CS_PB_BUF_LEN) return false;

    head._msg_type = conn_msg_normal;
    head._need_close = close;

    static char buf[MAX_CS_PB_BUF_LEN];
    ss_msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    TRACE_PROTO(ss_msg, ss_msg.msg_union_case(), "SendMsgToWeb");
    return _pserver->SendMessageToClient(head, buf, buf_len);
}

bool MessageBase::SendMsgToResponse(Session* psession)
{
    return SendMsgToResponse(psession->GetProxyHead(), psession->GetSSResponse());
}

bool MessageBase::SendMsgToResponse(stProxyHead& src_head, PBSSMsg& ss_msg)
{
    ss_msg.set_type(EN_SS_Response);
    uint32 buf_len = ss_msg.ByteSize();
    if (buf_len > MAX_SS_PB_BUF_LEN) return false;

    stProxyHead head;
    head._src_svr._type = _pserver->GetSvrType();
    head._src_svr._id   = _pserver->GetSvrID();
    head._des_svr._type = src_head._src_svr._type;
    head._des_svr._id   = src_head._src_svr._id;
    head._session_id    = src_head._session_id;
    head._route_type    = proxy_route_p2p;

    static char buf[MAX_SS_PB_BUF_LEN];
    ss_msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    TRACE_PROTO(ss_msg, ss_msg.msg_union_case(), "SendMsgToResponse");
    return _pserver->SendMessageToProxy(head, buf, buf_len, src_head._src_proxy_id);
}

bool MessageBase::SendMsgToInternal(const PBInternalMsg& internal_msg)
{
    uint32 buf_len = internal_msg.ByteSize();
    if (buf_len > MAX_SS_PB_BUF_LEN) return false;

    static char buf[MAX_SS_PB_BUF_LEN];
    internal_msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    TRACE_PROTO(internal_msg, internal_msg.msg_union_case(), "SendMsgToInternal");
    return _pserver->SendMessageToInternal(buf, buf_len);
}

bool MessageBase::SendMsgToEvent(const PBEvent& event)
{
    uint32 buf_len = event.ByteSize();
    if (buf_len > MAX_EVENT_BUF_LEN)
        return false;

    static char buf[MAX_EVENT_BUF_LEN];
    event.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    // TRACE_PROTO(event, 0, "SendMsgToEvent");
    return _pserver->SendMessageToEvent(buf, buf_len);
}

bool MessageBase::SendMsgToDBProxy(Session* psession, PBSSMsg& ss_msg)
{
    ss_msg.set_type(EN_SS_Request);
    uint32 buf_len = ss_msg.ByteSize();
    if (buf_len > MAX_SS_PB_BUF_LEN) return false;

    stProxyHead head;
    head._src_svr._type = _pserver->GetSvrType();
    head._src_svr._id   = _pserver->GetSvrID();
    head._des_svr._type = EN_1_DBProxy;
    head._session_id    = psession->session_id();
    head._route_type    = proxy_route_hash;
    head._hash_key      = ss_msg.uid();

    static char buf[MAX_SS_PB_BUF_LEN];
    ss_msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    psession->set_update_time(GetTime());

    TRACE_PROTO(ss_msg, ss_msg.msg_union_case(), "SendMsgToDBProxy");
    return _pserver->SendMessageToProxy(head, buf, buf_len);
}

bool MessageBase::SendMsgToPromoter(Session* psession, PBSSMsg& ss_msg)
{
    ss_msg.set_type(EN_SS_Request);
    uint32 buf_len = ss_msg.ByteSize();
    if (buf_len > MAX_SS_PB_BUF_LEN) return false;

    stProxyHead head;
    head._src_svr._type = _pserver->GetSvrType();
    head._src_svr._id   = _pserver->GetSvrID();
    head._des_svr._type = EN_6_Promoter;
    head._session_id    = psession->session_id();
    head._route_type    = proxy_route_hash;
    head._hash_key      = ss_msg.uid();

    static char buf[MAX_SS_PB_BUF_LEN];
    ss_msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    psession->set_update_time(GetTime());

    TRACE_PROTO(ss_msg, ss_msg.msg_union_case(), "SendMsgToPromotersvr");
    return _pserver->SendMessageToProxy(head, buf, buf_len);
}

bool MessageBase::SendMsgToServer(Session* psession, PBSSMsg& ss_msg, ENSvrType svr_type, uint32 svr_id)
{
    ss_msg.set_type(EN_SS_Request);
    uint32 buf_len = ss_msg.ByteSize();
    if (buf_len > MAX_SS_PB_BUF_LEN) return false;

    stProxyHead head;
    head._src_svr._type = _pserver->GetSvrType();
    head._src_svr._id   = _pserver->GetSvrID();
    if (psession)
    {
        head._session_id    = psession->session_id();
        psession->set_update_time(GetTime());
        psession->set_session_state(EN_Session_Wait_Response);
    }

    head._route_type    = proxy_route_p2p;
    head._des_svr._type = svr_type;
    head._des_svr._id   = svr_id;

    static char buf[MAX_SS_PB_BUF_LEN];
    ss_msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    TRACE_PROTO(ss_msg, ss_msg.msg_union_case(), "SendMsgToServer");
    return _pserver->SendMessageToProxy(head, buf, buf_len);
}

bool MessageBase::BroadcastToServer(PBSSMsg& ss_msg, ENSvrType type)
{
    ss_msg.set_type(EN_SS_Request);
    uint32 buf_len = ss_msg.ByteSize();
    if (buf_len > MAX_SS_PB_BUF_LEN) return false;

    stProxyHead head;
    head._src_svr._type = _pserver->GetSvrType();
    head._src_svr._id   = _pserver->GetSvrID();
    head._session_id    = 0;
    head._route_type    = proxy_route_broadcast;
    head._des_svr._type = type;

    static char buf[MAX_SS_PB_BUF_LEN];
    ss_msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    TRACE_PROTO(ss_msg, ss_msg.msg_union_case(), "BroadcastToServer");
    return _pserver->SendMessageToProxy(head, buf, buf_len);
}

//////////////////////////////////////////////////////////////////////////
// 定时器接口
int MessageBase::SetTimer(uint32 sec, const PBInternalMsg& inter)
{
    return SetTimerMsec(sec* 1000, inter);
}

int MessageBase::SetTimerMsec(uint32 msec, const PBInternalMsg& inter)
{
    return TimerManager::Instance()->CreateTimer(msec, inter);
}

void MessageBase::ClearTimer(int timer_id)
{
    if (timer_id < 0) return;
    TimerManager::Instance()->ReleaseTimer(timer_id);
}

//////////////////////////////////////////////////////////////////////////
// db 处理接口
template<class PB_TYPE>
static bool AddSaveData(SSReqSave& request, uint32 key, PB_TYPE& pb)
{
    PB_TYPE tmp;
    tmp.CopyFrom(pb);
    tmp.clear_head();
    uint32 len = tmp.ByteSize();
    if (len > MAX_KVDB_BUF_LEN)
        return false;

    // 序列化
    static char buf[MAX_KVDB_BUF_LEN];
    tmp.SerializeWithCachedSizesToArray((google::protobuf::uint8*)buf);

    // 本地版本号 + 1
    pb.mutable_head()->set_version(pb.head().version() + 1);

    PBKVDBData& db_data = *request.add_datas();
    db_data.set_key(key);
    db_data.set_value(buf, len);
    db_data.set_version(pb.head().version());
    db_data.set_update_time(GetTime());

    return true;
}

template<class PB_TYPE>
static bool ConstructDBData(PB_TYPE& pb, const PBKVDBData& data)
{
    // 必须先解析value
    if (!pb.ParseFromArray(data.value().c_str(), data.value().size()))
        return false;

    // 填充version
    pb.mutable_head()->set_version(data.version());

    return true;
}

bool MessageBase::SendQueryToDBProxy(Session * psession)
{
    bool get = false;
    for (auto i = 0; i < psession->db_processs_size(); ++i)
    {
        auto& process = psession->db_processs(i);

        // 只请求没有本地数据的uid
        if (process.has_player())
            continue;

        PBSSMsg msg;
        msg.set_uid(process.uid());
        msg.mutable_ss_req_query()->set_uid(process.uid());
        for (auto i = 0; i < process.keys_size(); ++i)
        {
            auto& data = *msg.mutable_ss_req_query()->add_datas();
            data.set_key(process.keys(i)); 
        }

        get = true;
        SendMsgToDBProxy(psession, msg);
    }
    if (get)
    {
        psession->set_session_state(EN_Session_Wait_Query);
    }

    return get;
}

bool MessageBase::SendSaveToDBProxy(Session * psession)
{
    if ((int)psession->process_index() >= psession->db_processs_size())
        return false;

    bool save = false;
    auto& process = psession->GetDBProcessInfo();
    PBDBData& player = psession->GetDBData();

    PBSSMsg msg;
    auto& request = *msg.mutable_ss_req_save();
    msg.set_uid(player.uid());
    request.set_uid(player.uid());

    // save 的必须是get 的key，有改动的(mutable) 才save
    for (auto i = 0; i < process.keys_size(); ++i)
    {
        auto key = process.keys(i);
        switch (key)
        {
            // 玩家数据
            case PBDBKey::kBaseInfo:
            {
                if (player.update_base_info() && AddSaveData(request, key, *player.mutable_base_info()))
                {
                    player.clear_update_base_info();
                    save = true;
                }
                break;
            }
            case PBDBKey::kGoldInfo:
            {
                if (player.update_gold_info() && AddSaveData(request, key, *player.mutable_gold_info()))
                {
                    player.clear_update_gold_info();
                    save = true;
                }
                break;
            }
            case PBDBKey::kPrizeInfo:
            {
                if (player.update_prize_info() && AddSaveData(request, key, *player.mutable_prize_info()))
                {
                    player.clear_update_prize_info();
                    save = true;
                }
                break;
            }
            case PBDBKey::kGameInfo:
            {
                if (player.update_game_info() && AddSaveData(request, key, *player.mutable_game_info()))
                {
                    player.clear_update_game_info();
                    save = true;
                }
                break;
            }

            // 街机数据
            case PBDBKey::kMBaseInfo:
            {
                if (player.update_m_base_info() && AddSaveData(request, key, *player.mutable_m_base_info()))
                {
                    player.clear_update_m_base_info();
                    save = true;
                }
                break;
            }
            case PBDBKey::kMLipstickInfo:
            {
                if (player.update_m_lipstick_info() && AddSaveData(request, key, *player.mutable_m_lipstick_info()))
                {
                    player.clear_update_m_lipstick_info();
                    save = true;
                }
                break;
            }

            // 代理数据
            case PBDBKey::kPBaseInfo:
            {
                if (player.update_p_base_info() && AddSaveData(request, key, *player.mutable_p_base_info()))
                {
                    player.clear_update_p_base_info();
                    save = true;
                }
                break;
            }

            // 全局数据
            case PBDBKey::kGLipPrizeInfo:
            {
                if (player.update_g_lip_prize_info() && AddSaveData(request, key, *player.mutable_g_lip_prize_info()))
                {
                    player.clear_update_g_lip_prize_info();
                    save = true;
                }
                break;
            }

            default:
            {
                LOG_ERROR("unknown dbkey: %u", key);
                return false;
            }
        }
    }
    if (save)
    {
        SendMsgToDBProxy(psession, msg);
        psession->set_session_state(EN_Session_Wait_Save);

        TRACE_PROTO(player, 0, "SaveDBData");
    }

    return save;
}

bool MessageBase::ParseDBData(PBDBData& player, const google::protobuf::RepeatedPtrField<PBKVDBData>& datas)
{
    for (auto i = 0; i < datas.size(); ++i)
    {
        const PBKVDBData& data = datas.Get(i);
        if (data.has_value())
        {
            switch (data.key())
            {
                // 玩家数据
                case PBDBKey::kBaseInfo:
                {
                    if (!ConstructDBData(*player.mutable_base_info(), data)) return false;
                    player.clear_update_base_info();
                    break;
                }
                case PBDBKey::kGoldInfo:
                {
                    if (!ConstructDBData(*player.mutable_gold_info(), data)) return false;
                    player.clear_update_gold_info();
                    break;
                }
                case PBDBKey::kPrizeInfo:
                {
                    if (!ConstructDBData(*player.mutable_prize_info(), data)) return false;
                    player.clear_update_prize_info();
                    break;
                }
                case PBDBKey::kGameInfo:
                {
                    if (!ConstructDBData(*player.mutable_game_info(), data)) return false;
                    player.clear_update_game_info();
                    break;
                }

                // 街机数据
                case PBDBKey::kMBaseInfo:
                {
                    if (!ConstructDBData(*player.mutable_m_base_info(), data)) return false;
                    player.clear_update_m_base_info();
                    break;
                }
                case PBDBKey::kMLipstickInfo:
                {
                    if (!ConstructDBData(*player.mutable_m_lipstick_info(), data)) return false;
                    player.clear_update_m_lipstick_info();
                    break;
                }

                // 代理数据
                case PBDBKey::kPBaseInfo:
                {
                    if (!ConstructDBData(*player.mutable_p_base_info(), data)) return false;
                    player.clear_update_p_base_info();
                    break;
                }

                // 全局数据
                case PBDBKey::kGLipPrizeInfo:
                {
                    if (!ConstructDBData(*player.mutable_g_lip_prize_info(), data)) return false;
                    player.clear_update_g_lip_prize_info();
                    break;
                }

                default: return false;
            }
        }
        else
        {
            InitDBData(data.key(), player);
        }
    }

    TRACE_PROTO(player, 0, "QueryDBData");
    return true;
}

void MessageBase::InitDBData(uint32 key, PBDBData& data)
{
    switch (key)
    {
        // 玩家数据
        case PBDBKey::kBaseInfo:
        {
            DBBaseInfo& info = *data.mutable_base_info();
            info.set_promoter_id(888888);
            break;
        }
        case PBDBKey::kGoldInfo:
        {
            DBGoldInfo& info = *data.mutable_gold_info();
            info.set_gold(0);
            break;
        }

        // 街机数据
        case PBDBKey::kMBaseInfo:
        {
            DBMBaseInfo& info = *data.mutable_m_base_info();
            info.set_game_type(EN_G_Type_Lipstick);
            info.set_total_prize_num(0);
            info.set_promoter_id(888888);
            data.clear_update_m_base_info();
            break;
        }
        case PBDBKey::kMLipstickInfo:
        {
            DBMLipstickInfo& info = *data.mutable_m_lipstick_info();

            info.add_round_timeouts(30);
            info.add_round_timeouts(30);
            info.add_round_timeouts(30);
            info.add_round_knives(8);
            info.add_round_knives(10);
            info.add_round_knives(12);

            info.set_start_timeout(120);
            info.set_retry_timeout(15);
            info.set_lottery_timeout(5);
            info.set_lottery_ani_timeout(99);
            info.set_buy_timeout(15);
            info.set_buy_ani_timeout(10);
            info.set_game_succ_timeout(15);

            info.set_retry_switch(true);
            info.set_lottery_switch(true);
            info.set_buy_switch(true);

            info.set_max_retry_time(5);

            data.clear_update_m_lipstick_info();
            break;
        }

        // 
        case PBDBKey::kGLipPrizeInfo:
        {
            data.mutable_g_lip_prize_info();
            data.clear_update_g_lip_prize_info();
            break;
        }

        default: break;
    }
}


