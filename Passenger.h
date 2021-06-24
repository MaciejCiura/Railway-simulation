//
// Created by ciamek on 08.06.2021.
//

#ifndef PROJEKT2_PASSENGER_H
#define PROJEKT2_PASSENGER_H

#include <thread>

class Passenger
{
public:
	Passenger(unsigned int rides = 1);
	
	Passenger(const Passenger &other) = delete;
	
	Passenger &operator=(const Passenger &) = delete;


private:
	unsigned int rides_left;
	
};


#endif //PROJEKT2_PASSENGER_H
