#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>
#include <map>

PTR( Game );
PTR( Title );
PTR( Ready );
PTR( Player );
PTR( StageManager );
PTR( CameraCtr );
PTR( Sound );
PTR( Field );

class Game : public Task {
public:
	enum STATE {
		STATE_TITLE,
		STATE_READY,
		STATE_PLAY,
		STATE_CLEAR,
	};
	enum SOUND {
		SOUND_BGM_AFTERGOAL,
		SOUND_BGM_GAME,
		SOUND_SE_COUNT,
		SOUND_SE_GRAVITY,
		SOUND_SE_TURBO
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
	ReadyPtr getReady( );
	PlayerPtr getPlayer( ) const;
	StageManagerPtr getStageManager( ) const;
	FieldPtr getField( ) const;
	CameraCtrPtr getCameraCtr( ) const;
	STATE getGameState( ) const;
	char* getSoundStr( SOUND sound_name );
private:
	TitlePtr _title;
	ReadyPtr _ready;
	STATE _state;
	PlayerPtr _player;
	FieldPtr _field;
	StageManagerPtr _stage_manager;
	CameraCtrPtr _camera_ctr;
	double _clear_line_x;

	std::map< SOUND, char* > _sound_str;
};