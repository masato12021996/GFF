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
		STATE_RUN,
		STATE_HOVER,
		STATE_TURBO,
		STATE_MAX
	};
public:
	void update( );
	Vector getPos( ) const;
	Vector getDir( ) const;
	STATE getState( ) const;
	AnimationPtr getAnimation( ) const;
private:
	void deviceController( );
	void move( );
	void animationUpdate( );
	void addForce( const Vector& vec );
	void swicthStatus( );
private:
	Vector _pos;	//à íu
	Vector _dir;	//å¸Ç´
	Vector _speed;	//ë¨ìx
	Vector _force;	//â¡ë¨ìx
	STATE _state;	//èÛë‘
	int _fly_time;	//ëÿãÛéûä‘
	AnimationPtr _animation;
};

