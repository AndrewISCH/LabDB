#pragma once

#include "Car.h"
#include "Index.h"
#include "Rent.h"
#include <iostream>
#include <string>
#include <iomanip>

using std::string, std::cout, std::cin;

template<typename T> inline void fread_one(FILE* file, T* buffer);
template<typename T> inline void fwrite_one(FILE* file, T* buffer);

void save_fopen(FILE** f, const char* path, const char* mode);
void fopen_db(FILE** f, const char* mode);
void fopen_index(FILE** f, const char* mode);
void fopen_rent(FILE** f, const char* mode);

string input_field(const char* field, const bool& is_update, const char* default_value);
double input_field_volume(const char* field, const bool& is_update, double default_value);
int input_number(const char* field, const bool& is_update, const int& default_value);
string format_field(const char* field, const int& max_width);
template<typename T> inline void format_output(T t, const int& max_width, const bool& is_last = false);

inline void format_output_volume(double t, const int& max_width, const bool& is_last = false);
int get_address_by_id(const int& id);
int prompt_car_id();
int prompt_rent_id();
int get_max_car_id();
int get_max_rent_id();
void fill_db();

static const char* INDEX_PATH = "car.ind";
static const char* CARS_PATH = "car.fl";
static const char* RENT_PATH = "rent.fl";
static const size_t INDEX_SIZE = sizeof(Index);
static const size_t CAR_SIZE = sizeof(Car);

template<typename T> 
inline void fread_one(FILE* file, T* buffer) { fread(buffer, sizeof(T), 1, file); }

template<typename T> 
inline void fwrite_one(FILE* file, T* buffer) { fwrite(buffer, sizeof(T), 1, file); }

template<typename T>
inline void format_output(T t, const int& max_width, const bool& is_last) {
	cout << std::left << std::setw(max_width) << std::setfill(' ') << t;
	if (!is_last) cout << " | ";
}

inline void format_output_volume(double t, const int& max_width, const bool& is_last) {
	cout << std::left << std::setw(max_width) << std::setfill(' ');
	if (t) {
		cout << t;
	}
	else {
		cout << 'A';
	}
	if (!is_last) cout << " | ";
}



