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
		STATE_JUMP,
		STATE_FALL,
		STATE_LAND,
		STATE_REVERSAL,
		STATE_CLEAR,
		STATE_CRASH,
		STATE_MAX
	};
public:
	void reset( );
	void update( );
	void awake( );
	void respawn( );
	void setEndMotion( );
	void setHitDebri( bool hit );
	Vector getPos( ) const;
	Vector getDir( ) const;
	STATE getState( ) const;
	bool isReversal( ) const;
	bool isEndClearMotion( ) const;
	bool isPlayerStoped( ) const;
	AnimationPtr getAnimation( ) const;
	Vector getSpeed( ) const;
	int getTurboCoolTime( ) const;
	int getMaxTurboCoolTime( ) const;
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
	Vector _before_speed;
	Vector _force;	//加速度
	STATE _state;	//状態
	int _fly_time;	//滞空時間
	Vector _gravity_vec;//重力向き
	bool _stop_controll;
	bool _is_turbo;
	bool _is_jump;
	bool _is_fall;
	bool _is_land;
	bool _is_reversal;
	bool _is_hit_debri;
	bool _can_turbo;
	bool _game_clear;
	bool _player_stoped;
	bool _end_clear_animation;
	int _turbo_time;
	int _before_device_button;
	int _push_jump_buton;
	AnimationPtr _animation;
};

