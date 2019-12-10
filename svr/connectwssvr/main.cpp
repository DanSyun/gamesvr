#include "jemalloc.h"
#include "connectws_server.h"

int main(int argc, char* argv[])
{
    auto psvr = new ConnectWsServer;
    int ret;
    if ((ret = psvr->Initialize(argc, argv)) == 0)
        psvr->Run();
    else
    {
        return ret;
    }
}

