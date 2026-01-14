#pragma once

#include <time.h>

extern time_t curTime;
extern time_t runTime;

int sntp_sync_start(void);