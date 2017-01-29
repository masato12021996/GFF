#pragma once
#include "smart_ptr.h"

PTR( Animation );

class Animation {
public:
enum MOTION {
	MOTION_PLAYER_NONE,
	MOTION_PLAYER_WAIT,
	MOTION_PLAYER_RUN,
	MOTION_PLAYER_HOVER,
	MOTION_PLAYER_TURBO,
	MOTION_PLAYER_REVERSAL,
	MOTION_PLAYER_READY,
	MOTION_PLAYER_LAND,
	MOTION_PLAYER_JUMP,
	MOTION_PLAYER_HOVER_JUMP,
	MOTION_PLAYER_HOVER_CRASH,
	MOTION_PLAYER_GOAL,
	MOTION_PLAYER_FALL,
	MOTION_PLAYER_CRASH,

	MOTION_MAX
};
public:
	Animation( MOTION motion = MOTION_MAX, double anim_speed = 1.0 );
	virtual ~Animation( );
public:
	void update( );
	bool isEndAnimation( ) const;
	int getMotion( ) const;
	double getAnimTime( ) const;
	double getEndAnimTime( ) const;
	void setAnimationTime( double anim_time );
	void setEndAnimationTime( double anim_time );
private:
	double _anim_time;
	double _anim_speed;
	double _max_anim_time;
	MOTION _motion;
};

