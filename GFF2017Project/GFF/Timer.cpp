#include "Timer.h"
#include <time.h>

Timer::Timer ( int time_limit ):
_time_limit( time_limit ) {
	_timer_start = false;
	_is_clear = false;
	_clear_time = 0;
}


Timer::~Timer( ) {

}

int Timer::getTimeCount( ) {
	int time = ( _time_limit * CLOCKS_PER_SEC ) - ( clock( ) - _start_time );
	if ( time < 0 ) {
		time = 0;
	}
	if ( _is_clear ) {
		time = _clear_time;
	}
	return time;
}

void Timer::timerStart( ) {
	_start_time = clock( );
	_timer_start = true;
}

bool Timer::isTimerStart( ) {
	return _timer_start;
}

bool Timer::isTimeLimit( ) {
	return ( ( clock( ) - _start_time ) / CLOCKS_PER_SEC ) >= _time_limit;
}

void Timer::clear( ) {
	_clear_time = ( _time_limit * CLOCKS_PER_SEC ) - ( clock( ) - _start_time );
	_is_clear = true;
}
