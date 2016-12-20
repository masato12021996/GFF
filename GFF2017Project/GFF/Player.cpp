#include "Player.h"
#include "Animation.h"
#include "Device.h"

const Vector START_POS = Vector( 0.0, 0.0, 0.0 );
const Vector START_DIR = Vector( 0.0, 0.0, 0.0 );

const double STATIC_FRICTION = 0.2;//�Ö��C��
const double DYNAMIC_FRICTION = 0.1;//�����C��
const double STATIC_FRICTION_RANGE = 0.01;//�Ö��C�͂̋���

const double MIN_RUN_SPEED = 0.1;
const double MIN_HOVER_SPEED = 0.8;

Player::Player( ) {
	_pos = START_POS;
	_dir = START_DIR;
	_force = Vector( 0, 0, 0 );
	_state = STATE_WAIT;
	_animation = AnimationPtr( new Animation( ) );
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
	_animation->update( );
}

//�f�o�C�X�̓��͂ɑΉ���������
void Player::deviceController( ) {
	DevicePtr device = Device::getTask( );
	//�A�i���O�X�e�B�b�N��X�̕������擾
	double dir_x = device->getDirX( );
	//�����n�ʏ�ɗ����Ă���ꍇ
	bool on_ground = true;
	if ( on_ground ) {
		Vector move_vec = Vector( dir_x, 0, 0 );//�ړ��x�N�g�������
		/*�����ŉ����x�̒���*/
		move_vec *= 0.001;
		addForce( move_vec );
	}
	//�������͏d�͔��]�R�}���h�������ꂽ�Ƃ�

}

void Player::move( ) {
	/*���������͂�����*/
	if ( _speed.getLength( ) < STATIC_FRICTION_RANGE ) {
		_force += _speed * -STATIC_FRICTION;//�Ö��C
	} else {
		_force += _speed * -DYNAMIC_FRICTION;//�����C
	}
	_speed += _force;//��������

	_force = Vector( 0, 0, 0 );//�����x�����Z�b�g����
	//�ړ�����͂�����
	bool can_move = true;
	if ( can_move ) {
		_pos += _speed;
	}
}

void Player::addForce( const Vector& force ) {
	_force += force;
}