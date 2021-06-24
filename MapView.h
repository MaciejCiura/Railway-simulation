//
// Created by ciamek on 14.06.2021.
//

#ifndef PROJEKT2_MAPVIEW_H
#define PROJEKT2_MAPVIEW_H

struct Border{
	int x1;
	int y1;
	
	int x2;
	int y2;
};

struct MapView{
	
	double map_height_;
	double map_width_;
	
	Border border;
};

#endif //PROJEKT2_MAPVIEW_H
