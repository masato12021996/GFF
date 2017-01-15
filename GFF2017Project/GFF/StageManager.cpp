#include "StageManager.h"
#include "StageBlock.h"
#include "Debri.h"
#include "Timer.h"
#include "Field.h"
#include "Game.h"


const double STAGE_BLOCK_WIDTH = 24;
const double STAGE_BLOCK_HEIGHT = 2;

const double DEBRI_WIDTH = 3;
const double DEBRI_HEIGHT = 2;

const int LIMIT_TIME = 30;


StageManager::StageManager( ) {
	_stage_obj_max = 0;
	_timer = TimerPtr( new Timer( LIMIT_TIME ) );
}


StageManager::~StageManager( ) {
}

StageBlockPtr StageManager::getStageBlock( int idx ) {
	return _stage_block[ idx ];
}

DebriPtr StageManager::getDebri( int idx ) {
	return _stage_debri[ idx ];
}

void StageManager::addStageBlock( Vector pos, int idx ) {
	StageBlockPtr stage_block = StageBlockPtr( new StageBlock( pos ) );
	_stage_block[ _stage_obj_max ] = stage_block;
	_stage_obj_max++;
}

void StageManager::addDebri( Vector pos, int idx ) {
	DebriPtr stage_debri = DebriPtr( new Debri( pos ) );
	_stage_debri[ _stage_obj_max ] = stage_debri;
	_stage_obj_max++;
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
	double multiple = 0;
	Field::FieldContents field_block;
	multiple_normalize_ray = Vector ( 0, 0, 0 );

	while ( ray.getLength( ) >= multiple_normalize_ray.getLength( ) ) {

		multiple_normalize_ray += origin_pos;

		int x = ( int )( ( multiple_normalize_ray.x + ( DEBRI_WIDTH / 2 ) ) / Field::FX_TO_MX );
		int y = ( int )( ( multiple_normalize_ray.y ) / Field::FY_TO_MY );
		if ( x < 0 ) {
			x = 0;
		}
		if ( y < 0 ) {
			y = 0;
		}
		field_block = field->getFieldObj( x, y );
		if( field_block.x >= 0 && field_block.y >= 0 ) {
			break;
		}
		multiple += 0.5;
		multiple_normalize_ray = ray.normalize( ) * multiple;

	}
	
	if ( field_block.x < 0 || field_block.y < 0 ) {
		return dir;
	}
	
	Vector block_center;
	block_center.x = field_block.x * Field::FX_TO_MX;
	block_center.y = field_block.y;
	Vector plane_point_a;
	Vector plane_point_b;
	Vector plane_point_c;
	Vector plane_point_d;
	if ( field_block.tag == Field::FIELD_OBJ_BLOCK ) {
		//ブロックの左上
		plane_point_a = Vector( block_center.x - ( STAGE_BLOCK_WIDTH / 2 ), block_center.y + ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
		//ブロックの右上
		plane_point_b = Vector( block_center.x + ( STAGE_BLOCK_WIDTH / 2 ), block_center.y + ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
		//ブロックの左下
		plane_point_c = Vector( block_center.x - ( STAGE_BLOCK_WIDTH / 2 ), block_center.y - ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
		//ブロックの右下
		plane_point_d = Vector( block_center.x + ( STAGE_BLOCK_WIDTH / 2 ), block_center.y - ( STAGE_BLOCK_HEIGHT / 2 ), 0 );
	}
	if ( field_block.tag == Field::FIELD_OBJ_DEBRI ) {
		plane_point_a = Vector( block_center.x - ( DEBRI_WIDTH / 2 ), block_center.y + ( DEBRI_HEIGHT / 2 ), 0 );
		//ブロックの右上
		plane_point_b = Vector( block_center.x + ( DEBRI_WIDTH / 2 ), block_center.y + ( DEBRI_HEIGHT / 2 ), 0 );
		//ブロックの左下
		plane_point_c = Vector( block_center.x - ( DEBRI_WIDTH / 2 ), block_center.y - ( DEBRI_HEIGHT / 2 ), 0 );
		//ブロックの右下
		plane_point_d = Vector( block_center.x + ( DEBRI_WIDTH / 2 ), block_center.y - ( DEBRI_HEIGHT / 2 ), 0 );
	}
	
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

double StageManager::getStageBlockWidth( ) {
	return STAGE_BLOCK_WIDTH;
}

double StageManager::getStageBlockHeight( ) {
	return STAGE_BLOCK_HEIGHT;
}

int StageManager::getMaxStageObjNum( ) {
	return _stage_obj_max;
}

