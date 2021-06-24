//
// Created by ciamek on 14.06.2021.
//

#ifndef PROJEKT2_MAPMEMBER_H
#define PROJEKT2_MAPMEMBER_H

#include "Position.h"

class MapMember{
public:
	MapMember()
		: position(0,0) {};
	
	virtual int get_x() const
	{
		return position.x_;
	}
	
	virtual int get_y() const
	{
		return position.y_;
	}
	
	void set_position(int x, int y)
	{
		position.x_ = x;
		position.y_ = y;
	}
	
private:
	Position position;
	
	
};

#endif //PROJEKT2_MAPMEMBER_H
