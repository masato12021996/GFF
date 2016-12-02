#pragma once
#include "mathmatics.h"
#include "smart_ptr.h"

PTR( Player );
PTR( Animation );

class Player {
public:
	Player( );
	virtual ~Player( );
public:
	enum STATE {
		STATE_WAIT,
		STATE_MAX
	};
public:
	void initialize( );
	void update( );
	Vector getPos( ) const;
	Vector getDir( ) const;
	STATE getState( ) const;
	AnimationPtr getAnimation( ) const;
private:
	void animationUpdate( );
private:
	Vector _pos;
	Vector _dir;
	STATE _state;
	AnimationPtr _animation;
	int _wait_time;
};

