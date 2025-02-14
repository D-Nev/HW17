#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
using namespace std;

class Vehicle {
protected:
    string brand, model;
    int year;
public:
    Vehicle(string b, string m, int y) : brand(move(b)), model(move(m)), year(y) {}
    virtual ~Vehicle() = default;
    virtual void DisplayInfo() const {
        cout << "Марка: " << brand << ", Модель: " << model << ", Рік: " << year;
    }
    string GetBrand() const { return brand; }
};

class Car : public Vehicle {
    int numberOfDoors;
    string fuelType;
public:
    Car(string b, string m, int y, int doors, string fuel)
        : Vehicle(move(b), move(m), y), numberOfDoors(doors), fuelType(move(fuel)) {
    }
    void DisplayInfo() const override {
        Vehicle::DisplayInfo();
        cout << ", Двері: " << numberOfDoors << ", Паливо: " << fuelType << endl;
    }
};

class Truck : public Vehicle {
    int loadCapacity, axleCount;
public:
    Truck(string b, string m, int y, int load, int axles)
        : Vehicle(move(b), move(m), y), loadCapacity(load), axleCount(axles) {
    }
    void DisplayInfo() const override {
        Vehicle::DisplayInfo();
        cout << ", Вантажопідйомність: " << loadCapacity << " т, Осей: " << axleCount << endl;
    }
};

class Motorcycle : public Vehicle {
    int engineVolume;
public:
    Motorcycle(string b, string m, int y, int volume)
        : Vehicle(move(b), move(m), y), engineVolume(volume) {
    }
    void DisplayInfo() const override {
        Vehicle::DisplayInfo();
        cout << ", Об'єм двигуна: " << engineVolume << " см³" << endl;
    }
};

class FleetManager {
    vector<unique_ptr<Vehicle>> vehicles;
public:
    void AddVehicle(unique_ptr<Vehicle> v) {
        vehicles.push_back(move(v));
    }
    void RemoveVehicle(const string& brand) {
        vehicles.erase(remove_if(vehicles.begin(), vehicles.end(), [&](const unique_ptr<Vehicle>& v) {
            return v->GetBrand() == brand;
            }), vehicles.end());
    }
    void ShowAllVehicles() const {
        for (const auto& v : vehicles) {
            v->DisplayInfo();
        }
    }
    void FindVehicleByBrand(const string& brand) const {
        for (const auto& v : vehicles) {
            if (v->GetBrand() == brand) {
                v->DisplayInfo();
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    FleetManager fleet;
    fleet.AddVehicle(make_unique<Car>("Toyota", "Camry", 2020, 4, "Бензин"));
    fleet.AddVehicle(make_unique<Truck>("Volvo", "FH16", 2018, 20, 3));
    fleet.AddVehicle(make_unique<Motorcycle>("Yamaha", "R1", 2022, 998));

    cout << "Всі транспортні засоби:\n";
    fleet.ShowAllVehicles();

    cout << "\nПошук за маркою 'Toyota':\n";
    fleet.FindVehicleByBrand("Toyota");

    return 0;
}
