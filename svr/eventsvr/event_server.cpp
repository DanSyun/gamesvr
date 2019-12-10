#include <time.h>

#include "event_server.h"
#include "pb_event.pb.h"
#include "daemon.hpp"
#include "time.hpp"

int run_flag = 0;
enum ENRunFlag
{
    EN_Flag_Exit = 1,
    EN_Flag_Reload  = 2,
};
void SignalExit(int sig)
{
    run_flag |= EN_Flag_Exit;
}
void SignalReload(int sig)
{
    run_flag |= EN_Flag_Reload;
}

// 解析传入的参数
void EventServer::ParseArgs(int argc, char* argv[], char* conf, bool& daemon)
{
    int opt;
    while ((opt = getopt(argc, argv, "dc:")) != -1)
    {
        switch (opt)
        {
            case 'd': daemon = true; break;
            case 'c': strcpy(conf, optarg); break;
            default: break;
        }
    }
}

// 加载配置
bool EventServer::LoadConfig(const char* conf)
{
	if (!_conf.Init(conf))
        return false;

    return true;
}

// 初始化网络
bool EventServer::InitNetWork()
{
	struct sockaddr_in ser_addr;						// server address

	_lsfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
	if (_lsfd < 0)
        return false;

	memset(&ser_addr, 0, sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = htonl(INADDR_ANY);           //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
	ser_addr.sin_port = htons(_conf.port());  				//端口号，需要网络序转换

	int ret = bind(_lsfd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
	if(ret < 0)
        return false;

	return true;
}

bool EventServer::Connect()
{
    _connect_time = 0;
    _mongo_client = NULL;
    _mongo_db = NULL;
    _coll_map.clear();

    mongoc_init();
	_mongo_client = mongoc_client_new(_conf.mongo_url().c_str());
    if (!_mongo_client)
    {
        DisConnect();
        return false;
    }

    auto now = time(NULL);
    _mongo_db = mongoc_client_get_database(_mongo_client, (_conf.mongo_db_name() + GetYYYYMMDD(now)).c_str());
    if (!_mongo_db)
    {
        DisConnect();
        return false;
    }

    _connect_time = now;
    return true;
}

void EventServer::DisConnect()
{
    if (_mongo_client)
    {
        mongoc_client_destroy(_mongo_client);
        _mongo_client = NULL;
    }
    if (_mongo_db)
    {
        mongoc_database_destroy(_mongo_db);
        _mongo_db = NULL;
    }
    for (auto iter = _coll_map.begin(); iter != _coll_map.end(); ++iter)
    {
        mongoc_collection_destroy(iter->second);
    }
    _coll_map.clear();
}

// 初始化
int EventServer::Initialize(int argc, char* argv [])
{
    // set time
    UpdateTime();

	// 解析配置
    char conf[32] = {0};
    bool daemon = false;
    ParseArgs(argc, argv, conf, daemon);

    // only one process per config name can run at a time
    g_pinstance = new RunOneInstance((string(conf) + string(".lock")).c_str());
    if (!g_pinstance->IsOneInstance()) return -100;

    if (daemon) Daemonize();

    // load conf
    if (!LoadConfig(conf)) return -1;

	// 网络初始化失败
	if (!InitNetWork()) return -2;

	// 初始化mongo
	if (!Connect()) return -3;

	// signal
    signal(SIGRTMIN, SignalExit);
    signal(SIGRTMIN+1, SignalReload);

    _addr_len = sizeof(_addr);

    return 0;
}

// 执行数据
void EventServer::Run()
{
    while (true)
    {
        if (!CheckRunFlag())
            break;

        CheckChangeDay();
    	DealNetWork();   //处理接收到的数据

        usleep(_conf.loop_sleep_usec());
    }
}

bool EventServer::CheckRunFlag()
{
    if (run_flag & EN_Flag_Exit)
    {
        return false;
    }
    return true;
}

void EventServer::CheckChangeDay()
{
    if (GetDiffDays(time(NULL), _connect_time) != 0)
    {
        DisConnect();
        Connect();
    }
}

// 处理接收消息
void EventServer::DealNetWork()
{
    uint16 count = 0;
	while(count++ < 16)
	{
		auto ret = recvfrom(_lsfd, _buf, sizeof(_buf), 0, (struct sockaddr*)&_addr, &_addr_len);
        if (ret > 0)
        {
    		if (!WriteMongoDB(_buf, ret))
    		{
                DisConnect();
                if (Connect())
                {
                    WriteMongoDB(_buf, ret);
                }
            }
        }
        else
        {
            break;
        }
	}
}

// 收取消息之后的处理
bool EventServer::WriteMongoDB(const char* buf, uint32 buf_len)
{
    PBEvent event;
    if (!event.ParseFromArray(buf, buf_len))
        return false;

    auto pcoll = GetMongoCollection(event.event_name());
    if (!pcoll)
        return false;

    bson_t data;
    bson_error_t error;

	//初始化数据
	bson_init(&data);

    for (int i = 0; i < event.num_fields_size(); ++i)
    {
        const auto& field = event.num_fields(i);
		BSON_APPEND_INT64(&data, field.key().c_str(), field.value());
    }
    for (int i = 0; i < event.str_fields_size(); ++i)
    {
        const auto& field = event.str_fields(i);
        BSON_APPEND_UTF8(&data, field.key().c_str(), field.value().c_str());
    }

    auto ret = mongoc_collection_insert(pcoll, MONGOC_INSERT_NONE, &data, NULL, &error);

    bson_destroy(&data);

    return ret;
}

mongoc_collection_t* EventServer::GetMongoCollection(const string & event_name)
{
    if (_coll_map.find(event_name) != _coll_map.end())
        return _coll_map[event_name];

    if (_mongo_db)
    {
        auto pcoll = mongoc_database_get_collection(_mongo_db, event_name.c_str());
        if (pcoll)
        {
            _coll_map[event_name] = pcoll;
        }
        return pcoll;
    }
    else
    {
        return NULL;
    }
}

