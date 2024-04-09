#include "Utils.h"

void save_fopen(FILE** f, const char* path, const char* mode) {
	if (fopen_s(f, path, mode) != 0)
		throw "error";
}

void fopen_db(FILE** f, const char* mode) { save_fopen(f, CARS_PATH, mode); }
void fopen_index(FILE** f, const char* mode) { save_fopen(f, INDEX_PATH, mode); }
void fopen_rent(FILE** f, const char* mode) { save_fopen(f, RENT_PATH, mode); }

string input_field(const char* field, const bool& is_update, const char* default_value) {
	string prompt = is_update ? " (Blank line to skip): " : ": ";
	string input;

	do {
		cout << "Enter " << field << prompt;
		std::getline(cin, input);
		if (!input.empty())
			return input.substr(0, MAX_SIZE);
		if (!is_update) cout << field << " can't be empty!\n";
	} while (!is_update);
	return string(default_value);
}

double input_field_volume(const char* field, const bool& is_update, double default_value) {
	string prompt = is_update ? " ('-' to skip): " : ": ";
	string input;

	do {
		cout << "Enter " << field << prompt;
		cin >> input;
		if ( stod(input) >= 0)
			return stod(input);
		if (!is_update) cout << field << " can't be empty!\n";
	} while (!is_update);
	return default_value;
}

int input_number(const char* field, const bool& is_update, const int& default_value) {
	string prompt = is_update ? " ('-' to skip): " : ": ";
	int input;
	do {
		cout << "Enter " << field << prompt;
		if (cin >> input && input >= 0)
			return input;
		if (input >= 0) {
			cin.clear();
			string discard;
			getline(cin, discard);
		}
		if (!is_update || input < 0) cout << "Wrong number!\n";
	} while (!is_update);

	return default_value;
}

string format_field(const char* field, const int& max_width) {
	string result = string(field);
	result = result.size() > max_width ? result.substr(0, max_width - 3) + "..." : result;
	return result;
}

int get_address_by_id(const int& id) {
	FILE* index_table;
	Index index;

	fopen_index(&index_table, "rb");
	fseek(index_table, id * INDEX_SIZE, SEEK_SET);
	fread_one(index_table, &index);

	fclose(index_table);
	return index.address;
}

int get_max_car_id() {
	FILE* index_table;
	Index index;
	fopen_index(&index_table, "rb");
	fseek(index_table, 0, SEEK_END);
	if (ftell(index_table) == 0)
		return -1;

	fseek(index_table, -(int)INDEX_SIZE, SEEK_END);
	fread_one(index_table, &index);
	fclose(index_table);
	return index.id;
}

int prompt_car_id() {
	int max_id = get_max_car_id();
	if (max_id == -1) {
		cout << "\nThere are no restaurants in the database!\n\n";
		return -1;
	}

	FILE* index_table;
	Index index;
	fopen_index(&index_table, "rb");

	int id;
	bool is_correct = false;
	while (!is_correct) {
		cout << "Enter car's id: ";
		if (!(cin >> id) || id < 0) continue;
		if (id > max_id) {
			cout << "Car with such id doesn't exists!\n";
			continue;
		}

		fseek(index_table, id * INDEX_SIZE, SEEK_SET);
		fread_one(index_table, &index);
		if (!index.exist)
			cout << "Car with such id doesn't exists!\n";
		else
			is_correct = true;
	}
	fclose(index_table);
	return id;
}

int get_max_rent_id() {
	FILE* rent_db;
	Rent rent;
	fopen_rent(&rent_db, "rb");
	fseek(rent_db, 0, SEEK_END);
	if (ftell(rent_db) == 0)
		return -1;

	fseek(rent_db, -(int)sizeof(Rent), SEEK_END);
	fread_one(rent_db, &rent);
	fclose(rent_db);
	return rent.id;
}

int prompt_rent_id() {
	int max_id = get_max_rent_id();
	if (max_id == -1) {
		cout << "\nThere are no rents in the database!\n\n";
		return -1;
	}

	FILE* rent_db;
	Rent rent;
	fopen_rent(&rent_db, "rb");

	int id;
	bool is_correct = false;
	while (!is_correct) {
		cout << "Enter rent's id: ";
		if (!(cin >> id) || id < 0) continue;
		if (id > max_id) {
			cout << "Rent with such id doesn't exists!\n";
			continue;
		}

		fseek(rent_db, id * sizeof(Rent), SEEK_SET);
		fread_one(rent_db, &rent);
		if (!rent.exists)
			cout << "Rent with such id doesn't exists!\n";
		else
			is_correct = true;
	}
	fclose(rent_db);
	return id;
}

void fill_db() {
	FILE* db;
	FILE* index_table;
	fopen_db(&db, "w+b");
	fclose(db);
	fopen_rent(&db, "w+b");
	fclose(db);
	fopen_index(&index_table, "w+b");
	fclose(index_table);
	int n = sizeof(Car);
	Car r1 = { 0, "BMW i7", "black", 0, 1 },
		r2 = { 1, "Honda Civic", "white", 1.5, 0 },
		r3 = { 2, "Renault Logan", "yellow", 1.6, 0 };
	r1.insert_in_db();
	r2.insert_in_db();
	r3.insert_in_db();

	Rent d1 = { 0, 0, "23/03/2024", "24/03/2024" ,700 },
		d2 = { 1, 1, "19/02/2019", "20/03/2019", 450 },
		d3 = { 2, 2, "12/05/2021","12/06/2021", 500 },
		d4 = { 3, 0, "05/05/2015", "04/07/2016", 250 };
	
	d1.insert_in_db();
	d2.insert_in_db();
	d3.insert_in_db();
	d4.insert_in_db();

}