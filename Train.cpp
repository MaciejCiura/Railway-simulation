//
// Created by Maciej Ciura on 08.06.2021.
//

#include "Train.h"
#include "Station.h"
#include "Random.h"
#include <utility>
#include <iostream>
#include <mutex>
#include <curses.h>

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_RandomDistribution;


Train::Train(unsigned int id, const std::shared_ptr<Station> &station)
		: id_(id), station_(station)
{

}

void Train::thread_func()
{
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(5 + Random::get_int() % 4));
		
		
		choose_track_rand();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		
		set_destination();
		
		state_ = BOARDING;
		board_train();
		
		std::this_thread::sleep_for(std::chrono::seconds(Random::get_int() % 4));

		state_ = MOVING;
		move_train();
		
		release_track();
		
		state_ = UNBOARDING;
		unboard_train();
		
		state_ = WAITING;
		std::this_thread::sleep_for(std::chrono::seconds(Random::get_int() % 4));
		distance_pct_ = 0;
	}
}

void Train::move_train()
{
	while (distance_pct_ < 1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		distance_pct_ += 0.01;
	}
}

void Train::unboard_train()
{
	while(!passengers_.empty())
	{
		full_ =false;
		if(unboard_passenger())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds (200));
		}
	}
}

void Train::board_train()
{
	station_.lock()->remove_train(id_);
	
	while(!full_)
	{
		if(board_passenger())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds (200));
			if(passengers_.size() == capacity)
				full_ = true;
		}
	}
}

void Train::set_destination()
{
	if (station_.lock() == track_.lock()->from_.lock())
		destination_ = track_.lock()->to_;
	else
	{
		destination_ = track_.lock()->from_;
		station_ = track_.lock()->to_;
	}
}

void Train::release_track()
{
	station_ = destination_;
	station_.lock()->add_train(shared_from_this());
	destination_.reset();
	track_.lock()->free_ = true;
	track_.lock()->track_in_use.notify_one();
	track_.lock()->mutex.unlock();
}

void Train::choose_track_rand()
{
	track_ = station_.lock()->get_tracks()[Random::get_int() % station_.lock()->get_tracks().size()]; //TODO random
	state_ = CHOOSING;
	std::unique_lock<std::mutex> track_lock(track_.lock()->mutex);
	track_.lock()->track_in_use.wait(track_lock, [this]{return track_.lock()->free_;});
	track_.lock()->free_ = false;
	track_lock.unlock();
}

void Train::start_thread()
{
	thread_ = std::thread(&Train::thread_func, this);
}

void Train::stop_thread()
{
	thread_.join();
}

bool Train::board_passenger()
{
	std::unique_lock<std::mutex> lock(station_.lock()->passenger_queue_mutex_);
	if (!station_.lock()->get_passengers().empty())
	{
		passengers_.emplace(station_.lock()->pop_passenger());
		lock.unlock();
		return true;
	}
	lock.unlock();
	return false;
}

bool Train::unboard_passenger()
{
	std::unique_lock<std::mutex> lock(station_.lock()->passenger_queue_mutex_);
	if (!passengers_.empty())
	{
		passengers_.pop();
		return true;
	}
	return false;
}

const std::weak_ptr<Station> &Train::get_station() const
{
	return station_;
}

const std::weak_ptr<Station> &Train::get_destination() const
{
	return destination_;
}

float Train::get_dist_pct() const
{
	return distance_pct_;
}

bool Train::on_station() const
{
	return destination_.lock() == nullptr;
}

std::string Train::get_state()
{
	switch (static_cast<int>(state_))
	{
		case 0:
			return "Waiting       ";
		
		case 1:
			return "Waiting for track " + std::to_string(track_.lock()->from_.lock()->id_) +'-' + std::to_string(track_.lock()->to_.lock()->id_);
		
		case 2:
			return "Boarding      ";
		
		case 3:
			return "Moving from " + std::to_string(station_.lock()->id_) + " to " + std::to_string(destination_.lock()->id_);
			
		case 4:
			return "Unboarding    ";
			
		default:
			break;
	}
	return "Wrong state";
}

const std::queue<std::unique_ptr<Passenger>> &Train::get_passengers() const
{
	return passengers_;
}



