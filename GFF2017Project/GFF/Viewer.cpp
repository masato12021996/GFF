#include "Viewer.h"
#include "Game.h"
#include "Player.h"
#include "StageManager.h"
#include "StageBlock.h"
#include "Animation.h"
#include "Application.h"
#include "Drawer.h"

const double MV1_SCALE = 0.1;
const int TIME_X = 650;
const int TIME_Y = 25;
const int TIME_WIDTH = 672;
const int TIME_HEIGHT = 128;
const int TIME_NUM_WIDTH = 64;
const int TIME_COLON_WIDTH = 32;
const int TIME_COLON_NUM = 10;


enum MODEL_MDL {
	MODEL_MDL_BOX
};

enum MODEL_MV1{
	MODEL_MV1_PLAYER_WAIT,
	MODEL_MV1_PLAYER_RUN,
	MODEL_MV1_PLAYER_HOVER,
	MODEL_MV1_PLAYER_TURBO
};

enum RES {
	RES_UI
};

ViewerPtr Viewer::getTask( ) {
	ApplicationPtr app = Application::getInstance( );
	return std::dynamic_pointer_cast< Viewer >( app->getTask( getTag( ) ) );
}

void Viewer::initialize( ) {
	DrawerPtr drawer = Drawer::getTask( );
	//ここでリソースの読み込み
	//MV1モデルの読み込み
	drawer->loadMV1Model( MODEL_MV1_PLAYER_WAIT, "Model/Player/player_wait.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_RUN, "Model/Player/player_walk.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_HOVER, "Model/Player/player_hover.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_TURBO, "Model/Player/player_turbo.mv1" );
	//MDLファイルモデルの読み込み
	drawer->loadMDLModel( MODEL_MDL_BOX, "Model/Stage/stage_box_dummy.mdl", "Model/Stage/stage_box_dummy_tex.jpg" );
	//UIグラフィック
	drawer->loadGraph( RES_UI, "UI/UI_number_REDandWHITE.png" );
}

Viewer::Viewer( ) {

}

Viewer::~Viewer( ) {

}

void Viewer::update( ) {
	//ここで描画処理
	drawStageMdl( );
	drawPlayer( );
	drawLimitTime( );
}
void Viewer::drawPlayer( ) {
	GamePtr game = Game::getTask( );
	PlayerPtr player = game->getPlayer( );
	DrawerPtr drawer = Drawer::getTask( );
	AnimationPtr animation = player->getAnimation( );

	Vector pos = player->getPos( ) + Vector( 0, -1, 0 );
	
	Matrix mat_rot = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI / 2 * 3 );
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( MV1_SCALE, MV1_SCALE, MV1_SCALE ) );
	Matrix mat_trans = Matrix::makeTransformTranslation( pos );
	
	Matrix mat = mat_rot * mat_scale * mat_trans;

	int motion = animation->getMotion( );
	double anim_tim = animation->getAnimTime( );
	Drawer::ModelMV1 model_mv1 = Drawer::ModelMV1( mat, motion, anim_tim );
	drawer->setModelMV1( model_mv1 );

}

void Viewer::drawLimitTime( ) {
	DrawerPtr drawer = Drawer::getTask( );

	GamePtr game = Game::getTask( );
	StageManagerPtr stage_manager = game->getStageManager( );
	int time = stage_manager->getTimeCount( );
	int second = time / 1000;
	int milli = ( time / 10 ) % 100;
	if ( milli <= 9 && second != 0 ) {
		milli = 99 - milli;
	}
	int y = TIME_Y;
	int ty = 0;
	if ( stage_manager->isTimeLimit( ) ) {
		ty = TIME_HEIGHT;
	}
	//秒数の最初の桁
	if ( log10( second ) < 1 ) {
		int x = TIME_X;
		int tx = 0;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, tx, ty, TIME_NUM_WIDTH, TIME_HEIGHT ), RES_UI ) );
	} else {
		int x = TIME_X;
		int num = second / 10;
		int tx = num * TIME_NUM_WIDTH;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, tx, ty, TIME_NUM_WIDTH, TIME_HEIGHT ), RES_UI ) );
	}
	//秒数の2つ目の桁
	{
		int x = TIME_X + TIME_NUM_WIDTH;
		int num = second % 10;
		int tx = num * TIME_NUM_WIDTH;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, tx, ty, TIME_NUM_WIDTH, TIME_HEIGHT ), RES_UI ) );
	}
	//コロン
	{
		int x = TIME_X + TIME_NUM_WIDTH * 2;
		int num = TIME_COLON_NUM;
		int tx = num * TIME_NUM_WIDTH;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, tx, ty, TIME_COLON_WIDTH, TIME_HEIGHT ), RES_UI ) );
	}
	//ミリ秒数の2つ目の桁
	if ( log10( milli ) < 1 ) {
		int x = TIME_X + TIME_NUM_WIDTH * 3 + TIME_COLON_WIDTH;
		int tx = 0;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, tx, ty, TIME_NUM_WIDTH, TIME_HEIGHT ), RES_UI ) );
	} else {
		int x = TIME_X + TIME_NUM_WIDTH * 3 + TIME_COLON_WIDTH;
		int num = milli % 10;
		int tx = num * TIME_NUM_WIDTH;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, tx, ty, TIME_NUM_WIDTH, TIME_HEIGHT ), RES_UI ) );
	}
	//ミリ秒数の最初の桁
	{
		int x = TIME_X + TIME_NUM_WIDTH * 2 + TIME_COLON_WIDTH;
		int num = milli / 10;
		int tx = num * TIME_NUM_WIDTH;
		drawer->setSprite( Drawer::Sprite( Drawer::Transform( x, y, num * TIME_NUM_WIDTH, ty, TIME_NUM_WIDTH, TIME_HEIGHT ), RES_UI ) );
	}
	
}

void Viewer::drawStageMdl( ) {
	DrawerPtr drawer = Drawer::getTask( );
	
	GamePtr game = Game::getTask( );
	StageManagerPtr stage_manager = game->getStageManager( );
	int stage_max = stage_manager->getMaxStageBlockNum( );
	int block_width = stage_manager->STAGE_MAX_WIDTH;
	int block_height = ( int )stage_manager->getStageBlockHeight( );

	for ( int i = 0; i < StageManager::STAGE_MAX_HEIGHT * StageManager::STAGE_MAX_WIDTH; i++ ) {
		StageBlockPtr stageBlock = stage_manager->getStageBlock( i );
		if ( !stageBlock ) {
			continue;
		}
		Vector pos = stageBlock->getPos( );
		pos.x *= stage_manager->getStageBlockWidth( );
		pos.y *= stage_manager->getStageBlockHeight( );
		pos.y -= 1;
		Drawer::ModelMDL model_mdl = Drawer::ModelMDL( pos, MODEL_MDL_BOX );
		drawer->setModelMDL( model_mdl );
	}
}
