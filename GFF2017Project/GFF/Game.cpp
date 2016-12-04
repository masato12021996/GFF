#include "Game.h"
#include "Player.h"
#include "Application.h"
#include "StageManager.h"

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
	_stage_manager->addStageBlock( Vector( 0, 0, 0 ) );
	_stage_manager->addStageBlock( Vector( 1, 1, 0 ) );

}

void Game::update( ) {
	_player->update( );
}