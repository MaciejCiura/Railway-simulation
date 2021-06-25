//
// Created by Maciej Ciura on 08.06.2021.
//

#ifndef PROJEKT2_TRAIN_H
#define PROJEKT2_TRAIN_H

#include <thread>
#include <vector>
#include <queue>
#include <atomic>
#include "Track.h"
#include "Passenger.h"
#include "MapMember.h"

enum TrainState{
	WAITING,
	CHOOSING,
	BOARDING,
	MOVING,
	UNBOARDING
};

class Station;

class Train : public MapMember, public std::enable_shared_from_this<Train>
{
public:
	explicit Train(unsigned int id, const std::shared_ptr<Station>& station);
	Train(const Train& other) = delete;
	Train& operator=(const Train&) = delete;
	
	void stop_thread();
	
	void start_thread(std::atomic<bool> &running);
	
	bool board_passenger();
	
	bool unboard_passenger();
	
	const std::weak_ptr<Station> & get_station() const;
	const std::weak_ptr<Station> & get_destination() const;
	
	float get_dist_pct() const;
	
	bool on_station() const;
	float distance_pct_ = 0;
	std::weak_ptr<Station> destination_;
	unsigned int id_;
	std::string get_state();
	const std::queue<std::unique_ptr<Passenger>> &get_passengers() const;

private:
	std::thread thread_;
	
	bool full_ = 0;
	
	int capacity = 10;
	
	TrainState state_;
	
	std::queue<std::unique_ptr<Passenger>> passengers_;
	
	std::condition_variable boarding;
	
private:
	
	std::weak_ptr<Track> track_;
	std::weak_ptr<Station> station_;
	
	
	void choose_track_rand();
	
	void release_track();
	
	void set_destination();
	
	void board_train(std::atomic<bool> &running_);
	
	void unboard_train();
	
	void move_train(std::atomic<bool> &running_);
};


#endif //PROJEKT2_TRAIN_H
