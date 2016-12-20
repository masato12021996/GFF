#include "LoadCSV.h"


LoadCSV::LoadCSV( ) {

}


LoadCSV::~LoadCSV( ) {

}

bool LoadCSV::loadCsv( const char* file_name ) {
	//ファイルの読み込み
	FILE* fp;
	errno_t err = fopen_s( &fp, file_name, "r" );
	if ( err != 0 ) {
		return false;
	}
	
	char buf[ 2048 ];

    //　カウントする
	int width = 0;
	int height = 0;
	while ( fgets( buf, 2048, fp ) != NULL ) {
		int w = 0;
		std::string str = buf;
		while ( true ) {
			w++;
			std::string::size_type index = str.find( "," );

			if ( index == std::string::npos ) {
				_csv_value.push_back( atoi( str.c_str( ) ) );
				break;
			}
			std::string substr = str.substr( 0, index );
			_csv_value.push_back( atoi( substr.c_str( ) ) );
			str = str.substr( index + 1 );
		}

		if ( width < w ) {
			width = w;
		}
		height++;
	}

	_csv_width = width;
	_csv_height = height;
	return true;
}

int LoadCSV::getCsvValue( int idx ) {
	return _csv_value[ idx ];
}

int LoadCSV::getCsvWidth( ) {
	return _csv_width;
}

int LoadCSV::getCsvHeight( ) {
	return _csv_height;
}