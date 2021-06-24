//
// Created by Maciej Ciura on 08.06.2021.
//

#ifndef PROJEKT2_TRACK_H
#define PROJEKT2_TRACK_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include "MapMember.h"

class Station;

class Track : public MapMember
{
public:
	Track( const std::shared_ptr<Station>& from, const std::shared_ptr<Station>& to);
	Track(const Track& other) = delete;
	Track& operator=(const Track&) = delete;
	int get_x() const final;
	std::mutex mutex;
	std::weak_ptr<Station> from_, to_;
	std::condition_variable track_in_use;
	bool free_;
private:
	
	std::thread thread;
	
	
};


#endif //PROJEKT2_TRACK_H
