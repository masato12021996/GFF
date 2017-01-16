#include "Viewer.h"
#include "Game.h"
#include "Player.h"
#include "CameraCtr.h"
#include "Camera.h"
#include "StageManager.h"
#include "StageBlock.h"
#include "Debri.h"
#include "Field.h"
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
//小さいほどに回転が速くなる
const int TOWER_ROTATE_SPEED = 20;


enum MODEL_MDL {
	MODEL_MDL_BOX,
	MODEL_MDL_DEBRI,
};

enum MODEL_MV1{
	MODEL_MV1_PLAYER_WAIT,
	MODEL_MV1_PLAYER_RUN,
	MODEL_MV1_PLAYER_HOVER,
	MODEL_MV1_PLAYER_TURBO,
	MODEL_MV1_PLAYER_REVERSAL,
	MODEL_MV1_PLAYER_READY,
	MODEL_MV1_PLAYER_LAND,
	MODEL_MV1_PLAYER_JUMP,
	MODEL_MV1_PLAYER_HOVER_JUMP,
	MODEL_MV1_PLAYER_HOVER_CRASH,
	MODEL_MV1_PLAYER_GOAL,
	MODEL_MV1_PLAYER_FALL,
	MODEL_MV1_PLAYER_CRASH,

	MODEL_MV1_BACK_GROUND,
	MODEL_MV1_BACK_TOWER,

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
	drawer->loadMV1Model( MODEL_MV1_PLAYER_RUN, "Model/Player/player_run.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_HOVER, "Model/Player/player_hover.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_TURBO, "Model/Player/player_turbo.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_REVERSAL, "Model/Player/player_reversal.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_READY, "Model/Player/player_ready.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_LAND, "Model/Player/player_land.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_JUMP, "Model/Player/player_jump.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_HOVER_JUMP, "Model/Player/player_hover_jump.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_HOVER_CRASH, "Model/Player/player_hover_crash.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_GOAL, "Model/Player/player_goal.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_FALL, "Model/Player/player_fall.mv1" );
	drawer->loadMV1Model( MODEL_MV1_PLAYER_CRASH, "Model/Player/player_crash.mv1" );

	drawer->loadMV1Model( MODEL_MV1_BACK_GROUND, "Model/Stage/back_ground.mv1" );
	drawer->loadMV1Model( MODEL_MV1_BACK_TOWER, "Model/Stage/back_tower.mv1" );

	//MDLファイルモデルの読み込み
	drawer->loadMDLModel( MODEL_MDL_BOX, "Model/Stage/stage_box_dummy.mdl", "Model/Stage/box_tex.jpg" );
	drawer->loadMDLModel( MODEL_MDL_DEBRI, "Model/Stage/debri.mdl", "Model/Stage/debri_tex.jpg" );


	//UIグラフィック
	drawer->loadGraph( RES_UI, "UI/UI_number_REDandWHITE.png" );
	_back_tower_angle = 0;
}

Viewer::Viewer( ) {

}

Viewer::~Viewer( ) {

}

