#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( StageManager );
PTR( StageBlock );

class StageManager {
public:
	static const int STAGE_MAX_WIDTH = 1000;
	static const int STAGE_MAX_HEIGHT = 100;
public:
	StageManager( );
	virtual ~StageManager( );
public:
	StageBlockPtr getStageBlock( int idx );
	double getStageBlockWidth( );
	double getStageBlockHeight( );
	void setMaxBlockNum( int num );
	void setStageWidth( int width );
	void setStageHeight( int height );
	int getMaxStageBlockNum( );
	double cross ( Vector a, Vector b );
	Vector raycastBlock( Vector origin_pos, Vector dir, bool is_trans_gravity );
	void addStageBlock( Vector pos, int i );
private:
	std::array< StageBlockPtr, STAGE_MAX_WIDTH * STAGE_MAX_HEIGHT > _stage_block;
	int _stage_block_max;
	int _stage_width;
	int _stage_height;
};

