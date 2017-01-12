#include "StageManager.h"
#include "StageBlock.h"
#include "Timer.h"
#include "Field.h"
#include "Game.h"


const double STAGE_BLOCK_WIDTH = 24;
const double STAGE_BLOCK_HEIGHT = 2;
const int LIMIT_TIME = 30;


StageManager::StageManager( ) {
	_stage_block_max = 0;
	_timer = TimerPtr( new Timer( LIMIT_TIME ) );
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

int StageManager::getTimeCount( ) {
	return _timer->getTimeCount( );
}

void StageManager::timerStart( ) {
	_timer->timerStart( );
}

bool StageManager::isTimeLimit( ) {
	return _timer->isTimeLimit( );
}

bool StageManager::isTimerStart( ) {
	return _timer->isTimerStart( );
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

Vector StageManager::raycastBlock( Vector origin_pos, Vector dir ) {
	GamePtr app = Game::getTask( );
	Vector ray = dir - origin_pos;
	Vector multiple_normalize_ray = ray.normalize( );
	FieldPtr field = app->getField( );
	int multiple = 0;
	int idx = -1;
	Field::FieldContents field_block;
	while ( dir.getLength( ) > multiple_normalize_ray.getLength( ) ) {

		multiple_normalize_ray = ray.normalize( ) * multiple;
		multiple_normalize_ray += origin_pos;

		int x = ( int )( ( multiple_normalize_ray.x + ( STAGE_BLOCK_WIDTH / 2 ) ) / Field::FX_TO_MX );
		int y = ( int )( ( multiple_normalize_ray.y + ( STAGE_BLOCK_HEIGHT / 2 ) ) / Field::FY_TO_MY );
		if ( x < 0 ) {
			x = 0;
		}
		if ( y < 0 ) {
			y = 0;
		}
		field_block = field->getFieldBlock( x, y );
		if( field_block.x > 0 && field_block.y > 0 ) {
			break;
		}
		multiple++;
	}
	
	if ( field_block.x < 0 || field_block.y < 0 ) {
		return dir;
	}
	
	Vector block_center;
	block_center.x = field_block.x;
	block_center.y = field_block.y;

	//ブロックの左上
	Vector plane_point_a = Vector( block_center.x - ( STAGE_BLOCK_WIDTH / 2 ), block_center.y + ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
	//ブロックの右上
	Vector plane_point_b = Vector( block_center.x + ( STAGE_BLOCK_WIDTH / 2 ), block_center.y + ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
	//ブロックの左下
	Vector plane_point_c = Vector( block_center.x - ( STAGE_BLOCK_WIDTH / 2 ), block_center.y - ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
	//ブロックの右下
	Vector plane_point_d = Vector( block_center.x + ( STAGE_BLOCK_WIDTH / 2 ), block_center.y - ( STAGE_BLOCK_HEIGHT / 2 ), 0 );

	Vector block_origin_pos[ 4 ] = { plane_point_a, plane_point_a, plane_point_d, plane_point_d };
	Vector block_dir[ 4 ] = { plane_point_b, plane_point_c, plane_point_b, plane_point_c };
	
	Vector a1 = origin_pos;
	Vector a2 = dir;
	Vector final_out = dir;
	for ( int i = 0; i < 4; i++ ) {
		
		Vector b1 = block_origin_pos[ i ];
		Vector b2 = block_dir[ i ];
		if( ( cross(a2-a1, b1-a1) * cross(a2-a1, b2-a1) > 0.0001 ) ||
			( cross(b2-b1, a1-b1) * cross(b2-b1, a2-b1) > 0.0001 ) ) {
			continue;
		}
		Vector a = a2 - a1;
		Vector b = b2 - b1;
		double d1 = cross( b, b1 - a1 );
		double d2 = cross( b, a );
		Vector out = a1 + a * ( 1 / d2 ) * d1 ;
		bool near_for_origin = ( final_out - origin_pos ).getLength( ) > ( out - origin_pos ).getLength( );
		if ( near_for_origin ) {
			final_out = out;
		}
	}

	return final_out;
	
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

