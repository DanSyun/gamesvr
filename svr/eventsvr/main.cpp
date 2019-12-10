#include "jemalloc.h"
#include "event_server.h"

int main(int argc, char* argv[])
{
	auto psvr = new EventServer;
    int ret;
    if ((ret = psvr->Initialize(argc, argv)) == 0)
        psvr->Run();
    else
    {
        return ret;
    }
}

