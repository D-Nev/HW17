#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>

class Room {
protected:
    int roomNumber;
    double pricePerNight;
    bool isBooked;

public:
    Room(int number, double price) : roomNumber(number), pricePerNight(price), isBooked(false) {}
    virtual ~Room() {}

    virtual std::string GetRoomType() const = 0;
    int GetRoomNumber() const { return roomNumber; }
    double GetPrice() const { return pricePerNight; }
    bool IsBooked() const { return isBooked; }

    void BookRoom() { isBooked = true; }
    void FreeRoom() { isBooked = false; }
};

class SingleRoom : public Room {
public:
    SingleRoom(int number, double price) : Room(number, price) {}
    std::string GetRoomType() const override { return "Одномісний"; }
};

class DoubleRoom : public Room {
public:
    DoubleRoom(int number, double price) : Room(number, price) {}
    std::string GetRoomType() const override { return "Двомісний"; }
};

class SuiteRoom : public Room {
    bool hasJacuzzi;
public:
    SuiteRoom(int number, double price, bool jacuzzi) : Room(number, price), hasJacuzzi(jacuzzi) {}
    std::string GetRoomType() const override { return "Люкс"; }
    bool HasJacuzzi() const { return hasJacuzzi; }
};

class HotelManager {
    std::vector<std::unique_ptr<Room>> rooms;
public:
    void AddRoom(std::unique_ptr<Room> room) {
        rooms.push_back(std::move(room));
    }

    bool BookRoom(int roomNumber) {
        auto it = std::find_if(rooms.begin(), rooms.end(), [roomNumber](const std::unique_ptr<Room>& r) {
            return r->GetRoomNumber() == roomNumber && !r->IsBooked();
            });
        if (it != rooms.end()) {
            (*it)->BookRoom();
            return true;
        }
        return false;
    }

    void FreeRoom(int roomNumber) {
        auto it = std::find_if(rooms.begin(), rooms.end(), [roomNumber](const std::unique_ptr<Room>& r) {
            return r->GetRoomNumber() == roomNumber;
            });
        if (it != rooms.end()) {
            (*it)->FreeRoom();
        }
    }

    void ShowAvailableRooms() const {
        for (const auto& room : rooms) {
            if (!room->IsBooked()) {
                std::cout << "Кімната " << room->GetRoomNumber() << " (" << room->GetRoomType()
                    << ") - Ціна: " << room->GetPrice() << " грн\n";
            }
        }
    }

    void SortRoomsByPrice() {
        std::sort(rooms.begin(), rooms.end(), [](const std::unique_ptr<Room>& a, const std::unique_ptr<Room>& b) {
            return a->GetPrice() < b->GetPrice();
            });
    }
};

class BookingSystem {
    std::list<std::pair<std::string, int>> reservations;
public:
    void MakeReservation(const std::string& clientName, int roomNumber) {
        reservations.emplace_back(clientName, roomNumber);
    }

    void CancelReservation(const std::string& clientName) {
        reservations.remove_if([&clientName](const std::pair<std::string, int>& r) {
            return r.first == clientName;
            });
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    HotelManager hotel;
    hotel.AddRoom(std::make_unique<SingleRoom>(101, 50.0));
    hotel.AddRoom(std::make_unique<DoubleRoom>(102, 75.0));
    hotel.AddRoom(std::make_unique<SuiteRoom>(201, 150.0, true));

    BookingSystem booking;
    hotel.ShowAvailableRooms();

    std::cout << "\nБронювання кімнати 102...\n";
    if (hotel.BookRoom(102)) {
        booking.MakeReservation("Джон Доу", 102);
    }

    hotel.ShowAvailableRooms();
    std::cout << "\nСортування кімнат за ціною...\n";
    hotel.SortRoomsByPrice();
    hotel.ShowAvailableRooms();

    return 0;
}
