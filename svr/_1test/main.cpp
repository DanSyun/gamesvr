#include <iostream>
#include <string>
#include "proto_base.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "base.h"
#include <unistd.h>
#include "proto_log.hpp"
#include "data_common.h"

using namespace std;

int main(int argc, char* argv[])
{
    PBDBData promoter;
    PBDBData machine;
    Session session;

    srand(time(NULL));
    // 游戏礼品
    DBPrize prize;
    prize.set_prize_id(1);
    prize.set_rate(70);
    // 礼品价值
    uint64 arr_value[10] = {0, 240, 10, 20, 5, 2, 1};
    // 单次消耗
    uint64 cost = 10;
    // 玩家
    PBDBData player;
    DataCommon::PlayerAddGold(&session, player, EN_Acc_Charge, 0, 10);
    promoter.mutable_p_base_info()->set_total_charge(promoter.p_base_info().total_charge() + 10);
    // 抽奖
    DBMLipstickInfo info;
    DBLotteryAward* plottery = NULL;
    PBAward* paward = NULL;
    // 1
    plottery = info.add_lottery_awards();
    plottery->set_rate(5);
    paward = plottery->add_awards();
    paward->set_type(EN_Award_Prize);
    paward->set_id(1);
    paward->set_giving_num(1);
    // 2
    plottery = info.add_lottery_awards();
    plottery->set_rate(55);
    paward = plottery->add_awards();
    paward->set_type(EN_Award_Prize);
    paward->set_id(2);
    paward->set_giving_num(1);
    // 3
    plottery = info.add_lottery_awards();
    plottery->set_rate(105);
    paward = plottery->add_awards();
    paward->set_type(EN_Award_Prize);
    paward->set_id(3);
    paward->set_giving_num(1);
    // 4
    plottery = info.add_lottery_awards();
    plottery->set_rate(205);
    paward = plottery->add_awards();
    paward->set_type(EN_Award_Prize);
    paward->set_id(4);
    paward->set_giving_num(1);
    // 5
    plottery = info.add_lottery_awards();
    plottery->set_rate(355);
    paward = plottery->add_awards();
    paward->set_type(EN_Award_Prize);
    paward->set_id(5);
    paward->set_giving_num(1);
    // 6
    plottery = info.add_lottery_awards();
    plottery->set_rate(655);
    paward = plottery->add_awards();
    paward->set_type(EN_Award_Prize);
    paward->set_id(6);
    paward->set_giving_num(1);


    // 游戏
    for (auto i = 0; i < 10000; ++i)
    {
        uint64 prize_id = 1;
        // 充值
        uint64 charge = 10;
        DataCommon::PlayerAddGold(&session, player, EN_Acc_Charge, 0, charge);
        promoter.mutable_p_base_info()->set_total_charge(promoter.p_base_info().total_charge() + charge);
        // 投币
        DataCommon::PlayerAddBuyIn(&session, player, 10, 0);
        // 玩游戏
        DataCommon::PlayerSubBuyIn(&session, player, (ENAccountReason)0, cost, 0);
        machine.mutable_m_base_info()->set_total_income(machine.m_base_info().total_income() + cost);

        // 概率
        uint32 rate = DataCommon::GetPrizeRate(machine, cost, prize.rate(), arr_value[prize_id], DataCommon::GetPromoterChargeRate(promoter));
        uint32 rand_num = rand() % 100000;
        bool get_prize = rand_num < rate;

        float lottery_rate = 0;
        // 发奖
        if (get_prize)
        {
            DataCommon::PlayerAddPrize(NULL, player, EN_Acc_Lip_Win, prize_id, 1);
            player.mutable_gold_info()->set_total_prize_value(player.gold_info().total_prize_value() + arr_value[prize_id]);
            machine.mutable_m_base_info()->set_total_prize_value(machine.m_base_info().total_prize_value() + arr_value[prize_id]);
        }
        // 抽奖
        else
        {
            lottery_rate = DataCommon::GetLotteryRate(machine, prize.rate(), DataCommon::GetPromoterChargeRate(promoter));
            if (lottery_rate > 100)
            {
                CSRspLottery response;
                uint32 rand_num = rand() % 1000;
                for (auto i = 0; i < info.lottery_awards_size(); ++i)
                {
                    const DBLotteryAward& award = info.lottery_awards(i);
                    if (rand_num < award.rate())
                    {
                        response.mutable_awards()->CopyFrom(award.awards());
                        break;
                    }
                }
                if (response.awards_size() == 0)
                {
                    PBAward* paward = response.add_awards();
                    paward->set_type(EN_Award_None);
                }
                if (response.awards(0).type() != EN_Award_None)
                {
                    DataCommon::PlayerAddAwards(NULL, player, EN_Acc_Lip_Lottery, response.awards());
                    player.mutable_gold_info()->set_total_prize_value(player.gold_info().total_prize_value() + arr_value[response.awards(0).id()]);
                    machine.mutable_m_base_info()->set_total_prize_value(machine.m_base_info().total_prize_value() + arr_value[response.awards(0).id()]);
                }
            }
        }
        cout << rand_num << ", " << rate << ", " << lottery_rate << ", " << (float)player.gold_info().total_prize_value() / (float)player.gold_info().total_consume() << endl;
    }
}
