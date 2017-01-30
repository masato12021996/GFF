#pragma once
class Title {
public:
	Title( );
	virtual ~Title( );
public:
	void update( );
	bool isEndTitle( ) const;
private:
	bool _is_end_titile;
};