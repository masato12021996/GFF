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
	void clear( );
	bool isTimeLimit( );
	bool isTimerStart( );
private:
	int _time_limit;
	int _start_time;
	bool _is_clear;
	bool _timer_start;
	int _clear_time;
};

