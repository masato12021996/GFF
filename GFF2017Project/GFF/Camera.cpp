#include "Camera.h"
#include "Game.h"
#include "Player.h"
#include "Application.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Model.h"

const Vector START_POS = Vector( 0, 10, -200 );

Camera::Camera( ) {
	_pos = START_POS;

	ApplicationPtr fw = Application::getInstance( );
	fw->setCameraUp( Vector( 0.0, 1.0, 0.0 ) );
	fw->setCamera( _target + _pos, _target );
}

Camera::~Camera( ) {
}

void Camera::update( ) {
	ApplicationPtr app = Application::getInstance( );
	app->setCamera( _target + _pos, _target );
}

Vector Camera::getCameraPos( ) {
	return _pos;
}

Vector Camera::getTargetPos( ) {
	return _target;
}

Vector Camera::getCameraOriginalPos( ) {
	return START_POS;
}

void Camera::setCameraPos( Vector pos ) {
	_pos = pos;
}

void Camera::setTargetPos( Vector target ) {
	_target = target;
}