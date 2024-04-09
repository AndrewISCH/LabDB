#include "Rent.h"
#include "Car.h"
#include "Garbage.h"
#include <string>

const size_t RENT_SIZE = sizeof(Rent);

int Rent::input(const bool& is_update) {
	if (!is_update) {
		int max_id = get_max_car_id();
		if (max_id == -1) {
			cout << "\nThere are no cars in the database!\n\n";
			return -1;
		}

		bool is_correct = false;
		while (!is_correct) {
			car_id = input_number("car's id", is_update, car_id);
			if (car_id > max_id || car_id < 0) {
				cout << "Car with such id doesn't exists!\n";
				continue;
			}
			is_correct = true;
		}
	}

	cin.ignore();
	strcpy_s(startdate, input_field("start date", is_update, startdate).c_str());
	strcpy_s(finishdate, input_field("finish date", is_update, startdate).c_str());
	costPerDay = input_number("price", is_update, costPerDay);
	return 1;
}

void Rent::insert_in_db() {
	FILE* rent_db;
	Rent temp;
	fopen_rent(&rent_db, "a+b");

	Car c = Car::get_by_id(car_id);
	int address;

	int deleted = get_first_deleted_rent();
	if (deleted != -1) {
		address = deleted;
		fseek(rent_db, address, SEEK_SET);
		fread_one(rent_db, &temp);
		id = temp.id;
		fclose(rent_db);
		fopen_rent(&rent_db, "r+b");
		fseek(rent_db, address, SEEK_SET);
		delete_from_rent_garbage(address);
	}
	else {
		fseek(rent_db, 0, SEEK_END);
		if (ftell(rent_db) != 0) {
			fseek(rent_db, -(int)RENT_SIZE, SEEK_END);
			fread_one(rent_db, &temp);
			id = temp.id + 1;
		}
		address = ftell(rent_db);
	}

	next = c.first_rent_address;
	exists = true;
	fwrite_one(rent_db, this);
	c.first_rent_address = address;
	c.rent_count++;
	c.update_db();
	fclose(rent_db);
}

Rent Rent::get_by_id(const int& car_id, const int& id) {
	FILE* rent_db;
	auto r = Car::get_by_id(car_id);
	Rent rent;
	fopen_rent(&rent_db, "rb");
	fseek(rent_db, r.first_rent_address, SEEK_SET);
	for (int i = 0; i < r.rent_count; i++) {
		fread_one(rent_db, &rent);
		if (rent.id == id) {
			fclose(rent_db);
			return rent;
		}
		fseek(rent_db, rent.next, SEEK_SET);
	}
	rent = Rent();
	rent.id = -1;
	fclose(rent_db);
	return rent;
}

bool Rent::update_db() {
	FILE* rent_db;
	Rent rent;
	fopen_rent(&rent_db, "r+b");
	auto r = Car::get_by_id(car_id);

	int prev;
	fseek(rent_db, r.first_rent_address, SEEK_SET);
	for (int i = 0; i < r.rent_count; i++) {
		prev = ftell(rent_db);
		fread_one(rent_db, &rent);
		if (rent.id == id) {
			fseek(rent_db, prev, SEEK_SET);
			fwrite_one(rent_db, this);
			fclose(rent_db);
			return true;
		}
		fseek(rent_db, rent.next, SEEK_SET);
	}
	fclose(rent_db);
	return false;
}

void Rent::remove() {
	exists = false;
	update_db();

	auto r = Car::get_by_id(car_id);
	int address = r.first_rent_address;

	FILE* rent_db;
	Rent temp, prev;
	fopen_rent(&rent_db, "r+b");

	fseek(rent_db, r.first_rent_address, SEEK_SET);
	fread_one(rent_db, &prev);

	if (prev.id != id) {
		for (int i = 1; i < r.rent_count; i++) {
			fseek(rent_db, prev.next, SEEK_SET);
			fread_one(rent_db, &temp);
			if (temp.id == id)
				break;
			prev = temp;
		}
		fclose(rent_db);
		address = prev.next;
		prev.next = next;
		prev.update_db();
	}
	else {
		r.first_rent_address = next;
		fclose(rent_db);
	}

	r.rent_count--;
	r.update_db();
	add_to_rent_garbage(address);
}

void Rent::display_all() {
	FILE* rent_db;
	Rent temp;
	fopen_rent(&rent_db, "rb");

	fseek(rent_db, 0, SEEK_END);
	int size = ftell(rent_db);
	fseek(rent_db, 0, SEEK_SET);

	const int num_width = 5;
	const int name_width = 13;

	cout << "\n\n";
	format_output("ID", num_width);
	format_output("CAR_ID", name_width);
	format_output("START DATE", name_width);
	format_output("FINISH DATE", name_width);
	format_output("COST/DAY", num_width, true);
	cout << "\n------+---------------+---------------+---------------+------------\n";
	while (ftell(rent_db) != size) {
		fread_one(rent_db, &temp);
		if (!temp.exists) continue;
		format_output(temp.id, num_width);
		format_output(temp.car_id, name_width);
		format_output(format_field(temp.startdate, name_width), name_width);
		format_output(format_field(temp.finishdate, name_width), name_width);
		format_output(temp.costPerDay, num_width, true);
		cout << "\n";
	}
	cout << "\n";
	fclose(rent_db);
}
