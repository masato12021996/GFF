#include "Title.h"
#include "Device.h"

Title::Title( ) {
	_is_end_titile = false;
}

Title::~Title( ) {
}

void Title::update( ) {
	DevicePtr device = Device::getTask( );
	_is_end_titile = false;
	if ( device->getButton( ) > 0 ) {
		_is_end_titile = true;
	}
}

bool Title::isEndTitle( ) const {
	return _is_end_titile;
}