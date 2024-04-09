#pragma once

#define MAX_SIZE 20

struct Car {
	int id = 0;
	char model_name[MAX_SIZE];
	char color[MAX_SIZE];
	double engine_volume = 1.0;
	bool is_automatic = false;
	int rent_count = 0;
	int first_rent_address = -1;

	void input(bool isUpdate = false);
	void insert_in_db();
	void update_db();
	void remove();

	static Car get_by_id(const int& id);
	static void dispay_all();
};


