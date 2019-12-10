#include "jemalloc.h"
#include "proxy_server.h"

int main(int argc, char* argv[])
{
    auto psvr = new ProxyServer;
    int ret;
    if ((ret = psvr->Initialize(argc, argv)) == 0)
        psvr->Run();
    else
    {
        return ret;
    }
}

