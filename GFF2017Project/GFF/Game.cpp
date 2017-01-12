#include "Game.h"
#include "Player.h"
#include "CameraCtr.h"
#include "Application.h"
#include "StageManager.h"
#include "LoadCSV.h"


GamePtr Game::getTask( ) {
	ApplicationPtr application = Application::getInstance( );
	return std::dynamic_pointer_cast< Game >( application->getTask( getTag( ) ) );
}

Game::Game( ) {
}

Game::~Game( ) {
}

PlayerPtr Game::getPlayer( ) const {
	return _player;
}

StageManagerPtr Game::getStageManager( ) const {
	return _stage_manager;
}

void Game::initialize( ) {
	_player = PlayerPtr( new Player( ) );
	_stage_manager = StageManagerPtr( new StageManager( ) );
	_camera_ctr = CameraCtrPtr( new CameraCtr( ) );
	LoadCSV csv;
	csv.loadCsv( "../Resources/MapData/MapData.csv" );
	int map_width = csv.getCsvWidth( );
	int map_height = csv.getCsvHeight( );
	_stage_manager->setStageWidth( map_width );
	_stage_manager->setStageHeight( map_height );
	for ( int i = ( map_width * map_height ) - 1; i >= 0; i-- ) {
		if ( csv.getCsvValue( i ) == 0 ) {
			continue;
		}
		Vector pos;
		pos.x = ( ( i ) % map_width );
		pos.y =  map_height - ( ( i ) / map_width ) - 1;
		pos.z = 0;
		_stage_manager->addStageBlock( pos, ( map_width * map_height ) - ( i + 1 )  );
	}
}

void Game::update( ) {
	if ( !_stage_manager->isTimerStart( ) ) {
		_stage_manager->timerStart( );
	}
	_player->update( );
	_camera_ctr->update( );
//	_stage_manager->isHitBlock( Vector( 0, 1, 0 ) );
}