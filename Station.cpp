//
// Created by ciamek on 08.06.2021.
//

#include "Station.h"

#include <utility>
#include <algorithm>


Station::Station(unsigned int id)
		: id_(id)
{
}

const std::vector<std::shared_ptr<Track>> &Station::get_tracks() const
{
	return tracks_;
}

void Station::start_thread(std::atomic<bool> &runing)
{
	//TODO add synchronization
	thread_ = std::thread([this, &runing](){
		while(runing)
		{
			if (passengers_.size() < 25)
			{
				std::unique_lock<std::mutex> lock(passenger_queue_mutex_);
				passengers_.emplace(std::make_unique<Passenger>(3));
				lock.unlock();
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
	});
}

const std::vector<std::shared_ptr<Train>> &Station::get_trains() const
{
	return trains_;
}

const std::queue<std::unique_ptr<Passenger>> &Station::get_passengers() const
{
	return passengers_;
}

std::unique_ptr<Passenger> Station::pop_passenger()
{
	std::unique_ptr<Passenger> tmp = std::move(passengers_.back());
	passengers_.pop();
	return tmp;
}

void Station::assign_tracks(std::vector<std::shared_ptr<Track>> connections)
{
	tracks_ = std::move(connections);
}

void Station::add_train(std::shared_ptr<Train> train)
{
	trains_.emplace_back(std::move(train));
}

void Station::remove_train(unsigned int id)
{
	trains_.erase(std::remove_if(trains_.begin(), trains_.end(), [id](std::shared_ptr<Train> &train){return train->id_==id;}), trains_.end());
}

void Station::stop_thread()
{
	thread_.join();
}
