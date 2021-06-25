//
// Created by Maciej Ciura on 14.06.2021.
//

#include "Renderer.h"
#include <curses.h>
#include <mutex>

std::mutex Renderer::print_mutex_;

double Renderer::window_height_ = 0;
double Renderer::window_width_ = 0;

MapView             Renderer::map_view_;
std::shared_ptr<Map> Renderer::map_;


void Renderer::initialize(std::shared_ptr<Map> &map)
{
	map_ = map;
	
	initscr();
	keypad(stdscr, TRUE);
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, TRUE);
	
	update_dimensions();
}

void Renderer::draw_screen()
{
	clear();
	draw_map();
	draw_trains();
	print_trains_states();
	refresh();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
}

void Renderer::update_dimensions()
{
	getmaxyx(stdscr, Renderer::window_height_, Renderer::window_width_);
	map_view_.map_height_ = window_width_ - window_width_ * 0.95;
	map_view_.map_width_ = window_height_ - window_height_ * 0.95;
	map_view_.border.x1 = window_width_ * 0.015;
	map_view_.border.y1 = window_height_ * 0.05;
	map_view_.border.x2 = window_width_ * 0.985;
	map_view_.border.y2 = window_height_ * 0.95;
	
	
	for (unsigned int i = 0; i < map_->get_stations().size(); i++)
	{
		map_->get_stations()[i]->set_position(station_positions[i][0] * window_width_,
											  station_positions[i][1] * window_height_);
	}
	
}

void Renderer::draw_map()
{
	draw_rectangle(map_view_.border.x1, map_view_.border.y1, map_view_.border.x2, map_view_.border.y2);
	draw_stations();
	draw_tracks();
}

void Renderer::draw_rectangle(int x1, int y1, int x2, int y2)
{
	mvhline(y1, x1, 0, x2 - x1);
	mvhline(y2, x1, 0, x2 - x1);
	mvvline(y1, x1, 0, y2 - y1);
	mvvline(y1, x2, 0, y2 - y1);
	mvaddch(y1, x1, ACS_ULCORNER);
	mvaddch(y2, x1, ACS_LLCORNER);
	mvaddch(y1, x2, ACS_URCORNER);
	mvaddch(y2, x2, ACS_LRCORNER);
}


void Renderer::draw_stations()
{
	for (const auto &i : map_->get_stations())
		draw_station(i);
}

void Renderer::draw_station(const std::shared_ptr<Station> &station)
{
	draw_rectangle(station->get_x() - STATION_WIDTH, station->get_y() - STATION_WIDTH / 2,
				   station->get_x() + STATION_WIDTH, station->get_y() + STATION_WIDTH / 2);
	for (int i = 0; i < station->get_passengers().size(); i++)
		printw("P");
	
	move(station->get_y() - STATION_WIDTH / 2 + 1, station->get_x() - STATION_WIDTH + 1);
	printw("Station %d", station->id_);
	move(station->get_y() - STATION_WIDTH / 2 + 2, station->get_x() - STATION_WIDTH + 1);
	printw("Passengers: %d ", station->get_passengers().size());
	move(station->get_y() + STATION_WIDTH / 2 - 1, station->get_x() - STATION_WIDTH + 1);
	for (const auto &train : station->get_trains())
		printw("%d ", train->id_);
	
}

void Renderer::draw_tracks()
{
	for (const auto &track : map_->get_tracks())
	{
		mvvline(track->to_.lock()->get_y()   + STATION_WIDTH / 2 + 1, track->from_.lock()->get_x(), ACS_VLINE,
				track->from_.lock()->get_y() - track->to_.lock()->get_y() - STATION_WIDTH - 1);
		
		mvhline(track->from_.lock()->get_y(), track->from_.lock()->get_x() + STATION_WIDTH + 1, ACS_HLINE,
				track->to_.lock()->get_x()   -   track->from_.lock()->get_x() - STATION_WIDTH * 2);
	}
}

void Renderer::draw_trains()
{
	for (const auto &train : map_->get_trains())
		draw_train(train);
}

void Renderer::draw_train(const std::shared_ptr<Train> &train)
{
	if (!train->on_station())
	{
		mvaddch(train->get_station().lock()->get_y() +
				(train->get_destination().lock()->get_y() - train->get_station().lock()->get_y()) *
				train->get_dist_pct(),
				train->get_station().lock()->get_x() +
				(train->get_destination().lock()->get_x() - train->get_station().lock()->get_x()) *
				train->get_dist_pct(), train->id_ + 48);
	}
	
}

void Renderer::print_trains_states()
{
	int i = 0;
	move(map_view_.border.y1 + 1 + i++, map_view_.border.x1 + 1);
	printw("Trains:");
	move(map_view_.border.y1 + 1 + i++, map_view_.border.x1 + 1);
	printw("|Id\t| Passengers \t| State ");
	for (const auto &train : map_->get_trains())
	{
		move(map_view_.border.y1 + 1 + i++, map_view_.border.x1 + 1);
		printw("| %d\t| %d\t\t| %s", train->id_, train->get_passengers().size(), train->get_state().c_str());
	}
}
