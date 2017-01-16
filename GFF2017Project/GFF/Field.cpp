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

Field::FieldContents Field::getFieldObj( int idx ) {
	return _field[idx];
}


void Field::setFieldDebris( int x, int y ) {
	int idx = x + y * _field_width;
	_field[ idx ].x = x;
	_field[ idx ].y = y;
	_field[ idx ].tag = Field::FIELD_OBJ_DEBRI;
	_idx_sevar.push_back(idx);
}

void Field::setFieldBlock( int x, int y ) {
	int idx = x + y * _field_width;
	for ( int i = -4; i < DEBRISTOBLOCKRATIO - 5; i++ ) {
		int fx = ( x + i );
		if ( fx < 0 ) {
			fx = 0;
		}
		int idx_f = fx + y * _field_width;
		_field[ idx_f ].x = x;
		_field[ idx_f ].y = y;
		_field[ idx_f ].tag = Field::FIELD_OBJ_BLOCK;
	}
	_idx_sevar.push_back( idx );
}

void Field::setFieldWidth( int width ) {
	_field_width = width;
}

int Field::getMaxIdx() {
	return _idx_sevar.size( );
}
int Field::getIdx( int i ) {
	return _idx_sevar[ i ];
}