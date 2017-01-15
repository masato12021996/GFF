#pragma once
#include "smart_ptr.h"

PTR( CameraCtr );
PTR( Camera );

class CameraCtr {
public:
	CameraCtr( );
	virtual ~CameraCtr( );
public:
	void update( );
	CameraPtr getCamera( );
private:
	void keepTargetLength( );
private:
	CameraPtr _camera;
	double _before_player_speed;
	double _camera_z_buffer;
	double _camera_target_buffer;
};

