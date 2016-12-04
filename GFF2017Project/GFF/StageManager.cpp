#include "StageManager.h"
#include "StageBlock.h"


const double STAGE_BLOCK_WIDTH = 8;
const double STAGE_BLOCK_HEIGHT = 2.4;

StageManager::StageManager( ) {
	
}


StageManager::~StageManager( ) {
}

StageBlockPtr StageManager::getStageBlock( int idx ) {
	return _stage_block[ idx ];
}

void StageManager::addStageBlock( Vector pos ) {
	StageBlockPtr stage_block = StageBlockPtr( new StageBlock( pos ) );
	_stage_block.push_back( stage_block );
	_stage_block_max++;
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

