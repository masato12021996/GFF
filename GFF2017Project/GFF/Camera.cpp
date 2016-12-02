#include "Camera.h"
#include "Application.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Model.h"

const Vector START_POS = Vector( 0, 0, -50 );

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
	//rotateCameraforMouse( );
}

//�J�����̃}�E�X�ł̉�]����
void Camera::rotateCameraforMouse( ) {
	// �}�E�X�̍��E��Z����]������悤��
	MousePtr mouse = Mouse::getTask( );
	Vector mouse_pos = mouse->getPos( );
	Vector diff = _before_mouse_pos - mouse_pos;
	_before_mouse_pos = mouse_pos;
	if ( diff.x != 0 ) {
		double angle = ( 10 * PI / 180 ) * diff.normalize( ).x;
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), angle );
		_pos = mat.multiply( _pos );
	}
	if ( diff.y != 0 ) {
		Vector axis = _pos.cross( Vector( 0.0, 0.0, 1.0 ) );
		double angle = ( 5 * PI / 180 ) * diff.normalize( ).y;
		Matrix mat = Matrix::makeTransformRotation( axis, angle );
		_pos = mat.multiply( _pos );
	}
	int wheel = mouse->getWheelRotValue( );
	if ( wheel != 0 ) {
		double length = _pos.getLength( );
		length += wheel;
		if ( length < 10 ) {
			length = 10;
		}
		_pos = _pos.normalize( ) * length;
	}
	ApplicationPtr application = Application::getInstance( );
	application->setCamera( _target + _pos, _target );
}