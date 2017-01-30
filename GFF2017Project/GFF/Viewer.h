#pragma once
#include "Task.h"
#include "smart_ptr.h"
#include <string>

PTR( Viewer );

class Viewer: public Task {
public:
	enum EFFECT {
		EFFECT_GRAVITY,
		EFFECT_TURBO,
		EFFECT_WIND
	};
public:
	static ViewerPtr getTask( );
	static std::string getTag( ) { return "VIEWER"; }
public:
	Viewer( );
	virtual ~Viewer( );
public:
	void initialize( );
	void update( );
private:
	void drawPlayer( );
	void drawTitle( );
	void drawStageMdl( );
	void drawLimitTime( );
	void drawBackTower( );
	void drawBackGround( );
	void drawTurboCoolTime( );
private:
	double _back_tower_angle;
};