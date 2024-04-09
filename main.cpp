#include "Car.h"
#include "Rent.h"
#include <iostream>

using std::cin;
using std::cout;

int main() {
	Car car;
	Rent rent;
	int action, id, carId;
	//fill_db();
	do {
		cout << "Choose action:\n"
			"(1) Add car\n"
			"(2) Get car by id\n"
			"(3) Update car\n"
			"(4) Delete car\n"
			"(5) Add rent\n"
			"(6) Get rent by id\n"
			"(7) Update rent\n"
			"(8) Delete rent\n"
			"---------------\n"
			"(9) Display all cars\n"
			"(10) Display all rents\n"
			"(11) Fill database\n"
			"(0) Exit\n";
		cin >> action;

		switch (action)
		{
		case 0:
			return 0;
		case 1:
			car.input();
			car.insert_in_db();
			cout << "\nSuccessfully added! id: " << car.id << "\n\n";
			break;
		case 2:
			id = prompt_car_id();
			if (id == -1) break;
			car = Car::get_by_id(id);
			cout << "\nId: " << car.id 
				<< "\nModel: " << car.model_name 
				<< "\nColor: " << car.color
				<< "\nGearbox type: " << (car.is_automatic? 'A' : 'M')
				<< "\nEngine Volume: " << car.engine_volume
				<< "\n\n";
			break;
		case 3:
			id = prompt_car_id();
			if (id == -1) break;
			car = Car::get_by_id(id);
			car.input(true);
			car.update_db();
			cout << "\nSuccessfully updated!\n\n";
			break;
		case 4:
			id = prompt_car_id();
			if (id == -1) break;
			car = Car::get_by_id(id);
			car.remove();
			cout << "\nDone\n\n";
			break;
		case 5:
			rent.input();
			rent.insert_in_db();
			cout << "\nSuccessfully added! id: " << rent.id << "\n\n";
			break;
		case 6: 
			carId = prompt_car_id();
			if (carId == -1) break;
			id = prompt_rent_id();
			if (id == -1) break;
			rent = Rent::get_by_id(carId, id);
			if (rent.id == -1)
				cout << "\nRent with id " << id << " doesn't exist!\n\n";
			else
				cout << "\nStart Date: " << rent.startdate
				<< "\nFinish Date: " << rent.finishdate
				<< "\nPrice: " << rent.costPerDay << "\n\n";
			break;
		case 7: 
			carId = prompt_car_id();
			if (carId == -1) break;
			id = prompt_rent_id();
			if (id == -1) break;
			rent = Rent::get_by_id(carId, id);
			if (rent.id == -1) {
				cout << "\nRent with id " << id << " doesn't exist!\n\n";
				break;
			}
			rent.input(true);
			rent.update_db();
			cout << "\nSuccessfully updated!\n\n";
			break;
		case 8:
			carId = prompt_car_id();
			if (carId == -1) break;
			id = prompt_rent_id();
			if (id == -1) break;
			rent = Rent::get_by_id(carId, id);
			if (rent.id == -1) {
				cout << "\nRent with id " << id << " doesn't exist!\n\n";
				break;
			}
			rent.remove();
			cout << "\nSuccessfully deleted\n\n";
			break;
		case 9:
			Car::dispay_all();
			break;
		case 10:
			Rent::display_all();
			break;
		case 11:
			fill_db();
			cout << "\nDone\n\n";
			break;
		default:
			break;
		}

		system("pause");
	} while (true);

	return 0;
}