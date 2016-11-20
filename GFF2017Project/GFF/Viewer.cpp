#include "Viewer.h"
#include "Application.h"
#include "Drawer.h"

ViewerPtr Viewer::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Viewer >( app->getTask( getTag( ) ) );
}

void Viewer::initialize( ) {
	DrawerPtr drawer = Drawer::getTask( );
	//‚±‚±‚ÅƒŠƒ\[ƒX‚Ì“Ç‚İ‚İ
}

Viewer::Viewer( ) {
}

Viewer::~Viewer( ) {
}

void Viewer::update( ) {
	//‚±‚±‚Å•`‰æˆ—
}
