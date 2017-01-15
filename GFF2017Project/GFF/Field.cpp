#include "Field.h"


const int DEBRISTOBLOCKRATIO = 8;

Field::Field( ) {
	_field_width = 0;
}


Field::~Field( ) {

}

Field::FieldContents Field::getFieldObj( int x, int y ) {
	int idx = x + y * _field_width;
	return _field[ idx ];
}

void Field::setFieldDebris( int x, int y ) {
	int idx = x + y * _field_width;
	_field[ idx ].x = x;
	_field[ idx ].y = y;
	_field[ idx ].tag = Field::FIELD_OBJ_DEBRI;
}

void Field::setFieldBlock( int x, int y ) {
	for ( int i = -3; i < DEBRISTOBLOCKRATIO - 3; i++ ) {
		int fx = ( x + i );
		if ( fx < 0 ) {
			fx = 0;
		}
		int idx = fx + y * _field_width;
		_field[ idx ].x = x;
		_field[ idx ].y = y;
		_field[ idx ].tag = Field::FIELD_OBJ_BLOCK;
	}
}

void Field::setFieldWidth( int width ) {
	_field_width = width;
}
