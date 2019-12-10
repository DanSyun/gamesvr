#include "ilogger.h"

using namespace log4cplus;

Logger g_info_logger = Logger::getInstance("info");
Logger g_error_logger = Logger::getInstance("error");
Logger g_debug_logger = Logger::getInstance("debug");
Logger g_proto_logger = Logger::getInstance("proto");

void InitRunLogger(std::string name)
{
    SharedAppenderPtr appender1(new RollingFileAppender((name + ".info").c_str(), 100 * 1024 * 1024, 3));
    appender1->setLayout(std::unique_ptr<Layout>(new PatternLayout("[%D]%m")));
    g_info_logger.addAppender(appender1);

    SharedAppenderPtr appender2(new RollingFileAppender((name + ".error").c_str(), 100 * 1024 * 1024, 3));
    appender2->setLayout(std::unique_ptr<Layout>(new PatternLayout("[%D]%m")));
    g_error_logger.addAppender(appender2);

    SharedAppenderPtr appender3(new RollingFileAppender((name + ".debug").c_str(), 100 * 1024 * 1024, 3));
    appender3->setLayout(std::unique_ptr<Layout>(new PatternLayout("[%D]%m")));
    g_debug_logger.addAppender(appender3);
}

void InitProtoLogger(const char* name)
{
    SharedAppenderPtr appender(new RollingFileAppender(name, 1024 * 1024 * 1024, 2, true, true));
    appender->setLayout(std::unique_ptr<Layout>(new PatternLayout("%m")));
    g_proto_logger.addAppender(appender);
}


