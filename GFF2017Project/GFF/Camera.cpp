#include "Camera.h"
#include "Game.h"
#include "Player.h"
#include "Application.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Model.h"

const Vector START_POS = Vector( 0, 10, -200 );
const Vector TARGET_KEEP_LENGHT = Vector( 10, 0, 0 );

CameraPtr Camera::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< Camera >( fw->getTask( getTag( ) ) );
}

Camera::Camera( ) {
	_pos = START_POS;
	_before_mouse_pos = _pos;

	ApplicationPtr fw = Application::getInstance( );
	fw->setCameraUp( Vector( 0.0, 1.0, 0.0 ) );
	fw->setCamera( _target + _pos, _target );
}

Camera::~Camera( ) {
}

void Camera::update( ) {
	keepTargetLength( );
}

void Camera::keepTargetLength( ) {
	GamePtr game = Game::getTask( ); 
	PlayerPtr player = game->getPlayer( );
	Vector player_pos = player->getPos( );
	_target = player_pos + TARGET_KEEP_LENGHT;
	ApplicationPtr app = Application::getInstance( );
	app->setCamera( _target + _pos, _target );
}