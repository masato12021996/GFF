#include "Viewer.h"
#include "Application.h"
#include "Drawer.h"

enum MODEL_MDL {
	MODEL_MDL_BOX
};

enum MODEL_MV1{
	MODEL_MV1_PLAYER_WAIT
};

ViewerPtr Viewer::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Viewer >( app->getTask( getTag( ) ) );
}

void Viewer::initialize( ) {
	DrawerPtr drawer = Drawer::getTask( );
	//�����Ń��\�[�X�̓ǂݍ���
	//MV1���f���̓ǂݍ���
	drawer->loadMV1Model( MODEL_MV1_PLAYER_WAIT, "Model/Player/player_wait.mv1", 0.1 );
	//MDL�t�@�C�����f���̓ǂݍ���
	drawer->loadMDLModel( MODEL_MDL_BOX, "Model/Stage/stage_box_dummy.mdl", "Model/Stage/stage_box_dummy_tex.jpg" );
}

Viewer::Viewer( ) {

}

Viewer::~Viewer( ) {

}

void Viewer::update( ) {
	//�����ŕ`�揈��
	//drawStageMdl( );
	drawPlayer( );
}
void Viewer::drawPlayer( ) {
	DrawerPtr drawer = Drawer::getTask( );
	Vector pos = Vector( 0, 0, 0 );
	Vector dir = Vector( 1, 0, 0 );
	int motion = MODEL_MV1_PLAYER_WAIT;
	double anim_tim = 0.0;
	Drawer::ModelMV1 model_mv1 = Drawer::ModelMV1( pos, dir, motion, anim_tim );
	drawer->setModelMV1( model_mv1 );
}

void Viewer::drawStageMdl( ) {
	DrawerPtr drawer = Drawer::getTask( );
	Drawer::ModelMDL model_mdl = Drawer::ModelMDL( Vector( 0, 0, 0 ), MODEL_MDL_BOX );
	drawer->setModelMDL( model_mdl );
}
