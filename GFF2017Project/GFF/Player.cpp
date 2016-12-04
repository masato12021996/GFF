#include "Player.h"
#include "Animation.h"
#include "Device.h"

const Vector START_POS = Vector( 0.0, 0.0, 0.0 );
const Vector START_DIR = Vector( 0.0, 0.0, -1.0 );

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

void Player::animationUpdate( ) {
	if ( _state == STATE_WAIT ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_WAIT ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_WAIT ) );
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
	_force += _speed * -0.1;//�ꎞ����
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