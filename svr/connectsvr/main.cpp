#include "jemalloc.h"
#include "connect_server.h"

int main(int argc, char* argv[])
{
    auto psvr = new ConnectServer;
    int ret;
    if ((ret = psvr->Initialize(argc, argv)) == 0)
        psvr->Run();
    else
    {
        return ret;
    }
}

