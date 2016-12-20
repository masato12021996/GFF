#pragma once
#include <vector>

class LoadCSV {
public:
	LoadCSV( );
	virtual ~LoadCSV( );
public:
	bool loadCsv( const char* file_name );
	int getCsvWidth( );
	int getCsvHeight( );
	int getCsvValue( int idx );
private:
	int _csv_width;
	int _csv_height;
	std::vector< int > _csv_value;
};

