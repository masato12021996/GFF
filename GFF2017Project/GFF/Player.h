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
		STATE_MAX
	};
public:
	void update( );
	Vector getPos( ) const;
	Vector getDir( ) const;
	STATE getState( ) const;
	bool isReversal() const;
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
	Vector _pos;	//�ʒu
	Vector _dir;	//����
	Vector _speed;	//���x
	Vector _before_speed;
	Vector _force;	//�����x
	STATE _state;	//���
	int _fly_time;	//�؋󎞊�
	Vector _gravity_vec;//�d�͌���
	bool _is_jump;
	bool _is_fall;
	bool _is_land;
	bool _is_reversal;
	int _before_device_button;
	int _push_jump_buton;
	AnimationPtr _animation;
};

