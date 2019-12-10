#pragma once

#include "frame_server.h"

class GameServer: public FrameServer
{
private:
    virtual int InitGame();
    virtual int Reload();
    virtual void ExitGame();
    virtual void OnClientMessage(stConnectHead& head, const char* buf, uint32 buf_len);
    virtual void OnClientOffline(const stConnectHead& head, uint64 logic_index);
    virtual void OnServerMessage(stProxyHead& head, char* buf, uint32 buf_len);
    virtual void OnInternalMessage(char* buf, uint32 buf_len);
    virtual void OnRoutineCheck();

    int LoadConfig();
};

