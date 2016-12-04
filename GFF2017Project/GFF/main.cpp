#include "Application.h"
#include "Mouse.h"
#include "Device.h"
#include "Drawer.h"
#include "Camera.h"
#include "Viewer.h"
#include "Game.h"

void main( ) {
	ApplicationPtr application = Application::getInstance( );
	MousePtr mouse = MousePtr( new Mouse );
	DevicePtr device = DevicePtr( new Device );
	DrawerPtr drawer = DrawerPtr( new Drawer( "../Resources" ) );
	CameraPtr camera = CameraPtr( new Camera );
	ViewerPtr viewer = ViewerPtr( new Viewer );
	GamePtr game = GamePtr( new Game );

	application->addTask( Mouse::getTag( ), mouse );
	application->addTask( Device::getTag( ), device );
	application->addTask( Drawer::getTag( ), drawer );
	application->addTask( Camera::getTag( ), camera );
	application->addTask( Viewer::getTag( ), viewer );
	application->addTask( Game::getTag( ), game );
}