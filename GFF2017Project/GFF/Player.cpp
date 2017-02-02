#include "Player.h"
#include "Animation.h"
#include "Device.h"
#include "Game.h"
#include "StageManager.h"
#include "Viewer.h"
#include "Sound.h"
#include "Drawer.h"

const Vector START_POS = Vector( 0.0, 10.0, 0.0 );
const Vector START_DIR = Vector( 1.0, 0.0, 0.0 );

const double STATIC_FRICTION = 4;//�Ö��C��
const double DYNAMIC_FRICTION = 2;//�����C��
const double GAME_END_FRICTION = 20;//�Q�[���I�������C
const double STATIC_FRICTION_RANGE = 0.01;//�Ö��C�͂̋���
const double TURBO_COOL_TIME = 1.0;

const double MIN_RUN_SPEED = 0.1;
const double MIN_HOVER_SPEED = 0.8;
const double MIN_TURBO_SPEED = 1.6;
const double MAX_SPEED = 2.0;

const double GRAVITY_FORCE = ( 9.8 / 60 ) / 40;
const double JUMP_POWER = 1;

const double PLAYER_RANGE = 0.1;
const double PLAYER_SCALE = 2.5;

Player::Player( ) {
	reset( );
}

Player::~Player( ) {
}

void Player::reset( ) {
	_pos = START_POS;
	_dir = START_DIR;
	_force = Vector( 0, 0, 0 );
	_gravity_vec = Vector( 0, -1, 0 );
	_state = STATE_WAIT;
	_animation = AnimationPtr( new Animation( ) );
	_fly_time = 0;
	_is_jump = false;
	_is_fall = false;
	_is_reversal = false;
	_is_turbo = false;
	_can_turbo = true;
	_turbo_time = 0;
	_before_device_button = 0;
	_speed = Vector( 0, 0, 0 );
	_before_speed = _speed;
	_push_jump_buton = 0;
	_game_clear = false;
	_player_stoped = false;
	_end_clear_animation = false;
	_stop_controll = true;
	_is_hit_debri = false;
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

void Player::awake( ) {
	_stop_controll = false;
}

void Player::respawn( ) {
	reset( );
	awake( );
}

void Player::setEndMotion( ) {
	_game_clear = true;
	_stop_controll = true;
}

//���̊֐�����StageMnager��RayCast�֐��Ŏg���B
void Player::setHitDebri( bool hit ) {
	_is_hit_debri = hit;
}

bool Player::isPlayerStoped( ) const {
	return _player_stoped;
}

bool Player::isEndClearMotion( ) const {
	return _end_clear_animation;
}

Vector Player::getDir( ) const {
	return _dir;
}

int Player::getMaxTurboCoolTime( ) const {
	return TURBO_COOL_TIME;
}

int Player::getTurboCoolTime( ) const {
	return _turbo_time;
}

Player::STATE Player::getState( ) const {
	return _state;
}

AnimationPtr Player::getAnimation( ) const {
	return _animation;
}

void Player::swicthStatus( ) {
	_state = STATE_WAIT;
	if ( _speed.x >= MIN_RUN_SPEED ) {
		_state = STATE_RUN;
	}
	if ( _speed.x >= MIN_HOVER_SPEED ) {
		_state = STATE_HOVER;
	}
	if ( _speed.x >= MIN_TURBO_SPEED ) {
		_state = STATE_TURBO;
	}
	if ( _is_jump ) {
		_state = STATE_JUMP;
		
	}
	if ( _is_fall ) {
		_state = STATE_FALL;
	}
	if ( _is_land ) {
		_state = STATE_LAND;
	}
	if ( _is_reversal ) {
		_state = STATE_REVERSAL;
	}

	if ( _is_turbo ) {
		_state = STATE_TURBO;
	}
	if ( _is_hit_debri ) {
		_state = STATE_CRASH;
	}
	if ( _player_stoped && onGround( ) ) {
		_state = STATE_CLEAR;
		//�N���A��̉��o��x�����X�^�[�g�߂��֖߂�
		_pos.x = 10.0;
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
		} else if ( _animation->getAnimTime( ) > ( _animation->getEndAnimTime( ) - 12 ) ) {
			_animation->setAnimationTime( 0 );
			_is_turbo = false;
		}
	}
	if ( _state == STATE_JUMP ) {
		if ( _speed.x >= MIN_HOVER_SPEED ) {
			if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_HOVER_JUMP ) {
				_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_HOVER_JUMP ) );
				
			} else if ( _animation->isEndAnimation( ) ) {
				double time = _animation->getEndAnimTime();
				_animation->setAnimationTime(time);
			}
		} else {
			if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_JUMP ) {
				_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_JUMP ) );
				
			} else if ( _animation->isEndAnimation( ) ) {
				double time = _animation->getEndAnimTime();
				_animation->setAnimationTime(time);
			}
		}
	}
	if ( _state == STATE_FALL ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_FALL ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_FALL ) );
		}
		else if (_animation->isEndAnimation()) {
			double time = _animation->getEndAnimTime();
			_animation->setAnimationTime(time);
		}
	}
	if ( _state == STATE_LAND ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_LAND ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_LAND ) );
		}
		else if ( _animation->isEndAnimation( ) ) {
			double time = _animation->getEndAnimTime( );
			_animation->setAnimationTime( time );
			_is_land = false;
		}
	}
	if ( _state == STATE_REVERSAL ) {
		if (_animation->getMotion( ) != Animation::MOTION_PLAYER_REVERSAL ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_REVERSAL ) );
		}
		else if ( _animation->isEndAnimation( ) ) {
			double time = _animation->getEndAnimTime( );
			_animation->setAnimationTime( time );
			_is_reversal = false;
		}
	}
	if ( _state == STATE_CRASH ) {
		if (_animation->getMotion( ) != Animation::MOTION_PLAYER_HOVER_CRASH ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_HOVER_CRASH ) );
		}
		else if ( _animation->isEndAnimation( ) ) {
			double time = _animation->getEndAnimTime( );
			_animation->setAnimationTime( time );
			respawn( );
		}
	}
	if ( _state == STATE_CLEAR ) {
		if ( _animation->getMotion( ) != Animation::MOTION_PLAYER_GOAL ) {
			_animation = AnimationPtr( new Animation( Animation::MOTION_PLAYER_GOAL ) );
		} else if ( _animation->isEndAnimation( ) ) {
			double time = _animation->getEndAnimTime( );
			_animation->setAnimationTime( time );
			_end_clear_animation = true;
		}
	}
	_animation->update( );
}

