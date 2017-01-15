#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( StageManager );
PTR( StageBlock );
PTR( Debri );
PTR( Timer );

class StageManager {
public:
	static const int STAGE_MAX_WIDTH = 1000;
	static const int STAGE_MAX_HEIGHT = 100;
public:
	StageManager( );
	virtual ~StageManager( );
public:
	StageBlockPtr getStageBlock( int idx );
	DebriPtr getDebri( int idx );
	double getStageBlockWidth( );
	double getStageBlockHeight( );
	int getTimeCount( );
	void timerStart( );
	bool isTimeLimit( );
	bool isTimerStart( );
	void setStageWidth( int width );
	void setStageHeight( int height );
	int getMaxStageObjNum( );
	double cross ( Vector a, Vector b );
	Vector raycastBlock( Vector origin_pos, Vector dir );
	void addStageBlock( Vector pos, int i );
	void addDebri( Vector pos, int i );
private:
	std::array< StageBlockPtr, STAGE_MAX_WIDTH * STAGE_MAX_HEIGHT > _stage_block;
	std::array< DebriPtr, STAGE_MAX_WIDTH * STAGE_MAX_HEIGHT > _stage_debri;
	int _stage_obj_max;
	int _stage_width;
	int _stage_height;
	TimerPtr _timer;
};

