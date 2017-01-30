#include "Ready.h"
#include "Sound.h"
#include "Game.h"

const int FPS = 60;
const int MAX_TIME = FPS * 4;
const int SPEED = 2;

Ready::Ready( ) {
	_time = MAX_TIME;
}

Ready::~Ready( ) {
}

void Ready::update( ) {
	_time -= SPEED;

	if ( _time % FPS == 0 ) {
		GamePtr game = Game::getTask( );
		SoundPtr sound = Sound::getTask( );
		sound->playSE( game->getSoundStr( Game::SOUND_SE_COUNT ) );
	}
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