#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include "mathmatics.h"
#include <string>

PTR( Camera );

class Camera : public Task {
public:
	static CameraPtr getTask( );
	static std::string getTag( ) { return "CAMERA"; }
public:
	Camera( );
	virtual ~Camera( );
public:
	void update( );
private:
	void keepTargetLength( );
	void rotateCameraforMouse( );
private:
	Vector _pos;
	Vector _target; 
	Vector _before_mouse_pos;
};

