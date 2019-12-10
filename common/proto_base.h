#pragma once

#include "pb_cs_msg.pb.h"
#include "pb_ss_msg.pb.h"
#include "pb_internal_msg.pb.h"
#include "pb_db_data.pb.h"
#include "pb_mem_data.pb.h"

enum ENGlobalConst
{
    EN_Session_Timeout_Check_Interval_Sec   = 5,        // session 超时轮询间隔
    EN_Session_Timeout_Sec                  = 10,       // session 超时时间
    EN_Timer_Check_Interval_Msec            = 100,      // timer 轮询间隔
    EN_Max_DB_Redo_Time                     = 5,        // 重做最大次数
};

