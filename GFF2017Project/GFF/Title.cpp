#include "Title.h"
#include "Device.h"
#include "Game.h"

Title::Title( ) {
	_is_end_titile = false;
	_stage_num = 0;
}

Title::~Title( ) {
}

void Title::update( ) {
	swicthStageSlect( );

	DevicePtr device = Device::getTask( );
	_is_end_titile = false;
	if ( device->getButton( ) > 0 ) {
		_is_end_titile = true;
	}
}

void Title::swicthStageSlect( ) {
	GamePtr game = Game::getTask( );

	int max_stage_num = game->getStageNum( );
	DevicePtr device = Device::getTask( );
	if ( device->getDirY( ) > 0 ) {
		_stage_num++;
	}else if ( device->getDirY( ) < 0 ) {
		_stage_num--;
	}
	if ( _stage_num <= 0 ) {
		_stage_num = 0;
	}
	if ( _stage_num >= max_stage_num ) {
		_stage_num = max_stage_num - 1;
	}

	_stage_str = game->getStageStr( _stage_num );
}

bool Title::isEndTitle( ) const {
	return _is_end_titile;
}

std::string Title::getSelectStageStr( ) const {
	return _stage_str;
}