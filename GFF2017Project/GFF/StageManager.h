#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"
#include <vector>

PTR( StageManager );
PTR( StageBlock );

class StageManager {
public:

public:
	StageManager( );
	virtual ~StageManager( );
public:
	StageBlockPtr getStageBlock( int idx );
	double getStageBlockWidth( );
	double getStageBlockHeight( );
	int getMaxStageBlockNum( );
	bool isHitBlock( Vector pos );
	void addStageBlock( Vector pos );
private:
	std::vector< StageBlockPtr > _stage_block;
	int _stage_block_max;
};

