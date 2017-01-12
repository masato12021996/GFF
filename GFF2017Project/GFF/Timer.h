#pragma once
#include "smart_ptr.h"

PTR( Timer );

class Timer {
public:
	Timer( int time_limit );
	virtual ~Timer( );
public:
	int getTimeCount( );
	void timerStart( );
	bool isTimeLimit( );
	bool isTimerStart( );
private:
	int _time_limit;
	int _start_time;
	bool _timer_start;
};

