 #include <iostream>
#include <string>
#include "Car.h"
#include "Index.h"
#include "Utils.h"
#include "Rent.h"
#include "Garbage.h"

using std::cout;
using std::cin;
using std::string;

void Car::input(bool is_update) {
	cin.ignore();
	strcpy_s(this->model_name, input_field("model", is_update, this->model_name).c_str());
	strcpy_s(this->color, input_field("color", is_update, this->color).c_str());
	cout << "Enter the type of gearbox. 1 - automatic, 0 - mechanic\n";
    bool type;
	cin >> type;
	if (!type) {
		this->engine_volume = input_field_volume("engine volume", is_update, this->engine_volume);
	}
	else this->engine_volume = 0;
}

void Car::insert_in_db() {
	FILE* index_table, * db;
	Index index;

	fopen_index(&index_table, "a+b");
	fopen_db(&db, "a+b");

	int deleted = get_first_deleted_car();
	if (deleted != -1) {
		fseek(index_table, deleted, SEEK_SET);
		fread_one(index_table, &index);
		id = index.id;
		fclose(db);
		fclose(index_table);
		fopen_index(&index_table, "r+b");
		fopen_db(&db, "r+b");
		fseek(index_table, deleted, SEEK_SET);
		fseek(db, index.address, SEEK_SET);
		delete_from_car_garbage(deleted);
	}
	else {
		fseek(index_table, 0, SEEK_END);
		if (ftell(index_table) != 0) {
			fseek(index_table, -(int)INDEX_SIZE, SEEK_END);
			fread_one(index_table, &index);
			id = index.id + 1;
		}
		index.id = id;
		index.address = id * CAR_SIZE;
	}

	fwrite_one(db, this);

	index.exist = true;
	fwrite_one(index_table, &index);

	fclose(index_table);
	fclose(db);
}

void Car::update_db() {
	FILE* db;
	fopen_db(&db, "r+b");
	fseek(db, get_address_by_id(id), SEEK_SET);
	fwrite_one(db, this);
	fclose(db);
}

Car Car::get_by_id(const int& id) {
	FILE* db;
	Car r;

	fopen_db(&db, "rb");
	fseek(db, get_address_by_id(id), SEEK_SET);
	fread_one(db, &r);

	fclose(db);
	return r;
}

void Car::dispay_all() {
	FILE* db, * index_table;
	Car temp;
	Index index;
	fopen_db(&db, "rb");
	fopen_index(&index_table, "rb");

	fseek(index_table, 0, SEEK_END);
	int size = ftell(index_table);
	fseek(index_table, 0, SEEK_SET);

	const int num_width = 5;
	const int name_width = 13;
	const int model_width = 20;

	cout << "\n\n";
	format_output("ID", num_width);
	format_output("MODEL", model_width);
	format_output("COLOR", name_width);
	format_output("VOL", num_width);
	format_output("RENTS", num_width, true);
	cout << "\n------+----------------------+---------------+-------+-------\n";
	while (ftell(index_table) != size) {
		fread_one(index_table, &index);
		if (!index.exist) continue;
		fseek(db, index.address, SEEK_SET);
		fread_one(db, &temp);
		format_output(temp.id, num_width);
		format_output(format_field(temp.model_name, model_width), model_width);
		format_output(format_field(temp.color, name_width), name_width);
		format_output_volume(temp.engine_volume, num_width);
		format_output(temp.rent_count, num_width, true);
		cout << "\n";
	}
	cout << "\n";
	fclose(db);
	fclose(index_table);
}

void Car::remove() {
	if (rent_count != 0) {
		Rent temp;
		FILE* dish_db;
		fopen_rent(&dish_db, "r+b");
		fseek(dish_db, first_rent_address, SEEK_SET);
		for (int i = 0; i < rent_count; i++) {
			fread_one(dish_db, &temp);
			fclose(dish_db);
			temp.remove();
			fopen_rent(&dish_db, "r+b");
			fseek(dish_db, temp.next, SEEK_SET);
		}
		first_rent_address = -1;
		rent_count = 0;
		fclose(dish_db);
	}

	FILE* index_table;
	Index index;

	fopen_index(&index_table, "r+b");
	fseek(index_table, id * INDEX_SIZE, SEEK_SET);
	fread_one(index_table, &index);
	index.exist = false;
	fseek(index_table, id * INDEX_SIZE, SEEK_SET);
	fwrite_one(index_table, &index);
	fclose(index_table);

	add_to_car_garbage(id * INDEX_SIZE);
}