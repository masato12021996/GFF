#pragma once
#include "smart_ptr.h"
#include <array>
#include <vector>

PTR( Field );

class Field {
public:
	enum FIELD_OBJ {
		FIELD_OBJ_BLOCK,
		FIELD_OBJ_DEBRI,
	};
public:
	struct FieldContents{
		int x;
		int y;
		FIELD_OBJ tag;
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
	FieldContents getFieldObj( int idx );

	int getMaxIdx( );
	int getIdx( int i );
	FieldContents getFieldObj( int x, int y );
	int getFieldMaxWidth( );
	int getFieldMaxHeight( );
	void setFieldWidth( int width );
private:
	static const int MAX_FIELD_X = 5000;
	static const int MAX_FIELD_Y = 5000;
private:
	std::array< FieldContents, MAX_FIELD_X * MAX_FIELD_Y > _field;
	std::vector< int > _idx_sevar;
	int _field_width;
};

