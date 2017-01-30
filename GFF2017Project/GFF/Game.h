#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>

PTR( Game );
PTR( Title );
PTR( Player );
PTR( StageManager );
PTR( CameraCtr );
PTR( Field );

class Game : public Task {
public:
	enum STATE {
		STATE_TITLE,
		STATE_PLAY,
		STATE_CLEAR,
	};
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
	CameraCtrPtr getCameraCtr( ) const;
	STATE getGameState( ) const;
private:
	STATE _state;
	PlayerPtr _player;
	TitlePtr _title;
	FieldPtr _field;
	StageManagerPtr _stage_manager;
	CameraCtrPtr _camera_ctr;
	double _clear_line_x;
};