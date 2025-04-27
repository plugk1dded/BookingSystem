#include "event.h"
#include "user.h"
#include "ticket.h"
#include "bookingsystem.h"
#include <fstream>
#include <sstream>

Event::Event(int _id, const std::string& _name, const std::string& _date,
    const std::string& _venue, int _totalSeats, double _basePrice,
    const std::string& _description, const std::string& _category)
    : IIdentifiable(_id), name(_name), eventDate(_date), venue(_venue),
    totalSeats(_totalSeats), availableSeats(_totalSeats), basePrice(_basePrice),
    description(_description), category(_category) {
}

bool Event::isExpired() const {
    return eventDate < DateTime::now();
}

double Event::calculateTicketPrice() const {
    return basePrice;
}

std::shared_ptr<Ticket> Event::createTicket(std::shared_ptr<User> user) {
    return BookingSystem::getInstance().createTicket(
        std::dynamic_pointer_cast<Event>(shared_from_this()), user);
}

void Event::display() const {
    std::cout << "Событие ID: " << id << "\n";
    std::cout << "Название: " << name << "\n";
    std::cout << "Дата: " << eventDate.toDateString() << "\n";
    std::cout << "Место проведения: " << venue << "\n";
    std::cout << "Доступно мест: " << availableSeats << " из " << totalSeats << "\n";
    std::cout << "Базовая цена: " << basePrice << " руб.\n";
    if (!description.empty()) {
        std::cout << "Описание: " << description << "\n";
    }
    std::cout << "Категория: " << category << "\n";
    std::cout << "Статус: " << (isExpired() ? "Прошедшее" : "Предстоящее") << "\n";
}

void Event::decreaseAvailableSeats() {
    if (availableSeats > 0) {
        availableSeats--;
    }
}

void Event::increaseAvailableSeats() {
    if (availableSeats < totalSeats) {
        availableSeats++;
    }
}

void Event::saveToFile() const {
    bool exists = false;
    int lineToReplace = -1;
    std::string line;
    std::ifstream inFile("events.txt");
    std::vector<std::string> lines;

    if (inFile.is_open()) {
        int lineNum = 0;
        while (std::getline(inFile, line)) {
            lines.push_back(line);
            std::istringstream iss(line);
            std::string type;
            int eventId;
            iss >> type >> eventId;
            if (type == "Event" && eventId == id) {
                exists = true;
                lineToReplace = lineNum;
            }
            lineNum++;
        }
        inFile.close();
    }

    if (exists) {
        std::ostringstream newLine;
        newLine << "Event\t" << id << "\t" << name << "\t" << eventDate.toDateString() << "\t"
            << venue << "\t" << totalSeats << "\t" << availableSeats << "\t"
            << basePrice << "\t" << description << "\t" << category;
        lines[lineToReplace] = newLine.str();

        std::ofstream outFile("events.txt", std::ios::trunc);
        if (outFile.is_open()) {
            for (const auto& l : lines) {
                outFile << l << std::endl;
            }
            outFile.close();
        }
    }
    else {
        std::ofstream file("events.txt", std::ios::app);
        if (file.is_open()) {
            file << "Event\t" << id << "\t" << name << "\t" << eventDate.toDateString() << "\t"
                << venue << "\t" << totalSeats << "\t" << availableSeats << "\t"
                << basePrice << "\t" << description << "\t" << category << std::endl;
            file.close();
        }
    }
}

Concert::Concert(int _id, const std::string& _name, const std::string& _date,
    const std::string& _venue, int _totalSeats, double _basePrice,
    const std::string& _artist, const std::string& _genre,
    int _duration, const std::string& _description,
    const std::string& _category)
    : Event(_id, _name, _date, _venue, _totalSeats, _basePrice, _description, _category),
    artist(_artist), genre(_genre), duration(_duration) {
}

double Concert::calculateTicketPrice() const {
    double price = basePrice * 1.1;

    DateTime dt = getEventDate();
    time_t t = time(nullptr);
    struct tm timeinfo;

    localtime_s(&timeinfo, &t);

    int weekday = timeinfo.tm_wday;
    if (weekday == 0 || weekday == 5 || weekday == 6) {
        price *= 1.05;
    }

    return price;
}

void Concert::display() const {
    Event::display();
    std::cout << "Исполнитель: " << artist << "\n";
    std::cout << "Жанр: " << genre << "\n";
    std::cout << "Продолжительность: " << duration << " мин.\n";
}

