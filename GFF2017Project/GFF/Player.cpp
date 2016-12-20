#include "Player.h"
#include "Animation.h"
#include "Device.h"

const Vector START_POS = Vector( 0.0, 0.0, 0.0 );
const Vector START_DIR = Vector( 1.0, 0.0, 0.0 );

const double STATIC_FRICTION = 0.2;//静摩擦力
const double DYNAMIC_FRICTION = 0.1;//動摩擦力
const double STATIC_FRICTION_RANGE = 0.01;//静摩擦力の境目

const double MIN_RUN_SPEED = 0.1;
const double MIN_HOVER_SPEED = 0.8;
const double MIN_TURBO_SPEED = 1.6;

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
	bool on_ground = true;
	if ( on_ground ) {
		Vector move_vec = Vector( dir_x, 0, 0 );//移動ベクトルを取る
		/*ここで加速度の調整*/
		move_vec *= 0.001;
		addForce( move_vec );
	}

	bool on_turbo = ( device->getButton( ) & BUTTON_D ) > 0;//turbo状態
	if ( on_turbo ) {
		Vector move_vec = _dir;//移動ベクトルを取る
		/*ここで加速度の調整*/
		move_vec *= 2;
		addForce( move_vec );
	}
	//こっちは重力反転コマンドが押されたとき

}

void Player::move( ) {
	/*減速処理はこちら*/
	if ( _speed.getLength( ) < STATIC_FRICTION_RANGE ) {
		_force += _speed * -STATIC_FRICTION;//静摩擦
	} else {
		_force += _speed * -DYNAMIC_FRICTION;//動摩擦
	}
	_speed += _force;//加速する

	_force = Vector( 0, 0, 0 );//加速度をリセットする
	//移動判定はこちら
	bool can_move = true;
	if ( can_move ) {
		_pos += _speed;
	}
}

void Player::addForce( const Vector& force ) {
	_force += force;
}