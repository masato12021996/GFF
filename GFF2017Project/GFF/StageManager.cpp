#include "StageManager.h"
#include "StageBlock.h"


const double STAGE_BLOCK_WIDTH = 24;
const double STAGE_BLOCK_HEIGHT = 2;

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
double StageManager::cross ( Vector a, Vector b ) {
	return a.x * b.y - a.y * b.x;
} 
Vector StageManager::raycastBlock( Vector origin_pos, Vector dir, bool is_trans_gravity ) {
	int x = ( int )( ( dir.x + ( STAGE_BLOCK_WIDTH / 2 ) ) / STAGE_BLOCK_WIDTH );
	int y = ( int )( ( dir.y + STAGE_BLOCK_HEIGHT ) / STAGE_BLOCK_HEIGHT );
	if ( x < 0 ) {
		x = 0;
	}
	if ( y < 0 ) {
		y = 0;
	}
	int idx = x + y * _stage_width;
	if( !_stage_block[ idx ] ) {
		return dir;
	}
	Vector block_center = _stage_block[ idx ]->getPos( );
	//ブロックの左上
	Vector plane_point_a = Vector( block_center.x - ( STAGE_BLOCK_WIDTH / 2 ), block_center.y + ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
	//ブロックの右上
	Vector plane_point_b = Vector( block_center.x + ( STAGE_BLOCK_WIDTH / 2 ), block_center.y + ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
	//ブロックの左下
	Vector plane_point_c = Vector( block_center.x - ( STAGE_BLOCK_WIDTH / 2 ), block_center.y - ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
	//ブロックの右下
	Vector plane_point_d = Vector( block_center.x + ( STAGE_BLOCK_WIDTH / 2 ), block_center.y - ( STAGE_BLOCK_HEIGHT / 2 ), 0 );

	Vector block_origin_pos;
	Vector block_dir;
	if ( is_trans_gravity ) {
		block_origin_pos = plane_point_c;
		block_dir = plane_point_d;
	} else {
		block_origin_pos = plane_point_a;
		block_dir = plane_point_b;
	}
	
	Vector a1 = origin_pos;
	Vector a2 = dir;
	Vector b1 = block_origin_pos;
	Vector b2 = block_dir;
	Vector a = a2 - a1;
	Vector b = b2 - b1;
	double d1 = cross( b, b1 - a1 );
	double d2 = cross( b, a );
	Vector out = a1 + a * ( 1 / d2 ) * d1 ;
	return out;
	
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