void Concert::saveToFile() const {
    bool exists = false;
    int lineToReplace = -1;
    std::string line;
    std::ifstream inFile("concerts.txt");
    std::vector<std::string> lines;

    if (inFile.is_open()) {
        int lineNum = 0;
        while (std::getline(inFile, line)) {
            lines.push_back(line);
            std::istringstream iss(line);
            int eventId;
            iss >> eventId;
            if (eventId == id) {
                exists = true;
                lineToReplace = lineNum;
            }
            lineNum++;
        }
        inFile.close();
    }

    if (exists) {
        std::ostringstream newLine;
        newLine << id << "\t" << name << "\t" << eventDate.toDateString() << "\t" << venue << "\t"
            << totalSeats << "\t" << availableSeats << "\t" << basePrice << "\t"
            << artist << "\t" << genre << "\t" << duration << "\t"
            << description << "\t" << category;
        lines[lineToReplace] = newLine.str();

        std::ofstream outFile("concerts.txt", std::ios::trunc);
        if (outFile.is_open()) {
            for (const auto& l : lines) {
                outFile << l << std::endl;
            }
            outFile.close();
        }
    }
    else {
        std::ofstream file("concerts.txt", std::ios::app);
        if (file.is_open()) {
            file << id << "\t" << name << "\t" << eventDate.toDateString() << "\t" << venue << "\t"
                << totalSeats << "\t" << availableSeats << "\t" << basePrice << "\t"
                << artist << "\t" << genre << "\t" << duration << "\t"
                << description << "\t" << category << std::endl;
            file.close();
        }
    }

    Event::saveToFile();
}

TheatrePlay::TheatrePlay(int _id, const std::string& _name, const std::string& _date,
    const std::string& _venue, int _totalSeats, double _basePrice,
    const std::string& _director, const std::string& _genre,
    int _duration, int _ageLimit,
    const std::string& _description, const std::string& _category)
    : Event(_id, _name, _date, _venue, _totalSeats, _basePrice, _description, _category),
    director(_director), genre(_genre), duration(_duration), ageLimit(_ageLimit) {
}

double TheatrePlay::calculateTicketPrice() const {
    double price = basePrice * 1.05;

    if (ageLimit >= 18) {
        price *= 1.1;
    }

    return price;
}

void TheatrePlay::display() const {
    Event::display();
    std::cout << "Режиссер: " << director << "\n";
    std::cout << "Жанр: " << genre << "\n";
    std::cout << "Продолжительность: " << duration << " мин.\n";
    std::cout << "Возрастное ограничение: " << (ageLimit > 0 ? std::to_string(ageLimit) + "+" : "Без ограничений") << "\n";
}

void TheatrePlay::saveToFile() const {
    bool exists = false;
    int lineToReplace = -1;
    std::string line;
    std::ifstream inFile("theatreplays.txt");
    std::vector<std::string> lines;

    if (inFile.is_open()) {
        int lineNum = 0;
        while (std::getline(inFile, line)) {
            lines.push_back(line);
            std::istringstream iss(line);
            int eventId;
            iss >> eventId;
            if (eventId == id) {
                exists = true;
                lineToReplace = lineNum;
            }
            lineNum++;
        }
        inFile.close();
    }

    if (exists) {
        std::ostringstream newLine;
        newLine << id << "\t" << name << "\t" << eventDate.toDateString() << "\t" << venue << "\t"
            << totalSeats << "\t" << availableSeats << "\t" << basePrice << "\t"
            << director << "\t" << genre << "\t" << duration << "\t" << ageLimit << "\t"
            << description << "\t" << category;
        lines[lineToReplace] = newLine.str();

        std::ofstream outFile("theatreplays.txt", std::ios::trunc);
        if (outFile.is_open()) {
            for (const auto& l : lines) {
                outFile << l << std::endl;
            }
            outFile.close();
        }
    }
    else {
        std::ofstream file("theatreplays.txt", std::ios::app);
        if (file.is_open()) {
            file << id << "\t" << name << "\t" << eventDate.toDateString() << "\t" << venue << "\t"
                << totalSeats << "\t" << availableSeats << "\t" << basePrice << "\t"
                << director << "\t" << genre << "\t" << duration << "\t" << ageLimit << "\t"
                << description << "\t" << category << std::endl;
            file.close();
        }
    }

    Event::saveToFile();
}
