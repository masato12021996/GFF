#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>

PTR( Game );
PTR( Player );

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
private:
	PlayerPtr _player;
};