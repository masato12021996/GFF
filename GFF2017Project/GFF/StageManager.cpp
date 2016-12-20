#include "StageManager.h"
#include "StageBlock.h"


const double STAGE_BLOCK_WIDTH = 8;
const double STAGE_BLOCK_HEIGHT = 2.4;

StageManager::StageManager( ) {
	_stage_block_max = 0;
}


StageManager::~StageManager( ) {
}

StageBlockPtr StageManager::getStageBlock( int idx ) {
	return _stage_block[ idx ];
}

void StageManager::addStageBlock( Vector pos, int idx ) {
	StageBlockPtr stage_block = StageBlockPtr( new StageBlock( pos ) );
	_stage_block[ idx ] = stage_block;
}

void StageManager::setMaxBlockNum( int num ) {
	_stage_block_max = num;
}

double StageManager::getStageBlockWidth( ) {
	return STAGE_BLOCK_WIDTH;
}

double StageManager::getStageBlockHeight( ) {
	return STAGE_BLOCK_HEIGHT;
}

int StageManager::getMaxStageBlockNum( ) {
	return _stage_block_max;
}

