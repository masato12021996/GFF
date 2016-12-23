#include "StageManager.h"
#include "StageBlock.h"


const double STAGE_BLOCK_WIDTH = 24;
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

void StageManager::setStageWidth( int width ) {
	_stage_width = width;
}

void StageManager::setStageHeight( int height ) {
	_stage_height = height;
}

bool StageManager::isHitBlock( Vector pos ) {
	int x = ( ( int )pos.x + ( ( int )STAGE_BLOCK_WIDTH / 2 ) ) / ( int )STAGE_BLOCK_WIDTH;
	int y = ( int )pos.y / ( int )STAGE_BLOCK_HEIGHT;
	if ( x < 0 ) {
		x = 0;
	}
	if ( y < 0 ) {
		y = 0;
	}
	int idx = x + y * _stage_width;
	if( _stage_block[ idx ] ) {
		return true;
	}
	return false;
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

