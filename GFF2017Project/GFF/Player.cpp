#include "Player.h"
#include "Animation.h"

const Vector START_POS = Vector( 0.0, 0.0, 0.0 );
const Vector START_DIR = Vector( 0.0, 0.0, 0.0 );

const int WAIT_MAX = 600;


Player::Player( ) {
	_pos = START_POS;
	_dir = START_DIR;
	_state = STATE_WAIT;
	_wait_time = 0;
	_animation = AnimationPtr( new Animation( ) );
}


Player::~Player( ) {
}

void Player::update( ) {
	animationUpdate( );
}

Vector Player::getPos( ) const {
	return _pos;
}

Vector Player::getDir( ) const {
	return _dir;
}

Player::STATE Player::getState( ) const {
	return _state;
}

AnimationPtr Player::getAnimation( ) const {
	return _animation;
}

void Player::animationUpdate( ) {
	if ( _wait_time > WAIT_MAX ) {
		_wait_time = 0;
	}

	if ( _state == STATE_WAIT ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_WAIT ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_WAIT ) );
		} else {
			if( _animation->isEndAnimation( ) ) {
				_animation->setAnimationTime( 0 );
			}
		}
		_wait_time++;
	} else {
		_wait_time = 0;
	}

	_animation->update( );
}