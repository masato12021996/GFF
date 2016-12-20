#include "Game.h"
#include "Player.h"
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
	LoadCSV csv;
	csv.loadCsv( "../Resources/MapData/MapData.csv" );
	int map_width = csv.getCsvWidth( );
	int map_height = csv.getCsvHeight( );
	_stage_manager ->setMaxBlockNum( map_width * map_height );
	for ( int i = 0; i < map_width * map_height; i++ ) {
		if ( csv.getCsvValue( i ) == 0 ) {
			continue;
		}
		Vector pos;
		pos.x = ( i % map_width );
		pos.y =  map_height - ( i / map_width ) - 1;
		pos.z = 0;
		_stage_manager->addStageBlock( pos, i );
	}
}

void Game::update( ) {
	_player->update( );
}