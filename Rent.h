#pragma once

#include "Utils.h"
#include "Car.h"

struct Rent {
	int id = 0;
	int car_id = -1;
	char startdate[MAX_SIZE];
	char finishdate[MAX_SIZE];
	int costPerDay = 0;
	int next = -1;
	bool exists = true;

	int input(const bool& is_update = false);
	void insert_in_db();
	bool update_db();
	void remove();

	static Rent get_by_id(const int& restaurant_id, const int& id);
	static void display_all();
};

