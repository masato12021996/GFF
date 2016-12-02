#include "Game.h"
#include "Player.h"
#include "Application.h"

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

void Game::initialize( ) {
	_player = PlayerPtr( new Player( ) );
}

void Game::update( ) {
	_player->update( );
}