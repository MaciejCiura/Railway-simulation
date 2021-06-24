//
// Created by Maciej Ciura on 08.06.2021.
//

#ifndef PROJEKT2_MAP_H
#define PROJEKT2_MAP_H

#define MAPSIZE 6

#include <thread>
#include <vector>
#include "Station.h"
#include "Track.h"

class Map
{
public:
	explicit Map(unsigned int size = MAPSIZE);
	
	Map(const Map &other) = delete;
	
	Map &operator=(const Map &) = delete;
	
	
	const std::vector<std::shared_ptr<Station>> &get_stations() const;
	
	const std::vector<std::shared_ptr<Track>> &get_tracks() const;
	
	const std::vector<std::shared_ptr<Train>> &get_trains();
	


private:
	unsigned int size_;
	std::thread thread_;
	std::vector<std::vector<unsigned int>> matrix_ = {{0, 1, 0, 1, 0, 0},
													  {1, 0, 1, 0, 1, 0},
													  {0, 1, 0, 0, 0, 1},
													  {1, 0, 0, 0, 1, 0},
													  {0, 1, 0, 1, 0, 1}, //TODO random generation of the map
													  {0, 0, 1, 0, 1, 0}};
	std::vector<std::shared_ptr<Station>> stations_;
	
	std::vector<std::shared_ptr<Track>> tracks_;
	
	std::vector<std::shared_ptr<Train>> trains_;
	
	void generate_stations();
	
	void generate_tracks();

	void generate_trains(unsigned int n);
	
	void assign_tracks();
};


#endif //PROJEKT2_MAP_H