//�f�o�C�X�̓��͂ɑΉ���������
void Player::deviceController( ) {
	if ( _stop_controll ) {
		return;
	}
	DevicePtr device = Device::getTask( );
	//�A�i���O�X�e�B�b�N��X�̕������擾
	double dir_x = device->getDirX( );
	//�����n�ʏ�ɗ����Ă���ꍇ
	bool on_ground = onGround( );
	if ( on_ground ) {
		//���s
		Vector move_vec = Vector( dir_x, 0, 0 );//�ړ��x�N�g�������
		move_vec *= 0.001;
		if ( dir_x > 0 ) {
			addForce( move_vec );
		}else if ( _speed.x > 0 ) {
			addForce( move_vec );
		}

		//�W�����v

		bool on_jump = ( device->getButton( ) & BUTTON_C ) > 0;//�W�����v���
		if ( on_jump ) {
			_is_jump = true;
			Vector move_vec = _gravity_vec * -1;//�ړ��x�N�g�������
			/*�����ŉ����x�̒���*/
			move_vec *= JUMP_POWER;
			addForce( move_vec );
			_push_jump_buton = 0;
		}
		else {
			_is_jump = false;
		}
	}
	else {
		//�󒆕��s
		Vector move_vec = Vector(dir_x, 0, 0);//�ړ��x�N�g�������
		move_vec *= 0.00001;
		addForce(move_vec);
		if ( !( ( device->getButton( ) & BUTTON_C ) > 0 ) && (_before_device_button & BUTTON_C) > 0 && _push_jump_buton < JUMP_POWER * 100 / 2 ) {
			Vector move_vec = _gravity_vec * (JUMP_POWER / 2 - _push_jump_buton / 100);
			addForce(move_vec);
		}
	}

	//�^�[�{
	if ( _turbo_time > TURBO_COOL_TIME * 60 ) {
		_can_turbo = true;
	}
	bool on_turbo = ( device->getButton( ) & BUTTON_D ) > 0;//turbo���
	if ( on_turbo && (_can_turbo || _is_land ) ) {
		_is_turbo = true;
		_can_turbo = false;
		_turbo_time = 0;
		Vector move_vec = Vector( 1, 0, 0 );//�ړ��x�N�g�������
		/*�����ŉ����x�̒���*/
		move_vec *= 2;
		addForce( move_vec );
		//�^�[�{�G�t�F�N�g
		DrawerPtr drawer = Drawer::getTask( );
		drawer->setEffect( Drawer::Effect( Viewer::EFFECT_TURBO, _pos + Vector( 15.0, 3.0 ), 1.0, Vector( 0.0, PI / 2 ) ) );

		//�^�[�{SE�Đ�
		GamePtr game = Game::getTask( );
		SoundPtr sound = Sound::getTask( );
		sound->playSE( game->getSoundStr( Game::SOUND_SE_TURBO ) );
	}

	//�������͏d�͔��]�R�}���h�������ꂽ�Ƃ�
	bool is_revers_gravity = ( ( device->getButton( ) & BUTTON_A ) > 0 ) && ( ( _before_device_button & BUTTON_A ) == 0 );//�d�͔��]���
	if ( is_revers_gravity ) {
		_gravity_vec *= -1;
		_pos.y += _gravity_vec.y * 3;
		_is_reversal = true;

		//�G�t�F�N�g
		DrawerPtr drawer = Drawer::getTask( );
		drawer->setEffect( Drawer::Effect( Viewer::EFFECT_TURBO, _pos ) );

		//�O���r�e�BSE�Đ�
		GamePtr game = Game::getTask( );
		SoundPtr sound = Sound::getTask( );
		sound->playSE( game->getSoundStr( Game::SOUND_SE_GRAVITY ) );
	}
	_before_device_button = device->getButton( );
	_turbo_time++;
}

