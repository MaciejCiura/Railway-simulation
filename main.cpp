#include <iostream>
#include <ncurses.h>
#include <atomic>
#include "Map.h"
#include "Track.h"
#include "Train.h"
#include "Station.h"
#include "Renderer.h"
#include "Random.h"

int main()
{
	std::mutex getch_mutex, print_mutex;
	
	auto map = std::make_shared<Map>();
	
	std::atomic<bool> running = true;
	
	for(const auto& station : map->get_stations())
	{
		station->start_thread(std::ref(running));
		for (const auto &train : station->get_trains())
		{
			train->start_thread(std::ref(running));
		}
	}
	
	std::thread scanner([&running, &getch_mutex]() {
		while (running)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			if (getch() == 'q')
				running = false;
		}
	});
	
	Renderer::initialize(map);
	Renderer::update_dimensions();
	while(running)
	{
		std::unique_lock<std::mutex> lock(print_mutex);
		Renderer::draw_screen();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		lock.unlock();
	}

	scanner.join();
	
	for(const auto& train : map->get_trains())
		train->stop_thread();
	
	for(const auto& station : map->get_stations())
		station->stop_thread();
	
	endwin();
	return 0;
}
