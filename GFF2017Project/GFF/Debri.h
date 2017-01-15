#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"

PTR( Debri );

class Debri {
public:
	Debri( Vector pos );
	virtual ~Debri( );
public:
	Vector getPos( );
private:
	Vector _pos;
};