void Player::move( ) {
	//�d��
	if ( !onGround( ) ) {
		_fly_time++;
	} else {
		_fly_time = 0;
	}
	Vector gravity_vec = _gravity_vec;
	gravity_vec *= GRAVITY_FORCE * _fly_time;
	addForce( gravity_vec );
	
	_speed += _force;//��������

	if ( _is_turbo ) {
		_speed.y = 0;
	}
	if ( _speed.y > 0 && _gravity_vec.y > 0 ||
		_speed.y < 0 && _gravity_vec.y < 0) {
		_is_fall = true;
	}
	_force = Vector( 0, 0, 0 );//�����x�����Z�b�g����
	//�Q�[���I�����̏���
	if ( _game_clear ) {
		_speed.x -= _speed.normalize( ).x * GRAVITY_FORCE * GAME_END_FRICTION;//�Ö��C
		if ( _speed.x <= 0 ) {
			_speed.x = 0;
			_player_stoped = true;
		}
	}

	/*���������͂�����*/
	//���C
	bool on_ground = onGround( );
	if ( on_ground ) {
		if ( _is_fall ) {
			_is_land = true;
		}
		_is_fall = false;
		if ( _speed.getLength( ) < STATIC_FRICTION_RANGE ) {
			_speed.x -= _speed.normalize( ).x * GRAVITY_FORCE * STATIC_FRICTION;//�Ö��C
		} else {
			_speed.x -= _speed.normalize( ).x * GRAVITY_FORCE * DYNAMIC_FRICTION;//�����C
		}
		if ( _gravity_vec.y < 0 ) {
			if (_speed.y < 0) {
				_speed = Vector( _speed.x, 0, _speed.z );
			}
		}
		else {
			if (_speed.y > 0) {
				_speed = Vector( _speed.x, 0, _speed.z );
			}
		}
		
	}

	if ( _speed.x > MAX_SPEED ) {
		_speed = Vector( MAX_SPEED, _speed.y, _speed.z );
	}
	if ( _speed.getLength( ) < 0.01 ) {
		_speed = Vector( 0, 0, 0 );
	}

	//�ړ�����͂�����
	bool can_move = canMove( );
	if ( !can_move ) {
		GamePtr game = Game::getTask( );
		StageManagerPtr stage_mgr = game->getStageManager( );
		Vector hit_pos = stage_mgr->raycastBlock( _pos, _speed + _speed.normalize( ) * PLAYER_RANGE );
		_pos = hit_pos + _speed.normalize( ) * -1 * PLAYER_RANGE;
		_speed = Vector( 0, 0, 0 );
	} else {
		_pos += _speed;
	}
	_before_speed = _speed;
}

void Player::addForce( const Vector& force ) {
	_force += force;
}

bool Player::onGround( ) {
	GamePtr game = Game::getTask( );
	StageManagerPtr stage_mgr = game->getStageManager( );
	Vector hit_pos = stage_mgr->raycastBlock( _pos, _gravity_vec.normalize( ) * PLAYER_RANGE * 1.5 );
	bool result =  !( _pos == hit_pos );
	return result;
}

bool Player::canMove( ) {
	GamePtr game = Game::getTask( );
	StageManagerPtr stage_mgr = game->getStageManager( );
	Vector hit_pos = stage_mgr->raycastBlock( _pos, _speed + _speed.normalize( ) * PLAYER_RANGE );
	bool result_foot =  ( _pos == hit_pos );
	Vector player_body_pos;
	if ( _gravity_vec.y > 0 ) {
		player_body_pos = Vector( _pos.x, _pos.y - PLAYER_SCALE, _pos.z );
	} else {
		player_body_pos = Vector( _pos.x, _pos.y + PLAYER_SCALE, _pos.z );
	} 
	hit_pos = stage_mgr->raycastBlock( player_body_pos, _speed + _speed.normalize( ) * PLAYER_RANGE );
	bool result_body = ( player_body_pos == hit_pos );
	return result_foot & result_body;
}

Vector Player::getSpeed( ) const {
	return _speed;
}

bool Player::isReversal( ) const {
	return _gravity_vec.y > 0;
}