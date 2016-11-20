#include "Viewer.h"
#include "Application.h"
#include "Drawer.h"

enum MODEL_MDL {
	MODEL_MDL_BOX,
};

ViewerPtr Viewer::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Viewer >( app->getTask( getTag( ) ) );
}

void Viewer::initialize( ) {
	DrawerPtr drawer = Drawer::getTask( );
	//‚±‚±‚ÅƒŠƒ\[ƒX‚Ì“Ç‚Ýž‚Ý
	drawer->loadMDLModel( MODEL_MDL_BOX, "Stage/stage_box_dummy.mdl", "Stage/stage_box_dummy_tex.jpg" );
}

Viewer::Viewer( ) {

}

Viewer::~Viewer( ) {

}

void Viewer::update( ) {
	//‚±‚±‚Å•`‰æˆ—
	drawStageMdl( );
	
}

void Viewer::drawStageMdl( ) {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMDL model_mdl = Drawer::ModelMDL( Vector( 0, 0, 0 ), MODEL_MDL_BOX );
	drawer->setModelMDL( model_mdl );
}
