#pragma once

#include <string>
#include <iostream>

#include "ilogger.h"
#include "singleton.hpp"
#include "protobuf/message.h"
#include "protobuf/text_format.h"
#include "protobuf/stubs/common.h"

static inline void NewProtoLogHandler(google::protobuf::LogLevel level, const char* filename, int line, const std::string& message);

class ProtoLog: public Singleton<ProtoLog>
{
public:
    void Init()
    {
        google::protobuf::SetLogHandler(NewProtoLogHandler);
    }
    void LogHandle(google::protobuf::LogLevel level, char const * filename, int line, std::string const & message)
    {
        LOG_ERROR("protobuf err, %s|%d|%s", filename, line, message.c_str());
    }
    void TraceProto(const google::protobuf::Message& message, uint32 msg_id = -1, const char* info = "")
    {
        std::string str;
        google::protobuf::TextFormat::PrintToString(message, &str);
        LOG_PROTO("------time=%u---msg_name=%s---msg_id=%d---info=%s------\n%s",
            GetTime(), message.GetTypeName().c_str(), msg_id, info, str.c_str());
    }
    void PrintProto(const google::protobuf::Message& message, uint32 msg_id = -1, const char* info = "")
    {
        std::string str;
        google::protobuf::TextFormat::PrintToString(message, &str);
        std::cout << "---"
                    << "---time=" << GetTimeMsec()
                    << "---msg_name=" << message.GetTypeName()
                    << "---msg_id=" << msg_id
                    << "---info=" << info
                    << "------" << std::endl;
        std::cout << str ;
    }
};

static inline void NewProtoLogHandler(google::protobuf::LogLevel level, const char* filename, int line, const std::string& message)
{
    ProtoLog::Instance()->LogHandle(level, filename, line, message);
}


