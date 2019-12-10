#include "jemalloc.h"
#include "game_server.h"

int main(int argc, char* argv[])
{
    auto psvr = new GameServer;
    int ret;
    if ((ret = psvr->Initialize(argc, argv)) == 0)
        psvr->Run();
    else
    {
        return ret;
    }
}

