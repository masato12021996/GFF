#pragma once
#include <string>

class Title {
public:
	Title( );
	virtual ~Title( );
public:
	void update( );
	std::string getSelectStageStr( ) const;
	bool isEndTitle( ) const;
private:
	void swicthStageSlect( );
private:
	bool _is_end_titile;
	std::string _stage_str;
	int _stage_num;
};