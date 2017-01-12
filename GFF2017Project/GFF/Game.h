#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>

PTR( Game );
PTR( Player );
PTR( StageManager );
PTR( CameraCtr );
PTR( Field );

class Game : public Task {
public:
	static GamePtr getTask( );
	static std::string getTag( ){ return "GAME"; }
public:
	Game( );
	virtual ~Game( );
public:
	void initialize( );
	void update( );
	PlayerPtr getPlayer( ) const;
	StageManagerPtr getStageManager( ) const;
	FieldPtr getField( ) const;
private:
	PlayerPtr _player;
	FieldPtr _field;
	StageManagerPtr _stage_manager;
	CameraCtrPtr _camera_ctr;
};