#pragma once

#include "base.h"
#include "proto_base.h"
#include "session_manager.h"


enum ENAccountReason
{
    EN_Acc_Unknown          = 0,
    EN_Acc_Charge           = 1, // 充值
    EN_Acc_GM_Add           = 2, // GM加
    EN_Acc_GM_Sub           = 3, // GM扣
    EN_Acc_Lip_Play         = 4, // 口红机游戏
    EN_Acc_Lip_Retry        = 5, // 口红机续命
    EN_Acc_Lip_Win          = 6, // 口红机游戏
    EN_Acc_Lip_Lottery      = 7, // 口红机抽奖
    EN_Acc_Lip_Buy          = 8, // 口红机保底
    EN_Acc_Game_Refund      = 9, // 游戏离线退款
    EN_Acc_Buy_In           = 10, // 游戏投币
    EN_Acc_Clear_Buy_In     = 11, // 清除投币
    EN_Acc_Buy_In_Timeout   = 12, // 投币超时
};

class DataCommon
{
public:
    static uint64 PlayerAddBuyIn(Session* psession, PBDBData& player, uint64 num, uint64 machine_id);
    static uint64 PlayerSubBuyIn(Session* psession, PBDBData& player, ENAccountReason reason, uint64 cost, uint64 machine_id);
    static uint64 GetPlayerBuyIn(const PBDBData& player, uint64 machine_id);
    static uint64 GetPlayerBuyIn(const PBDBData& player);
    static void CheckPlayerBuyInTimeout(Session* psession, PBDBData& player);
    static bool PlayerClearBuyIn(Session* psession, PBDBData& player, uint64 machine_id);

    static void PlayerAddAwards(Session* psession, PBDBData& player, ENAccountReason reason, const google::protobuf::RepeatedPtrField<PBAward>& awards);
    static void PlayerAddAward(Session* psession, PBDBData& player, ENAccountReason reason, const PBAward& award);
    static void PlayerAddGold(Session* psession, PBDBData& player, ENAccountReason reason, uint64 giving_num, uint64 charge_num = 0);
    static uint64 PlayerSubGold(Session* psession, PBDBData& player, ENAccountReason reason, uint64 num);
    static void PlayerAddPrize(Session* psession, PBDBData& player, ENAccountReason reason, uint64 prize_id, uint32 num);
    static void PlayerSubPrize(Session* psession, PBDBData& player, ENAccountReason reason, uint64 prize_id, uint32 num);
    static void PlayerAddPoint(uint64 uid, ENAccountReason reason, uint32 num);

    static uint32 GetPlayerPrizeNum(const PBDBData& player);
    static uint32 GetPlayerPrizeNum(const PBDBData& player, uint64 prize_id);
    static float GetPlayerChargeRate(const PBDBData& player);
    static float GetPromoterChargeRate(const PBDBData & promoter);
    static uint32 GetPrizeRate(const PBDBData& machine, uint64 cost, uint32 prize_rate, uint64 prize_value, float p_charge_rate);
    static float GetLotteryRate(const PBDBData& machine, uint32 prize_rate, float p_charge_rate);

    static uint32 SumRepeated32(const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& timeouts, uint32 start = 0);
};

