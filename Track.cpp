//
// Created by Maciej Ciura on 08.06.2021.
//

#include "Track.h"
#include "Station.h"

Track::Track(const std::shared_ptr<Station>& from, const std::shared_ptr<Station>& to)
	: from_(from), to_(to), free_(true)
{
}

int Track::get_x() const
{
	return from_.lock()->get_x();
}

