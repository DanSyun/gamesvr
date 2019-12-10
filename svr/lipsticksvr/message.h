#pragma once

#include "message_base.h"
#include "machine_manager.h"
#include "player_manager.h"

class Message: public MessageBase
{
public:
    static bool MessageFilter(stConnectHead& head, const char* buf, uint32 buf_len, PBCSMsg& msg);
    static void LogoutMachine(uint64 machine_id, bool notify_cli = false, ENLogoutReason reason = EN_Logout_Default, bool clear_db = true);
    static void LogoutMachine(Machine* pmachine, bool notify_cli = false, ENLogoutReason reason = EN_Logout_Default, bool clear_db = true);
    static void LogoutPlayer(uint64 player_id, bool notify_cli = false, ENLogoutReason reason = EN_Logout_Default, bool clear_db = true);
    static void LogoutPlayer(Player* pplayer, bool notify_cli = false, ENLogoutReason reason = EN_Logout_Default, bool clear_db = true);

private:
    static bool OnMachineLogin(stConnectHead& head, const PBCSMsg& msg);
    static bool OnPlayerLogin(stConnectHead& head, const PBCSMsg& msg);
    static bool OnLoginCheck(stConnectHead& head, const PBCSMsg& msg);
};

