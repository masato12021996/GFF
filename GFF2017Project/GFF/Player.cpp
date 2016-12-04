#include "Player.h"
#include "Animation.h"
#include "Device.h"

const Vector START_POS = Vector( 0.0, 0.0, 0.0 );
const Vector START_DIR = Vector( 0.0, 0.0, 0.0 );

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
	//こっちは重力反転コマンドが押されたとき

}

void Player::move( ) {
	/*減速処理はこちら*/
	_force += _speed * -0.1;//一時原則
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