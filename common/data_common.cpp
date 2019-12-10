#include "data_common.h"
#include "config.h"
#include "system.hpp"

///////////////////////////////////////////////////////////////////////////////
// 玩家投币
// gold_info
uint64 DataCommon::PlayerAddBuyIn(Session* psession, PBDBData & player, uint64 num, uint64 machine_id)
{
    DBGoldInfo& info = *player.mutable_gold_info();
    if (num > info.gold()) num = info.gold();

    info.set_gold(info.gold() - num);
    for (auto i = 0; i < info.buy_ins_size(); ++i)
    {
        DBBuyIn& buy_in = *info.mutable_buy_ins(i);
        if (buy_in.machine_id() == machine_id)
        {
            buy_in.set_num(info.buy_ins(i).num() + num);
            buy_in.set_update_time(GetTime());
            return buy_in.num();
        }
    }

    DBBuyIn& buy_in = *info.add_buy_ins();
    buy_in.set_machine_id(machine_id);
    buy_in.set_num(num);
    buy_in.set_update_time(GetTime());

    // event
    PBEvent& event = psession->AddEvent();
    event.set_event_name("gold_account");
    PBNumField* p_num_field = NULL;
    p_num_field = event.add_num_fields();
    p_num_field->set_key("player_id");
    p_num_field->set_value(player.uid());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("num");
    p_num_field->set_value(0);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("after_gold");
    p_num_field->set_value(info.gold());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("after_buy_in");
    p_num_field->set_value(DataCommon::GetPlayerBuyIn(player));
    p_num_field = event.add_num_fields();
    p_num_field->set_key("reason");
    p_num_field->set_value(EN_Acc_Buy_In);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("time");
    p_num_field->set_value(GetTime());

    return buy_in.num();
}

///////////////////////////////////////////////////////////////////////////////
// 玩家游戏扣除投币
// base_info
// gold_info
uint64 DataCommon::PlayerSubBuyIn(Session* psession, PBDBData & player, ENAccountReason reason, uint64 cost, uint64 machine_id)
{
    uint64 ret = 0;
    DBGoldInfo& info = *player.mutable_gold_info();
    for (auto i = 0; i < info.buy_ins_size(); ++i)
    {
        DBBuyIn& buy_in = *info.mutable_buy_ins(i);
        if (buy_in.machine_id() == machine_id)
        {
            if (cost > buy_in.num()) cost = buy_in.num();
            if (cost == 0) return buy_in.num();
            info.set_total_consume(info.total_consume() + cost);
            buy_in.set_num(buy_in.num() - cost);
            buy_in.set_update_time(GetTime());
            ret = buy_in.num();
            break;
        }
    }

    // event
    PBEvent& event = psession->AddEvent();
    event.set_event_name("gold_account");
    PBNumField* p_num_field = NULL;
    p_num_field = event.add_num_fields();
    p_num_field->set_key("player_id");
    p_num_field->set_value(player.uid());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("num");
    p_num_field->set_value(- cost);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("after_gold");
    p_num_field->set_value(info.gold());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("after_buy_in");
    p_num_field->set_value(DataCommon::GetPlayerBuyIn(player));
    p_num_field = event.add_num_fields();
    p_num_field->set_key("reason");
    p_num_field->set_value(reason);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("time");
    p_num_field->set_value(GetTime());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("promoter_id");
    p_num_field->set_value(player.base_info().promoter_id());

    return ret;
}

