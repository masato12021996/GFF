#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>

PTR( Game );

class Game : public Task {
public:
	static GamePtr getTask( );
	static std::string getTag( ){ return "GAME"; }
public:
	Game( );
	virtual ~Game( );
public:
	void update( );
};