void Viewer::update( ) {
	//ここで描画処理
	drawStageMdl( );
	drawPlayer( );
	drawBackTower( );
	drawBackGround( );
	drawLimitTime( );
}
void Viewer::drawPlayer() {
	GamePtr game = Game::getTask();
	PlayerPtr player = game->getPlayer();
	DrawerPtr drawer = Drawer::getTask();
	AnimationPtr animation = player->getAnimation();

	Vector pos = player->getPos();
	Matrix mat_rot = Matrix::makeTransformRotation(Vector(0, 1, 0), PI / 2 * 3);
	Matrix mat_reversal_rot = Matrix::makeTransformRotation(Vector(0, 0, 1), PI * 3 );
	Matrix mat_scale = Matrix::makeTransformScaling(Vector(MV1_SCALE, MV1_SCALE, MV1_SCALE));
	Matrix mat_trans = Matrix::makeTransformTranslation(pos);
	bool _is_reversal = player->isReversal();
	Matrix mat = mat_rot;
	if (_is_reversal) {
		mat = Matrix::makeTransformRotation(Vector(0, 1, 0), PI / 6 * 11 );
		mat = mat * mat_reversal_rot;
	}
	mat = mat * mat_scale * mat_trans;
	
	int motion = animation->getMotion( );
	double anim_tim = animation->getAnimTime( );
	Drawer::ModelMV1 model_mv1 = Drawer::ModelMV1( mat, motion, anim_tim );
	drawer->setModelMV1( model_mv1 );

	/*StageManagerPtr stage_manager = game->getStageManager( );
	Vector ans = stage_manager->raycastBlock( pos, Vector( pos.x, pos.y - 3 ) );
	drawer->drawString( 0, 0, "x:%lf, y:%lf", ans.x, ans.y );*/
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
	int stage_obj_max = stage_manager->getMaxStageObjNum( );
	for ( int i = 0; i < stage_obj_max; i++ ) {
		StageBlockPtr stageBlock = stage_manager->getStageBlock( i );
		if ( stageBlock ) {
			Vector pos = stageBlock->getPos( );
			pos.x *= Field::FX_TO_MX;
			pos.y *= Field::FY_TO_MY;
			Drawer::ModelMDL model_mdl = Drawer::ModelMDL( pos, MODEL_MDL_BOX );
			drawer->setModelMDL( model_mdl );
		}
		DebriPtr debri = stage_manager->getDebri( i );
		if ( debri ) {
			Vector pos = debri->getPos( );
			pos.x *= Field::FX_TO_MX;
			pos.y *= Field::FY_TO_MY;
			pos.y -= 1;
			Drawer::ModelMDL model_mdl = Drawer::ModelMDL( pos, MODEL_MDL_DEBRI );
			drawer->setModelMDL( model_mdl );
		}
	}
}

void Viewer::drawBackTower( ) {
	
	DrawerPtr drawer = Drawer::getTask( );
	GamePtr game = Game::getTask( );

	CameraCtrPtr camera_ctr = game->getCameraCtr( );
	CameraPtr camera = camera_ctr->getCamera( );
	Vector camera_pos = camera->getTargetPos( );
	camera_pos.z = 0;
	Vector pos = Vector ( 0, 0, 0 );
	pos += camera_pos;
	pos.z = 100;
	PlayerPtr player = game->getPlayer( );
	_back_tower_angle += ( player->getSpeed( ).x / TOWER_ROTATE_SPEED );
	Matrix mat_rot = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), _back_tower_angle );
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( MV1_SCALE * 0.7, MV1_SCALE * 0.7, MV1_SCALE * 0.7 ) );
	Matrix mat_trans = Matrix::makeTransformTranslation( pos );
	
	Matrix mat = mat_rot * mat_scale * mat_trans;

	Drawer::ModelMV1 model_mv1 = Drawer::ModelMV1( mat, MODEL_MV1_BACK_TOWER, 0 );
	drawer->setModelMV1( model_mv1 );
}


void Viewer::drawBackGround( ) {
	DrawerPtr drawer = Drawer::getTask( );
	GamePtr game = Game::getTask( );
	CameraCtrPtr camera_ctr = game->getCameraCtr( );
	CameraPtr camera = camera_ctr->getCamera( );
	Vector camera_pos = camera->getTargetPos( );
	camera_pos.z = 0;
	Vector pos = Vector( 0, 7, 120 );
	pos += camera_pos;
	Matrix mat_rot = Matrix::makeTransformRotation( Vector( 0, 0, 0 ), 0 );
	Matrix mat_scale = Matrix::makeTransformScaling( Vector( MV1_SCALE * 1.2, MV1_SCALE * 1.2, MV1_SCALE * 1.2 ) );
	Matrix mat_trans = Matrix::makeTransformTranslation( pos );
	
	Matrix mat = mat_rot * mat_scale * mat_trans;

	Drawer::ModelMV1 model_mv1 = Drawer::ModelMV1( mat, MODEL_MV1_BACK_GROUND, 0 );
	drawer->setModelMV1( model_mv1 );
}

