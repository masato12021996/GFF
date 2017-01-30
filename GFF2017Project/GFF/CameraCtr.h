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
	bool isEndAccess( );
	CameraPtr getCamera( );
private:
	void keepTargetLength( );
	void accessPlayer( );
	void gameEndCamera( );
private:
	CameraPtr _camera;
	bool _game_end_camera;
	double _before_player_speed;
	double _camera_z_buffer;
	double _camera_target_buffer;
};

