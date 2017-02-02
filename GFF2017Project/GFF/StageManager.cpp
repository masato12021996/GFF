#include "StageManager.h"
#include "StageBlock.h"
#include "Debri.h"
#include "Field.h"
#include "Timer.h"
#include "Player.h"
#include "Game.h"


const double STAGE_BLOCK_WIDTH = 24;
const double STAGE_BLOCK_HEIGHT = 2;

const double DEBRI_WIDTH = 3;
const double DEBRI_HEIGHT = 2;


StageManager::StageManager(  ) {
	_stage_obj_max = 0;
	_timer = TimerPtr ( new Timer( 30 ) );
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
	DebriPtr debri = DebriPtr( new Debri( pos ) );
	_stage_debri[ _stage_obj_max ] = debri;
	_stage_obj_max++;
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

//判定先がいなかったらオリジンPOSを返す。
Vector StageManager::raycastBlock( Vector origin_pos, Vector next_pos ) {
	GamePtr app = Game::getTask( );
	Vector dir = origin_pos + next_pos;
	Vector ray = dir - origin_pos;

	if ( ray.getLength( ) == 0 ) {
		return origin_pos;
	}
	
	FieldPtr field = app->getField( );
	int max_idx = field->getMaxIdx( );
	Vector final_out = dir;
	bool is_not_closs = true;
	bool final_hit_debri = false;
	for ( int i = 0; i < max_idx; i++ ) {
		bool now_hit_debri = false;
		int idx = field->getIdx( i );
		Field::FieldContents field_block = field->getFieldObj(idx);
		Vector block_center;
		block_center.x = field_block.x * Field::FX_TO_MX;
		block_center.y = field_block.y * Field::FY_TO_MY;
		Vector plane_point_a;
		Vector plane_point_b;
		Vector plane_point_c;
		Vector plane_point_d;

		if ( field_block.tag == Field::FIELD_OBJ_BLOCK ) {
			now_hit_debri = false;
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
			now_hit_debri = true;

			plane_point_a = Vector( block_center.x - ( DEBRI_WIDTH / 2 ), block_center.y + ( DEBRI_HEIGHT / 2 ), 0 );
			//ブロックの右上
			plane_point_b = Vector( block_center.x + ( DEBRI_WIDTH / 2 ), block_center.y + ( DEBRI_HEIGHT / 2 ), 0 );
			//ブロックの左下
			plane_point_c = Vector( block_center.x - ( DEBRI_WIDTH / 2 ), block_center.y - ( DEBRI_HEIGHT / 2 ), 0 );
			//ブロックの右下
			plane_point_d = Vector( block_center.x + ( DEBRI_WIDTH / 2 ), block_center.y - ( DEBRI_HEIGHT / 2), 0 );
		}

		Vector block_origin_pos[ 4 ] = { plane_point_a, plane_point_a, plane_point_d, plane_point_d };
		Vector block_dir[ 4 ] = { plane_point_b, plane_point_c, plane_point_b, plane_point_c };

		Vector a1 = origin_pos;
		Vector a2 = dir;
		
	
		for ( int i = 0; i < 4; i++ ) {

			Vector b1 = block_origin_pos[ i ];
			Vector b2 = block_dir[ i ];
			if ( ( cross( a2 - a1, b1 - a1 ) * cross( a2 - a1, b2 - a1 ) > 0.000001 ) ||
				( cross( b2 - b1, a1 - b1 ) * cross( b2 - b1, a2 - b1 ) > 0.000001 ) ) {
				continue;
			}
			is_not_closs = false;
			Vector a = a2 - a1;
			Vector b = b2 - b1;
			double d1 = cross( b, b1 - a1 );
			double d2 = cross( b, a );
			Vector out = a1 + a * ( 1 / d2 ) * d1;
			bool near_for_origin = ( final_out - origin_pos ).getLength( ) > ( out - origin_pos ).getLength( );
			if ( near_for_origin ) {
				final_hit_debri = now_hit_debri;
				final_out = out;
			}
		}
		
	}
	if ( is_not_closs ) {
		final_out = origin_pos;
	}
	if ( final_hit_debri ) {
		//デブリ判定を行う為プレイヤーはここでしか使わない。
		GamePtr game = Game::getTask( );
		PlayerPtr player = game->getPlayer( );
		player->setHitDebri( final_hit_debri );
	}
	return final_out;
	
}

void StageManager::setTimer( TimerPtr timer ) {
	_timer = timer;
}

int StageManager::getTimeCount( ) {
	return _timer->getTimeCount( ); 
}
void StageManager::timerStart( ) {
	_timer->timerStart( );
}
void StageManager::setClear( ) {
	return _timer->clear( );
}

bool StageManager::isTimeLimit( ) {
	return _timer->isTimeLimit( );
}
bool StageManager::isTimerStart( ) {
	return _timer->isTimerStart( );
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