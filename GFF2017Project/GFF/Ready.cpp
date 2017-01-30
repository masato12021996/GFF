#include "Ready.h"

const int FPS = 60;
const int MAX_TIME = FPS * 4;

Ready::Ready( ) {
	_time = MAX_TIME;
}

Ready::~Ready( ) {
}

void Ready::update( ) {
	_time--;
}

int Ready::getTimeCount( ) {
	return _time / FPS;
}

bool Ready::isEndReady( ) const {
	bool result = false;
	if ( _time <= 0 ) {
		result = true;
	}
	return result;
}