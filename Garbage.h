#pragma once

void add_to_garbage(const int& address, const char* path);
int get_first_deleted(const char* path);
void delete_from_garbage(const int& address, const char* path);

void add_to_rent_garbage(const int& address);
int get_first_deleted_rent();
void delete_from_rent_garbage(const int& address);

void add_to_car_garbage(const int& address);
int get_first_deleted_car();
void delete_from_car_garbage(const int& address);