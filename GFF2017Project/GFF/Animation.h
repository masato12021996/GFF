#pragma once
#include "smart_ptr.h"

PTR( Animation );

class Animation {
public:
enum MOTION {
	MOTION_PLAYER_WAIT,	     
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

