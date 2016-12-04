#include "StageBlock.h"


StageBlock::StageBlock( Vector pos ) :
_pos( pos ) {

}


StageBlock::~StageBlock( ) {

}

Vector StageBlock::getPos( ) {
	return _pos;
}