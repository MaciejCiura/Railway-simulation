#include <iostream>
#include <ncurses.h>
#include "Map.h"
#include "TicketInspector.h"
#include "Track.h"
#include "Train.h"
#include "Station.h"
#include "Renderer.h"
#include "Random.h"

int main()
{
	auto map = std::make_shared<Map>();
	int b;
	
	for(const auto& station : map->get_stations())
	{
		station->start_passenger_producer_process();
		for (const auto &train : station->get_trains())
		{
//			std::cout << "Starting train process";
			train->start_thread();
		}
	}
//	map->get_trains()[0]->start_thread();
	
	std::thread thread([&map](){
		Renderer::initialize(map);
		Renderer::update_dimensions();
		std::cout << "initialized";
		while(1)
		{
			Renderer::draw_screen();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	});

	
	endwin();
	int a;
	std::cin >> a;
	for(const auto train : map->get_trains())
		train->stop_thread();
	return 0;
}