///////////////////////////////////////////////////////////////////////////////
// 检查投币超时，服务器挂了可能出现，需要还给gold字段
// gold_info
void DataCommon::CheckPlayerBuyInTimeout(Session* psession, PBDBData & player)
{
    bool need_event = false;
    const DBGoldInfo& info = player.gold_info();
    for (auto i = 0; i < info.buy_ins_size();)
    {
        const DBBuyIn& buy_in = info.buy_ins(i);
        if (buy_in.update_time() + 600 <= GetTime())
        {
            need_event = true;
            player.mutable_gold_info()->set_gold(info.gold() + buy_in.num());
            player.mutable_gold_info()->mutable_buy_ins()->DeleteSubrange(i, 1);
        }
        else
        {
            ++i;
        }
    }

    if (need_event)
    {
        // event
        PBEvent& event = psession->AddEvent();
        event.set_event_name("gold_account");
        PBNumField* p_num_field = NULL;
        p_num_field = event.add_num_fields();
        p_num_field->set_key("player_id");
        p_num_field->set_value(player.uid());
        p_num_field = event.add_num_fields();
        p_num_field->set_key("num");
        p_num_field->set_value(0);
        p_num_field = event.add_num_fields();
        p_num_field->set_key("after_gold");
        p_num_field->set_value(info.gold());
        p_num_field = event.add_num_fields();
        p_num_field->set_key("after_buy_in");
        p_num_field->set_value(DataCommon::GetPlayerBuyIn(player));
        p_num_field = event.add_num_fields();
        p_num_field->set_key("reason");
        p_num_field->set_value(EN_Acc_Buy_In_Timeout);
        p_num_field = event.add_num_fields();
        p_num_field->set_key("time");
        p_num_field->set_value(GetTime());
        p_num_field = event.add_num_fields();
        p_num_field->set_key("promoter_id");
        p_num_field->set_value(player.base_info().promoter_id());
    }
}

///////////////////////////////////////////////////////////////////////////////
// 玩家离开游戏清除投币
// gold_info
bool DataCommon::PlayerClearBuyIn(Session* psession, PBDBData & player, uint64 machine_id)
{
    bool need_save = false;
    DBGoldInfo& info = *player.mutable_gold_info();
    for (auto i = 0; i < info.buy_ins_size(); ++i)
    {
        DBBuyIn& buy_in = *info.mutable_buy_ins(i);
        if (buy_in.machine_id() == machine_id)
        {
            info.set_gold(info.gold() + buy_in.num());
            info.mutable_buy_ins()->DeleteSubrange(i, 1);
            need_save = true;
            break;
        }
    }

    if (need_save)
    {
        // event
        PBEvent& event = psession->AddEvent();
        event.set_event_name("gold_account");
        PBNumField* p_num_field = NULL;
        p_num_field = event.add_num_fields();
        p_num_field->set_key("player_id");
        p_num_field->set_value(player.uid());
        p_num_field = event.add_num_fields();
        p_num_field->set_key("num");
        p_num_field->set_value(0);
        p_num_field = event.add_num_fields();
        p_num_field->set_key("after_gold");
        p_num_field->set_value(info.gold());
        p_num_field = event.add_num_fields();
        p_num_field->set_key("after_buy_in");
        p_num_field->set_value(DataCommon::GetPlayerBuyIn(player));
        p_num_field = event.add_num_fields();
        p_num_field->set_key("reason");
        p_num_field->set_value(EN_Acc_Clear_Buy_In);
        p_num_field = event.add_num_fields();
        p_num_field->set_key("time");
        p_num_field->set_value(GetTime());
    }

    return need_save;
}

///////////////////////////////////////////////////////////////////////////////
// 获取玩家投币
uint64 DataCommon::GetPlayerBuyIn(const PBDBData& player, uint64 machine_id)
{
    const DBGoldInfo& info = player.gold_info();
    for (auto i = 0; i < info.buy_ins_size(); ++i)
    {
        const DBBuyIn& buy_in = info.buy_ins(i);
        if (buy_in.machine_id() == machine_id)
        {
            return buy_in.num();
        }
    }
    return 0;
}
uint64 DataCommon::GetPlayerBuyIn(const PBDBData& player)
{
    uint64 sum = 0;
    const DBGoldInfo& info = player.gold_info();
    for (auto i = 0; i < info.buy_ins_size(); ++i)
    {
        const DBBuyIn& buy_in = info.buy_ins(i);
        sum += buy_in.num();
    }
    return sum;
}

