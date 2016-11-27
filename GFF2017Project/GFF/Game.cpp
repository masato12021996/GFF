#include "Game.h"
#include "Application.h"

GamePtr Game::getTask( ) {
	ApplicationPtr application = Application::getInstance( );
	return std::dynamic_pointer_cast< Game >( application->getTask( getTag( ) ) );
}

Game::Game( ) {
}

Game::~Game( ) {
}

void Game::update( ) {
}