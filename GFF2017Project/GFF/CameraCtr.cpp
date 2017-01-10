#include "CameraCtr.h"
#include "Camera.h"
#include "Player.h"
#include "Game.h"

const Vector TARGET_KEEP_LENGHT = Vector( 10, 0, 0 );
const float CAMERA_TO_PLAYER_LENGHT_MAX = 50;
const float CAMERA_Z_ACCELE_MAX = 3;
const int CAMERA_CENTER_MULT = 5;


CameraCtr::CameraCtr( ) {
	_camera = CameraPtr( new Camera( ) );
	_before_player_speed = 0;
	_camera_z_buffer = 10;
}


CameraCtr::~CameraCtr( ) {

}

void CameraCtr::update( ) {

	GamePtr game = Game::getTask( ); 
	PlayerPtr player = game->getPlayer( );
	Vector speed = player->getSpeed( );
	Vector camera_pos = _camera->getCameraPos( );
	
	_camera_z_buffer += ( _before_player_speed -  speed.getLength( ) ) * 70;
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
	_before_player_speed = speed.getLength( );


	keepTargetLength( );
	_camera->update( );
}

void CameraCtr::keepTargetLength( ) {
	GamePtr game = Game::getTask( ); 
	PlayerPtr player = game->getPlayer( );
	Vector player_pos = player->getPos( );
	Vector target = player_pos + TARGET_KEEP_LENGHT;
	target.x -= _camera_target_buffer;
	_camera->setTargetPos( target );
}