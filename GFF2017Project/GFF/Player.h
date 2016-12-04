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
private:
	Vector _pos;	//�ʒu
	Vector _dir;	//����
	Vector _speed;	//���x
	Vector _force;	//�����x
	STATE _state;	//���
	AnimationPtr _animation;
};

