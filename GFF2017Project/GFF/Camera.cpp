#include "Camera.h"
#include "Application.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Model.h"

const Vector START_TO_POS = Vector( 0, 50, 50 );

CameraPtr Camera::getTask( ) {
	ApplicationPtr fw = Application::getInstance( );
	return std::dynamic_pointer_cast< Camera >( fw->getTask( getTag( ) ) );
}

Camera::Camera( ) {
	_to_pos = START_TO_POS;
	_before_mouse_pos = _to_pos;

	ApplicationPtr fw = Application::getInstance( );
	fw->setCameraUp( Vector( 0.0, 1.0, 0.0 ) );
	fw->setCamera( _target + _to_pos, _target );
}

Camera::~Camera( ) {
}

void Camera::update( ) {
	// マウスの左右でZ軸回転をするように
	ApplicationPtr fw = Application::getInstance( );
	MousePtr mouse = Mouse::getTask( );
	KeyboardPtr keyboard = Keyboard::getTask( );

	Vector mouse_pos = mouse->getPos( );

	Vector diff = _before_mouse_pos - mouse_pos;
	_before_mouse_pos = mouse_pos;

	if ( diff.x != 0 ) {
		double angle = ( 10 * PI / 180 ) * diff.normalize( ).x;
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), angle );
		_to_pos = mat.multiply( _to_pos );
	}

	if ( diff.y != 0 ) {
		Vector axis = _to_pos.cross( Vector( 0.0, 0.0, 1.0 ) );
		double angle = ( 5 * PI / 180 ) * diff.normalize( ).y;
		Matrix mat = Matrix::makeTransformRotation( axis, angle );
		_to_pos = mat.multiply( _to_pos );
	}

	int wheel = mouse->getWheelRotValue( );
	if ( wheel != 0 ) {
		double length = _to_pos.getLength( );
		length += wheel;
		if ( length < 10 ) {
			length = 10;
		}
		_to_pos = _to_pos.normalize( ) * length;
	}

	fw->setCamera( _target + _to_pos, _target );
}