#pragma once

#include <iostream>
#include <string>

#include "type_def.h"
#include "net_head.h"
#include "svr_config.h"
#include "ilogger.h"

#include "time.hpp"
#include "run_one_instance.hpp"

using namespace std;

extern uint64 UpdateTime();
extern uint64 GetTimeMsec();
extern uint32 GetTime();

extern RunOneInstance* g_pinstance;

