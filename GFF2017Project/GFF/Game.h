#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>

PTR( Game );
PTR( Player );
PTR( StageManager );

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
private:
	PlayerPtr _player;
	StageManagerPtr _stage_manager;
};