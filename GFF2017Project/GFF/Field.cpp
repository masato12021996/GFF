#include "Field.h"


const int DEBRISTOBLOCKRATIO = 8;

Field::Field( ) {
	_field_width = 0;
}


Field::~Field( ) {

}

Field::FieldContents Field::getFieldBlock( int x, int y ) {
	int idx = x + y * _field_width;
	return _field[ idx ];
}

void Field::setFieldDebris( int x, int y ) {
	int idx = x + y * _field_width;
	_field[ idx ].x = x;
	_field[ idx ].y = y;
}

void Field::setFieldBlock( int x, int y ) {
	for ( int i = 0; i < DEBRISTOBLOCKRATIO; i++ ) {
		int idx = ( x + i ) + y * _field_width;
		_field[ idx ].x = x;
		_field[ idx ].y = y;
	}
}

void Field::setFieldWidth( int width ) {
	_field_width = width;
}
