#include "Viewer.h"
#include "Game.h"
#include "Player.h"
#include "StageManager.h"
#include "StageBlock.h"
#include "Animation.h"
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
	//ここでリソースの読み込み
	//MV1モデルの読み込み
	drawer->loadMV1Model( MODEL_MV1_PLAYER_WAIT, "Model/Player/player_wait.mv1", 0.1 );
	//MDLファイルモデルの読み込み
	drawer->loadMDLModel( MODEL_MDL_BOX, "Model/Stage/stage_box_dummy.mdl", "Model/Stage/stage_box_dummy_tex.jpg" );
}

Viewer::Viewer( ) {

}

Viewer::~Viewer( ) {

}

void Viewer::update( ) {
	//ここで描画処理
	drawStageMdl( );
	drawPlayer( );
}
void Viewer::drawPlayer( ) {
	GamePtr game = Game::getTask( );
	PlayerPtr player = game->getPlayer( );
	DrawerPtr drawer = Drawer::getTask( );
	AnimationPtr animation = player->getAnimation( );

	Vector pos = player->getPos( );
	Vector dir = player->getDir( );
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI / 2 );
	mat = Matrix::makeTransformScaling( Vector( 0.1, 0.1, 0.1 ) );

	int motion = animation->getMotion( );
	double anim_tim = animation->getAnimTime( );
	Drawer::ModelMV1 model_mv1 = Drawer::ModelMV1( mat, motion, anim_tim );
	drawer->setModelMV1( model_mv1 );
}

void Viewer::drawStageMdl( ) {
	DrawerPtr drawer = Drawer::getTask( );
	
	GamePtr game = Game::getTask( );
	StageManagerPtr stage_manager = game->getStageManager( );
	int stage_max = stage_manager->getMaxStageBlockNum( );
	for ( int i = 0; i < stage_max; i++ ) {
		StageBlockPtr stageBlock = stage_manager->getStageBlock( i );
		Vector pos = stageBlock->getPos( );
		pos.x *= stage_manager->getStageBlockWidth( ) * 3;
		pos.y *= stage_manager->getStageBlockHeight( );
		pos.y -= 1;
		Drawer::ModelMDL model_mdl = Drawer::ModelMDL( pos, MODEL_MDL_BOX );
		drawer->setModelMDL( model_mdl );
	}
	
}
