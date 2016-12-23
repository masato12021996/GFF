#include "Player.h"
#include "Animation.h"
#include "Device.h"
#include "Game.h"
#include "StageManager.h"

const Vector START_POS = Vector( 0.0, 0.0, 0.0 );
const Vector START_DIR = Vector( 1.0, 0.0, 0.0 );

const double STATIC_FRICTION = 0.2;//静摩擦力
const double DYNAMIC_FRICTION = 0.1;//動摩擦力
const double STATIC_FRICTION_RANGE = 0.01;//静摩擦力の境目

const double MIN_RUN_SPEED = 0.1;
const double MIN_HOVER_SPEED = 0.8;
const double MIN_TURBO_SPEED = 1.6;
const double MAX_SPEED = 2.0;

const double GRAVITY_FORCE = ( 9.8 / 60 ) / 100;
const double JUMP_POWER = 1;

Player::Player( ) {
	_pos = START_POS;
	_dir = START_DIR;
	_force = Vector( 0, 0, 0 );
	_state = STATE_WAIT;
	_animation = AnimationPtr( new Animation( ) );
	_fly_time = 0;
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

//デバイスの入力に対応した処理
void Player::deviceController( ) {
	DevicePtr device = Device::getTask( );
	//アナログスティックのXの方向を取得
	double dir_x = device->getDirX( );
	//もし地面上に立っている場合
	bool on_ground = onGround( );
	if ( on_ground ) {
		Vector move_vec = Vector( dir_x, 0, 0 );//移動ベクトルを取る
		/*ここで加速度の調整*/
		move_vec *= 0.001;
		addForce( move_vec );
		_fly_time = 0;
	} else {
		_fly_time++;
	}
	//ターボ
	bool on_turbo = ( device->getButton( ) & BUTTON_D ) > 0;//turbo状態
	if ( on_turbo ) {
		Vector move_vec = _dir;//移動ベクトルを取る
		/*ここで加速度の調整*/
		move_vec *= 2;
		addForce( move_vec );
	}
	//重力
	bool on_jump = ( device->getButton( ) & BUTTON_C ) > 0;//ジャンプ状態
	if ( on_jump && on_ground ) {
		Vector move_vec = Vector( 0, 1, 0 );//移動ベクトルを取る
		/*ここで加速度の調整*/
		move_vec *= JUMP_POWER;
		addForce( move_vec );
	}
	//こっちは重力反転コマンドが押されたとき

}

void Player::move( ) {
	/*減速処理はこちら*/

	//摩擦
	bool on_ground = onGround( );
	if ( on_ground ) {
		if ( _speed.getLength( ) < STATIC_FRICTION_RANGE ) {
			addForce( Vector( 1, 0, 0 ) * _speed.getLength( ) * -STATIC_FRICTION );//静摩擦
		} else {
			addForce( Vector( 1, 0, 0 ) * _speed.getLength( ) * -DYNAMIC_FRICTION );//動摩擦
		}
		addForce( Vector( 0, 1, 0 ) * _speed.y * 0.1 );//垂直抗力
	}
	{//重力
		Vector gravity_vec = Vector( 0, -1, 0 );
		gravity_vec *= GRAVITY_FORCE * _fly_time;
		addForce( gravity_vec );
	}
	_speed += _force;//加速する
	if ( _speed.x > MAX_SPEED ) {
		_speed = Vector( MAX_SPEED, _speed.y, _speed.z );
	}
	
	_force = Vector( 0, 0, 0 );//加速度をリセットする
	//移動判定はこちら
	bool can_move = canMove( );
	if ( !can_move ) {
		_pos -= _speed * 1.01;
	}
	_pos += _speed;
}

void Player::addForce( const Vector& force ) {
	_force += force;
}

bool Player::onGround( ) {
	GamePtr game = Game::getTask( );
	StageManagerPtr stage_mgr = game->getStageManager( );
	bool result = stage_mgr->isHitBlock( _pos + Vector( 0, -0.1, 0 ) );
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