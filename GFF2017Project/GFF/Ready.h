#pragma once
class Ready {
public:
	Ready(  );
	virtual ~Ready(  );
public:
	void update( );
	int getTimeCount( );
	bool isEndReady( ) const;
private:
	int _time;
};