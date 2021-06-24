//
// Created by ciamek on 08.06.2021.
//

#ifndef PROJEKT2_STATION_H
#define PROJEKT2_STATION_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>

#include "Track.h"
#include "Passenger.h"
#include "Train.h"
#include "MapMember.h"

class Station
		: public MapMember, public std::enable_shared_from_this<Station>
{
public:
	explicit Station(unsigned int id);
	
	Station(const Station &other) = delete;
	
	Station &operator=(const Station &) = delete;
	
	
	const unsigned int id_;
	
	void create_train(unsigned int id);
	
	const std::vector<std::shared_ptr<Track>> &get_tracks() const;
	
	const std::vector<std::shared_ptr<Train>> &get_trains() const;
	
	const std::queue<std::unique_ptr<Passenger>> &get_passengers() const;
	
	void add_train(std::shared_ptr<Train> train);
	
	std::unique_ptr<Passenger> pop_passenger();
	
	void assign_tracks(std::vector<std::shared_ptr<Track>> connections);
	
	void start_passenger_producer_process();

	void remove_train(unsigned int id);
	
	std::mutex passenger_queue_mutex_;
	
	std::condition_variable queue_full;
private:
	std::thread passenger_producer_;
	std::vector<std::shared_ptr<Track>> tracks_;
	std::vector<std::shared_ptr<Train>> trains_;
	std::queue<std::unique_ptr<Passenger>> passengers_;
	
	
};


#endif //PROJEKT2_STATION_H
