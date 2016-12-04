#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"

PTR( StageBlock );

class StageBlock {
public:
	StageBlock( Vector pos );
	virtual ~StageBlock( );
public:
	Vector getPos( );
private:
	Vector _pos;
};

