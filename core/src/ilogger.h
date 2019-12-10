#pragma once

#include <string>
#include "logger.h"
#include "layout.h"
#include "appender.h"
#include "fileappender.h"
#include "consoleappender.h"
#include "loggingmacros.h"

using namespace log4cplus;

extern Logger g_info_logger;
extern Logger g_error_logger;
extern Logger g_debug_logger;
extern Logger g_proto_logger;

extern void InitRunLogger(std::string name);
extern void InitProtoLogger(const char* name);

#define LOG_INFO(fmt, args...)      LOG4CPLUS_INFO_FMT(g_info_logger, "[%s|%s|%d] " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#define LOG_ERROR(fmt, args...)     LOG4CPLUS_INFO_FMT(g_error_logger, "[%s|%s|%d] " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#define LOG_PROTO(fmt, args...)     LOG4CPLUS_INFO_FMT(g_proto_logger, fmt "\n", ##args)

#ifdef _DEBUG
#define LOG_DEBUG(fmt, args...)     LOG4CPLUS_INFO_FMT(g_debug_logger, "[%s|%s|%d] " fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define LOG_DEBUG(fmt, args...)
#endif

#ifdef _DEBUG
#define TRACE_PROTO(msg, cmd, info) ProtoLog::Instance()->TraceProto(msg, cmd, info);
#else
#define TRACE_PROTO(msg, cmd, info)
#endif
