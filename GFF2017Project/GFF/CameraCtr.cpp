#include "CameraCtr.h"
#include "Camera.h"
#include "Player.h"
#include "Game.h"

const Vector TARGET_KEEP_LENGHT = Vector( 10, 0, 0 );
const float CAMERA_TO_PLAYER_LENGHT_MAX = 50;
const float CLEAR_CAMERA_Z_DIFF = 150;
const float CLEAR_CAMERA_Z_FRAME_DIFF = 5;
const float CLEAR_CAMERA_TARGET_FRAME_DIFF = 1;
const float CAMERA_Z_ACCELE_MAX = 3;
const int CAMERA_CENTER_MULT = 5;


CameraCtr::CameraCtr( ) {
	_camera = CameraPtr( new Camera( ) );
	_before_player_speed = 0;
	_camera_z_buffer = 10;
	_game_end_camera = false;
}


CameraCtr::~CameraCtr( ) {

}

void CameraCtr::update( ) {
	GamePtr game = Game::getTask( ); 
	PlayerPtr player = game->getPlayer( );
	if ( player->isPlayerStoped( ) ) {
		gameEndCamera( );
	} else {
		accessPlayer( );
	}
	keepTargetLength( );
	_camera->update( );
}

bool CameraCtr::isEndAccess( ) {
	return _game_end_camera;
}

void CameraCtr::gameEndCamera( ) {
	Vector pos = _camera->getCameraPos( );
	Vector original_pos = _camera->getCameraOriginalPos( );

	if ( pos.z - original_pos.z > CLEAR_CAMERA_Z_DIFF ) {
		_game_end_camera = true;
		return;
	}
	GamePtr game = Game::getTask( ); 
	pos.z += CLEAR_CAMERA_Z_FRAME_DIFF;
	_camera->setCameraPos( pos );
	_camera_target_buffer += CLEAR_CAMERA_TARGET_FRAME_DIFF;
	if ( _camera_target_buffer >= TARGET_KEEP_LENGHT.x ) {
		_camera_target_buffer = TARGET_KEEP_LENGHT.x;
	}
}

void CameraCtr::accessPlayer( ) { 
	GamePtr game = Game::getTask( ); 
	PlayerPtr player = game->getPlayer( );
	Vector speed = player->getSpeed( );
	Vector camera_pos = _camera->getCameraPos( );
	
	_camera_z_buffer += ( _before_player_speed -  speed.x ) * 70;
	if ( _camera_z_buffer > 0 && speed.getLength( ) > 1 ) {
		_camera_z_buffer = 0;
	}
	Vector camera_original_pos = _camera->getCameraOriginalPos( );
	if ( ( camera_original_pos - camera_pos ).getLength( ) > CAMERA_TO_PLAYER_LENGHT_MAX &&
		 _camera_z_buffer < 0 ) {
		_camera_z_buffer = 0;
	}
	if ( camera_original_pos.z - camera_pos.z < 0 &&
		 _camera_z_buffer > 0 ) {
		_camera_z_buffer = 0;
	}
	double camera_move_z = _camera_z_buffer;
	if ( abs( _camera_z_buffer ) > CAMERA_Z_ACCELE_MAX ) {
		camera_move_z = CAMERA_Z_ACCELE_MAX * _camera_z_buffer / abs( _camera_z_buffer );
	}
	_camera_z_buffer -= camera_move_z;
	camera_pos += Vector ( 0, 0, camera_move_z );

	_camera->setCameraPos( camera_pos );
	_camera_target_buffer += camera_move_z / CAMERA_CENTER_MULT;
	_before_player_speed = speed.x;

}

void CameraCtr::keepTargetLength( ) {
	GamePtr game = Game::getTask( ); 
	PlayerPtr player = game->getPlayer( );
	Vector player_pos = player->getPos( );
	Vector target = player_pos + TARGET_KEEP_LENGHT;
	target.x -= _camera_target_buffer;
	_camera->setTargetPos( target );
}

CameraPtr CameraCtr::getCamera( ) {
	return _camera;
}