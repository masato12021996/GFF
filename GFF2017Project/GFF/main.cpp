#include "Application.h"
#include "Camera.h"
#include "Viewer.h"
#include "Drawer.h"
#include "Mouse.h"

void main( ) {
	ApplicationPtr application = Application::getInstance( );
	CameraPtr camera = CameraPtr( new Camera( ) );
	ViewerPtr viewer = ViewerPtr( new Viewer( ) );
	DrawerPtr drawer = DrawerPtr( new Drawer( "../Resources" ) );
	MousePtr mouse = MousePtr( new Mouse( ) );

	application->addTask( Camera::getTag( ), camera );
	application->addTask( Viewer::getTag( ), viewer );
	application->addTask( Drawer::getTag( ), drawer );
	application->addTask( Mouse::getTag( ), mouse );

}