///////////////////////////////////////////////////////////////////////////////
// 加奖品
// gold_info
// prize_info
void DataCommon::PlayerAddAwards(Session* psession, PBDBData& player, ENAccountReason reason, const google::protobuf::RepeatedPtrField<PBAward>& awards)
{
    for (auto i = 0; i < awards.size(); ++i)
    {
        PlayerAddAward(psession, player, reason, awards.Get(i));
    }
}
void DataCommon::PlayerAddAward(Session* psession, PBDBData& player, ENAccountReason reason, const PBAward& award)
{
    switch (award.type())
    {
        case EN_Award_Gold:
            PlayerAddGold(psession, player, reason, award.giving_num(), award.charge_num());
            break;

        case EN_Award_Prize:
            PlayerAddPrize(psession, player, reason, award.id(), award.giving_num());
            break;

        // 积分是web加
        case EN_Award_Point:
            player.mutable_gold_info();
            PlayerAddPoint(player.uid(), reason, award.giving_num());
            break;

        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
// 加金币
// base_info
// gold_info
void DataCommon::PlayerAddGold(Session* psession, PBDBData& player, ENAccountReason reason, uint64 giving_num, uint64 charge_num)
{
    // 口红机抽奖算充值
    if (reason == EN_Acc_Lip_Lottery)
    {
        std::swap(giving_num, charge_num);
    }

    uint64 total_num = giving_num + charge_num;
    if (total_num == 0) return;

    DBGoldInfo& info = *player.mutable_gold_info();
    info.set_gold(info.gold() + total_num);
    if (giving_num != 0)
    {
        info.set_total_giving(info.total_giving() + giving_num);
    }
    if (charge_num != 0)
    {
        info.set_total_charge(info.total_charge() + charge_num);
    }

    // 退款操作
    if (reason == EN_Acc_Game_Refund)
    {
        if (giving_num > info.total_consume()) giving_num = info.total_consume();
        if (giving_num > info.total_giving()) giving_num = info.total_giving();
        info.set_total_consume(info.total_consume() - giving_num);
        info.set_total_giving(info.total_giving() - giving_num);
    }

    // event
    PBEvent& event = psession->AddEvent();
    event.set_event_name("gold_account");
    PBNumField* p_num_field = NULL;
    p_num_field = event.add_num_fields();
    p_num_field->set_key("player_id");
    p_num_field->set_value(player.uid());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("num");
    p_num_field->set_value(total_num);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("charge_num");
    p_num_field->set_value(charge_num);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("giving_num");
    p_num_field->set_value(giving_num);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("after_gold");
    p_num_field->set_value(info.gold());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("after_buy_in");
    p_num_field->set_value(DataCommon::GetPlayerBuyIn(player));
    p_num_field = event.add_num_fields();
    p_num_field->set_key("reason");
    p_num_field->set_value(reason);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("time");
    p_num_field->set_value(GetTime());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("promoter_id");
    p_num_field->set_value(player.base_info().promoter_id());
}

///////////////////////////////////////////////////////////////////////////////
// 扣金币
// base_info
// gold_info
uint64 DataCommon::PlayerSubGold(Session* psession, PBDBData& player, ENAccountReason reason, uint64 num)
{
    if (num == 0) return player.gold_info().gold();

    DBGoldInfo& info = *player.mutable_gold_info();
    if (num > info.gold()) num = info.gold();

    info.set_gold(info.gold() - num);

    if (reason != EN_Acc_GM_Sub)
        info.set_total_consume(info.total_consume() + num);

    // event
    PBEvent& event = psession->AddEvent();
    event.set_event_name("gold_account");
    PBNumField* p_num_field = NULL;
    p_num_field = event.add_num_fields();
    p_num_field->set_key("player_id");
    p_num_field->set_value(player.uid());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("num");
    p_num_field->set_value(- num);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("after_gold");
    p_num_field->set_value(info.gold());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("after_buy_in");
    p_num_field->set_value(DataCommon::GetPlayerBuyIn(player));
    p_num_field = event.add_num_fields();
    p_num_field->set_key("reason");
    p_num_field->set_value(reason);
    p_num_field = event.add_num_fields();
    p_num_field->set_key("time");
    p_num_field->set_value(GetTime());
    p_num_field = event.add_num_fields();
    p_num_field->set_key("promoter_id");
    p_num_field->set_value(player.base_info().promoter_id());

    return info.gold();
}

///////////////////////////////////////////////////////////////////////////////
// 加礼品
void DataCommon::PlayerAddPrize(Session* psession, PBDBData& player, ENAccountReason reason, uint64 prize_id, uint32 num)
{
    DBPrizeInfo& info = *player.mutable_prize_info();
    bool find = false;
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        DBPrize& prize = *info.mutable_prizes(i);
        if (prize.prize_id() == prize_id)
        {
            find = true;
            prize.set_prize_num(prize.prize_num() + num);
            break;
        }
    }
    if (!find)
    {
        DBPrize& prize = *info.add_prizes();
        prize.set_prize_id(prize_id);
        prize.set_prize_num(num);
    }

    auto pprize = ConfigPrize::Instance()->GetPrize(prize_id);
    if (pprize)
    {
        DBGoldInfo& gold_info = *player.mutable_gold_info();
        gold_info.set_total_prize_value(gold_info.total_prize_value() + pprize->value);
    }

}

///////////////////////////////////////////////////////////////////////////////
// 扣礼品
void DataCommon::PlayerSubPrize(Session* psession, PBDBData& player, ENAccountReason reason, uint64 prize_id, uint32 num)
{
    DBPrizeInfo& info = *player.mutable_prize_info();
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        DBPrize& prize = *info.mutable_prizes(i);
        if (prize.prize_id() == prize_id)
        {
            if (num > prize.prize_num()) num = prize.prize_num();
            prize.set_prize_num(prize.prize_num() - num);
            if (prize.prize_num() == 0) info.mutable_prizes()->DeleteSubrange(i, 1);
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// 加积分
void DataCommon::PlayerAddPoint(uint64 uid, ENAccountReason reason, uint32 num)
{
    static char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "sh ../../../web_script/curl_point.sh %llu %u %d %u %d", uid, EN_Award_Point, 0, num, reason);
    dan::system(cmd);
}

///////////////////////////////////////////////////////////////////////////////
// 获取可领奖品数量
uint32 DataCommon::GetPlayerPrizeNum(const PBDBData & player)
{
    const DBPrizeInfo& info = player.prize_info();
    uint32 num = 0;
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        num += info.prizes(i).prize_num();
    }
    return num;
}

///////////////////////////////////////////////////////////////////////////////
// 获取指定id可领奖品数量
uint32 DataCommon::GetPlayerPrizeNum(const PBDBData& player, uint64 prize_id)
{
    const DBPrizeInfo& info = player.prize_info();
    uint32 num = 0;
    for (auto i = 0; i < info.prizes_size(); ++i)
    {
        if (info.prizes(i).prize_id() == prize_id)
            num += info.prizes(i).prize_num();
    }
    return num;
}

///////////////////////////////////////////////////////////////////////////////
// 充值占比
float DataCommon::GetPlayerChargeRate(const PBDBData & player)
{
    const DBGoldInfo& info = player.gold_info();

    if (info.total_charge() + info.total_giving() == 0)
        return 0.1;

    if (info.total_charge() == 0)
        return 0.1;

    return (float)info.total_charge() / ((float)info.total_charge() + (float)info.total_giving());
}
float DataCommon::GetPromoterChargeRate(const PBDBData & promoter)
{
    const DBPBaseInfo& info = promoter.p_base_info();

    if (info.total_charge() + info.total_giving() == 0)
        return 0.1;

    if (info.total_charge() == 0)
        return 0.1;

    return (float)info.total_charge() / ((float)info.total_charge() + (float)info.total_giving());
}

///////////////////////////////////////////////////////////////////////////////
// 计算中奖率
uint32 DataCommon::GetPrizeRate(const PBDBData& machine, uint64 cost, uint32 prize_rate, uint64 prize_value, float p_charge_rate)
{
    // 基础中奖率
    float basic_rate = (float)cost * p_charge_rate * ((float)prize_rate / 100) / (float)prize_value;

    // 中奖率调整系数
    const DBMBaseInfo& info = machine.m_base_info();
    float fix = ((float)info.total_income() * ((float)prize_rate / 100) * p_charge_rate - (float)info.total_prize_value()) / (float)prize_value;
    if (fix < -1) fix = -1;
    fix = 1 + fix * 0.9;

    // 最终中奖率
    float final_rate = basic_rate * fix * p_charge_rate;
    if (final_rate < 0.005) final_rate = 0.005;
    else if (final_rate > 1) final_rate = 1;

    return final_rate * 100000;
}

///////////////////////////////////////////////////////////////////////////////
// 计算中奖率
float DataCommon::GetLotteryRate(const PBDBData& machine, uint32 prize_rate, float p_charge_rate)
{
    const DBMBaseInfo& info = machine.m_base_info();
    return ((float)info.total_income() * ((float)prize_rate / 100) * p_charge_rate - (float)info.total_prize_value());
}

///////////////////////////////////////////////////////////////////////////////
// repeated 32求和
uint32 DataCommon::SumRepeated32(const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& repeateds, uint32 start)
{
    uint32 sum = 0;
    for (int32 i = start; i < repeateds.size(); ++i)
    {
        sum += repeateds.Get(i);
    }
    return sum;
}

