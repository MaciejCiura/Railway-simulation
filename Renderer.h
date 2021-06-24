//
// Created by Maciej Ciura on 14.06.2021.
//

#ifndef PROJEKT2_RENDERER_H
#define PROJEKT2_RENDERER_H

#include <mutex>
#include "MapView.h"
#include "Map.h"

#define STATION_WIDTH 10

class Renderer{

public:
	Renderer() = delete;
	
	Renderer(const Renderer &other) = delete;
	
	Renderer &operator=(const Renderer &) = delete;
	
	static void initialize(std::shared_ptr<Map>& map);
	
	static void draw_screen();
	
	static void draw_rectangle(int x1, int y1, int x2, int y2);
	
	static void draw_map();
	
	static void draw_tracks();
	
	static void draw_stations();
	
	static void draw_train(const std::shared_ptr<Train> &train);

	static void update_dimensions();
	
	static void draw_station(const std::shared_ptr<Station> &station);
	
	static void draw_trains();
	
	static void test();
	
	static void test2();
	
	static void print_trains_states();
	
	static std::shared_ptr<Map> map_;
private:
	static double window_height_;
	static double window_width_;
	
	static MapView map_view_;
	
	static std::mutex print_mutex_;
	
	constexpr const static float station_positions[6][2] = {{0.2, 0.7}, {0.5, 0.7}, {0.8, 0.7},
															
															{0.2, 0.3}, {0.5, 0.3}, {0.8, 0.3}};
	
	

};


#endif //PROJEKT2_RENDERER_H
