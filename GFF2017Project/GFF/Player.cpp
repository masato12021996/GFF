#include "Player.h"
#include "Animation.h"
#include "Device.h"
#include "Game.h"
#include "StageManager.h"

const Vector START_POS = Vector( 0.0, 10.0, 0.0 );
const Vector START_DIR = Vector( 1.0, 0.0, 0.0 );

const double STATIC_FRICTION = 4;//静摩擦力
const double DYNAMIC_FRICTION = 2;//動摩擦力
const double STATIC_FRICTION_RANGE = 0.01;//静摩擦力の境目

const double MIN_RUN_SPEED = 0.1;
const double MIN_HOVER_SPEED = 0.8;
const double MIN_TURBO_SPEED = 1.6;
const double MAX_SPEED = 2.0;

const double GRAVITY_FORCE = ( 9.8 / 60 ) / 40;
const double JUMP_POWER = 1;

const double PLAYER_RANGE = 0.5;

Player::Player( ) {
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
	_before_device_button = 0;
	_speed = Vector(0, 0, 0);
	_before_speed = _speed;
	_push_jump_buton = 0;
}

Player::~Player( ) {
}

void Player::update( ) {
	deviceController( );//入力処理
	move( );//移動更新処理
	swicthStatus( );//状態更新
	animationUpdate( );//描画に対するアニメーションの更新
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
		if ( _is_fall ) {
			_state = STATE_FALL;
		}
	}
	if ( _is_land ) {
		_state = STATE_LAND;
	}
	if ( _is_reversal ) {
		_state = STATE_REVERSAL;
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
	if (_state == STATE_FALL) {
		if (_animation->getMotion() != Animation::MOTION_PLAYER_FALL) {
			_animation = AnimationPtr(new Animation(Animation::MOTION_PLAYER_FALL));
		}
		else if (_animation->isEndAnimation()) {
			double time = _animation->getEndAnimTime();
			_animation->setAnimationTime(time);
		}
	}
	if ( _state == STATE_LAND ) {
		if (_animation->getMotion() != Animation::MOTION_PLAYER_LAND) {
			_animation = AnimationPtr(new Animation(Animation::MOTION_PLAYER_LAND));
		}
		else if (_animation->isEndAnimation()) {
			double time = _animation->getEndAnimTime();
			_animation->setAnimationTime(time);
			_is_land = false;
		}
	}
	if (_state == STATE_REVERSAL) {
		if (_animation->getMotion() != Animation::MOTION_PLAYER_REVERSAL) {
			_animation = AnimationPtr(new Animation(Animation::MOTION_PLAYER_REVERSAL));
		}
		else if (_animation->isEndAnimation()) {
			double time = _animation->getEndAnimTime();
			_animation->setAnimationTime(time);
			_is_reversal = false;
		}
	}
	_animation->update( );
}

//デバイスの入力に対応した処理
void Player::deviceController( ) {
	DevicePtr device = Device::getTask( );
	//アナログスティックのXの方向を取得
	double dir_x = device->getDirX( );
	//もし地面上に立っている場合
	bool on_ground = onGround( );
	if ( on_ground ) {
		//歩行
		Vector move_vec = Vector( dir_x, 0, 0 );//移動ベクトルを取る
		move_vec *= 0.001;
		if (dir_x > 0) {
			addForce(move_vec);
		}else if (_speed.x > 0) {
			addForce(move_vec);
		}

		//ジャンプ
		bool on_jump = ( device->getButton( ) & BUTTON_C ) > 0;//ジャンプ状態
		if ( on_jump ) {
			_is_jump = true;
			Vector move_vec = _gravity_vec * -1;//移動ベクトルを取る
			/*ここで加速度の調整*/
			move_vec *= JUMP_POWER;
			addForce( move_vec );
			_push_jump_buton = 0;
		}
		else {
			_is_jump = false;
		}
	}
	else {
		//空中歩行
		Vector move_vec = Vector(dir_x, 0, 0);//移動ベクトルを取る
		move_vec *= 0.0001;
		addForce(move_vec);
		if ( !( ( device->getButton() & BUTTON_C ) > 0 ) && (_before_device_button & BUTTON_C) > 0 && _push_jump_buton < JUMP_POWER * 100 / 2 ) {
			Vector move_vec = _gravity_vec * (JUMP_POWER / 2 - _push_jump_buton / 100);
			addForce(move_vec);
		}
	}

	//ターボ
	bool on_turbo = ( device->getButton( ) & BUTTON_D ) > 0;//turbo状態
	if ( on_turbo ) {
		Vector move_vec = Vector( 1, 0, 0 );//移動ベクトルを取る
		/*ここで加速度の調整*/
		move_vec *= 2;
		addForce( move_vec );
	}

	//こっちは重力反転コマンドが押されたとき
	bool is_revers_gravity = ( ( device->getButton( ) & BUTTON_A ) > 0 ) && ( ( _before_device_button & BUTTON_A ) == 0 );//重力反転状態
	if ( is_revers_gravity ) {
		_gravity_vec *= -1;
		_is_reversal = true;
	}
	_before_device_button = device->getButton( );
}

void Player::move( ) {
	//重力
	if ( !onGround( ) ) {
		_fly_time++;
	} else {
		_fly_time = 0;
	}
	Vector gravity_vec = _gravity_vec;
	gravity_vec *= GRAVITY_FORCE * _fly_time;
	addForce( gravity_vec );
	
	_speed += _force;//加速する

	if ( _speed.y > 0 && _before_speed.y < 0 ||
		 _speed.y < 0 && _before_speed.y > 0 ) {
		_is_fall = true;
	}
	_force = Vector( 0, 0, 0 );//加速度をリセットする

	/*減速処理はこちら*/
	//摩擦
	bool on_ground = onGround( );
	if ( on_ground ) {
		if ( _is_fall ) {
			_is_land = true;
		}
		_is_fall = false;
		if ( _speed.getLength( ) < STATIC_FRICTION_RANGE ) {
			_speed.x -= _speed.normalize( ).x * GRAVITY_FORCE * STATIC_FRICTION;//静摩擦
		} else {
			_speed.x -= _speed.normalize( ).x * GRAVITY_FORCE * DYNAMIC_FRICTION;//動摩擦
		}
		if ( _gravity_vec.y < 0 ) {
			if (_speed.y < 0) {
				_speed = Vector(_speed.x, 0, _speed.z);
			}
		}
		else {
			if (_speed.y > 0) {
				_speed = Vector(_speed.x, 0, _speed.z);
			}
		}
		
	}

	if ( _speed.x > MAX_SPEED ) {
		_speed = Vector( MAX_SPEED, _speed.y, _speed.z );
	}
	if ( _speed.getLength( ) < 0.01 ) {
		_speed = Vector( 0, 0, 0 );
	}

	//移動判定はこちら
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
	bool result =  ( _pos == hit_pos );
	return result;
}

Vector Player::getSpeed( ) const {
	return _speed;
}

bool Player::isReversal( ) const {
	return _gravity_vec.y > 0;
}