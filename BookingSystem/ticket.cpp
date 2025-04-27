#include "ticket.h"
#include <fstream>
#include <sstream>
#include <vector>

Ticket::Ticket(int _id, int _eventId, int _userId, double _price)
    : IIdentifiable(_id), eventId(_eventId), userId(_userId), price(_price), isActive(true) {
    DateTime now = DateTime::now();
    bookingTime = now.toString();
}

void Ticket::display() const {
    std::cout << "Билет ID: " << id << "\n";
    std::cout << "Событие ID: " << eventId << "\n";
    std::cout << "Пользователь ID: " << userId << "\n";
    std::cout << "Цена: " << price << " руб.\n";
    std::cout << "Время бронирования: " << bookingTime << "\n";
    std::cout << "Статус: " << (isActive ? "Активен" : "Отменен") << "\n";
}

void Ticket::saveToFile() const {
    bool exists = false;
    int lineToReplace = -1;
    std::string line;
    std::ifstream inFile("tickets.txt");
    std::vector<std::string> lines;

    if (inFile.is_open()) {
        int lineNum = 0;
        while (std::getline(inFile, line)) {
            lines.push_back(line);
            std::istringstream iss(line);
            int ticketId;
            iss >> ticketId;
            if (ticketId == id) {
                exists = true;
                lineToReplace = lineNum;
            }
            lineNum++;
        }
        inFile.close();
    }

    if (exists) {
        std::ostringstream newLine;
        newLine << id << "\t" << eventId << "\t" << userId << "\t"
            << price << "\t" << bookingTime << "\t" << (isActive ? "active" : "canceled");
        lines[lineToReplace] = newLine.str();

        std::ofstream outFile("tickets.txt", std::ios::trunc);
        if (outFile.is_open()) {
            for (const auto& l : lines) {
                outFile << l << std::endl;
            }
            outFile.close();
        }
    }
    else {
        std::ofstream file("tickets.txt", std::ios::app);
        if (file.is_open()) {
            file << id << "\t" << eventId << "\t" << userId << "\t"
                << price << "\t" << bookingTime << "\t" << (isActive ? "active" : "canceled") << std::endl;
            file.close();
        }
    }
}