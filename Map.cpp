//
// Created by Maciej Ciura on 08.06.2021.
//

#include <iostream>
#include "Map.h"

Map::Map(unsigned int size)
		: size_(size)
{
	generate_stations();
	generate_tracks();
	assign_tracks();
	generate_trains(1);
}

void Map::generate_stations()
{
	for (unsigned int i = 0; i < size_; i++)
	{
		stations_.emplace_back(std::make_shared<Station>(i));
	}
}

void Map::generate_tracks()
{
	stations_.reserve(size_);
	for (unsigned int i = 0; i < size_; i++)
		for (unsigned int j = i + 1; j < size_; j++)
		{
			if (matrix_[i][j])
				tracks_.emplace_back(std::make_shared<Track>(stations_[i], stations_[j]));
		}
}

void Map::assign_tracks()
{
	for (unsigned int i = 0; i < size_; i++)
	{
		std::vector<std::shared_ptr<Track>> station_tracks;
		for(auto& track : tracks_)
		{
			
			if (track->from_.lock()->id_ == i || track->to_.lock()->id_ == i)
				station_tracks.emplace_back(track);
		}
		stations_[i]->assign_tracks(station_tracks);
	}
}

void Map::generate_trains(unsigned int n)
{
	int id = 0;
	for(auto& s : stations_)
	{
		for (unsigned int i = 0; i < n; i++)
		{
			auto train = std::make_shared<Train>(id++, s);
			trains_.emplace_back(train);
			s->add_train(train);
		}
	}
}

const std::vector<std::shared_ptr<Station>> &Map::get_stations() const
{
	return stations_;
}

const std::vector<std::shared_ptr<Track>> &Map::get_tracks() const
{
	return tracks_;
}

const std::vector<std::shared_ptr<Train>> &Map::get_trains()
{
	return trains_;
}





