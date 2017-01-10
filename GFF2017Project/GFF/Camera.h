#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include "mathmatics.h"
#include <string>

PTR( Camera );

class Camera {
public:
	Camera( );
	virtual ~Camera( );
public:
	void update( );
	void setCameraPos( Vector pos );
	void setTargetPos( Vector target );
	Vector getCameraPos( );
	Vector getTargetPos( );
	Vector getCameraOriginalPos( );
private:
	Vector _pos;
	Vector _target; 
};

