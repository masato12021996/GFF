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
	Vector getSpeed( ) const;
private:
	void deviceController( );
	void move( );
	void animationUpdate( );
	void addForce( const Vector& vec );
	void swicthStatus( );
	bool onGround( );
	bool canMove( );
private:
	Vector _pos;	//位置
	Vector _dir;	//向き
	Vector _speed;	//速度
	Vector _force;	//加速度
	STATE _state;	//状態
	int _fly_time;	//滞空時間
	Vector _gravity_vec;//重力向き
	AnimationPtr _animation;
};

