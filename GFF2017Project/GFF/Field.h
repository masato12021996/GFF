#pragma once
#include "smart_ptr.h"
#include <array>

PTR( Field );

class Field {
public:
	static struct FieldContents{
		int x;
		int y;
	};
public:
	Field( );
	virtual ~Field( );
public:
	static const int FX_TO_MX = 3;
	static const int FY_TO_MY = 2;
public:
	void setFieldDebris( int x, int y );
	void setFieldBlock( int x, int y );
	FieldContents getFieldBlock( int x, int y );
	int getFieldMaxWidth( );
	int getFieldMaxHeight( );
	void setFieldWidth( int width );
private:
	static const int MAX_FIELD_X = 10000;
	static const int MAX_FIELD_Y = 10000;
private:
	std::array< FieldContents, MAX_FIELD_X * MAX_FIELD_Y > _field;
	int _field_width;
};

