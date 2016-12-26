#include "Player.h"
#include "Animation.h"
#include "Device.h"
#include "Game.h"
#include "StageManager.h"

const Vector START_POS = Vector( 0.0, 3.0, 0.0 );
const Vector START_DIR = Vector( 1.0, 0.0, 0.0 );

const double STATIC_FRICTION = 0.2;//�Ö��C��
const double DYNAMIC_FRICTION = 0.1;//�����C��
const double STATIC_FRICTION_RANGE = 0.01;//�Ö��C�͂̋���

const double MIN_RUN_SPEED = 0.1;
const double MIN_HOVER_SPEED = 0.8;
const double MIN_TURBO_SPEED = 1.6;
const double MAX_SPEED = 2.0;

const double GRAVITY_FORCE = ( 9.8 / 60 ) / 25;
const double JUMP_POWER = 1;

Player::Player( ) {
	_pos = START_POS;
	_dir = START_DIR;
	_force = Vector( 0, 0, 0 );
	_gravity_vec = Vector( 0, -1, 0 );
	_state = STATE_WAIT;
	_animation = AnimationPtr( new Animation( ) );
	_fly_time = 0;
}

Player::~Player( ) {
}

void Player::update( ) {
	deviceController( );//���͏���
	move( );//�ړ��X�V����
	swicthStatus( );//��ԍX�V
	animationUpdate( );//�`��ɑ΂���A�j���[�V�����̍X�V
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

void Player::swicthStatus( ) {
	_state = STATE_WAIT;
	if ( _speed.getLength( ) >= MIN_RUN_SPEED ) {
		_state = STATE_RUN;
	}
	if ( _speed.getLength( ) >= MIN_HOVER_SPEED ) {
		_state = STATE_HOVER;
	}
	if ( _speed.getLength( ) >= MIN_TURBO_SPEED ) {
		_state = STATE_TURBO;
	}
}

void Player::animationUpdate( ) {
	if ( _state == STATE_WAIT ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_WAIT ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_WAIT ) );
		} else if ( _animation->isEndAnimation( ) ) {
			_animation->setAnimationTime( 0 );
		}
	}
	if ( _state == STATE_RUN ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_RUN ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_RUN ) );
		} else if ( _animation->isEndAnimation( ) ) {
			_animation->setAnimationTime( 0 );
		}
	}
	if ( _state == STATE_HOVER ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_HOVER ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_HOVER ) );
		} else if ( _animation->isEndAnimation( ) ) {
			_animation->setAnimationTime( 0 );
		}
	}
	if ( _state == STATE_TURBO ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_TURBO ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_TURBO ) );
		} else if ( _animation->isEndAnimation( ) ) {
			_animation->setAnimationTime( 0 );
		}
	}
	_animation->update( );
}

//�f�o�C�X�̓��͂ɑΉ���������
void Player::deviceController( ) {
	DevicePtr device = Device::getTask( );
	//�A�i���O�X�e�B�b�N��X�̕������擾
	double dir_x = device->getDirX( );
	//�����n�ʏ�ɗ����Ă���ꍇ
	bool on_ground = onGround( );
	if ( on_ground ) {
		Vector move_vec = Vector( dir_x, 0, 0 );//�ړ��x�N�g�������
		/*�����ŉ����x�̒���*/
		move_vec *= 0.001;
		addForce( move_vec );
		_fly_time = 0;
	} else {
		_fly_time++;
	}
	//�^�[�{
	bool on_turbo = ( device->getButton( ) & BUTTON_D ) > 0;//turbo���
	if ( on_turbo ) {
		Vector move_vec = _dir;//�ړ��x�N�g�������
		/*�����ŉ����x�̒���*/
		move_vec *= 2;
		addForce( move_vec );
	}
	//�d��
	bool on_jump = ( ( device->getButton( ) & BUTTON_C ) > 0 ) && on_ground;//�W�����v���
	if ( on_jump ) {
		Vector move_vec = Vector( 0, 1, 0 );//�ړ��x�N�g�������
		/*�����ŉ����x�̒���*/
		move_vec *= JUMP_POWER;
		addForce( move_vec );
	}
	//�������͏d�͔��]�R�}���h�������ꂽ�Ƃ�
	bool is_revers_gravity = ( device->getButton( ) & BUTTON_A ) > 0;//�d�͔��]���
	if ( is_revers_gravity ) {
		_gravity_vec *= -1;
	}
}

void Player::move( ) {
	/*���������͂�����*/

	//���C
	bool on_ground = onGround( );
	if ( on_ground ) {
		if ( _speed.getLength( ) < STATIC_FRICTION_RANGE ) {
			addForce( _speed * -STATIC_FRICTION );//�Ö��C
		} else {
			addForce( _speed * -DYNAMIC_FRICTION );//�����C
		}
		_speed = Vector( _speed.x, 0, _speed.z );
	}
	{//�d��
		Vector gravity_vec = _gravity_vec;
		gravity_vec *= GRAVITY_FORCE * _fly_time;
		addForce( gravity_vec );
	}
	_speed += _force;//��������
	if ( _speed.x > MAX_SPEED ) {
		_speed = Vector( MAX_SPEED, _speed.y, _speed.z );
	}
	
	_force = Vector( 0, 0, 0 );//�����x�����Z�b�g����
	//�ړ�����͂�����
	bool can_move = canMove( );
	while ( !can_move ) {
		_speed *= 0.99;
		can_move = canMove( );
	}
	_pos += _speed;
}

void Player::addForce( const Vector& force ) {
	_force += force;
}

bool Player::onGround( ) {
	GamePtr game = Game::getTask( );
	StageManagerPtr stage_mgr = game->getStageManager( );
	bool result = stage_mgr->isHitBlock( _pos + ( _gravity_vec * 0.1 ) );
	return result;
}

bool Player::canMove( ) {
	GamePtr game = Game::getTask( );
	StageManagerPtr stage_mgr = game->getStageManager( );
	bool result = !stage_mgr->isHitBlock( _pos + _speed );
	return result;
}

Vector Player::getSpeed( ) const {
	return _speed;
}