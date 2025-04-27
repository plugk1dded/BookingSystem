#include "user.h"
#include "event.h"
#include "ticket.h"
#include "bookingsystem.h"
#include <fstream>
#include <sstream>

User::User(int _id, const std::string& _name, const std::string& _email, const std::string& _phone)
    : IIdentifiable(_id), name(_name), email(_email), phone(_phone) {
}

bool User::bookTicket(std::shared_ptr<Event> event) {
    if (event->getAvailableSeats() <= 0) {
        return false;
    }

    auto ticket = event->createTicket(shared_from_this());
    return ticket != nullptr;
}

bool User::cancelTicket(int ticketId) {
    return BookingSystem::getInstance().cancelTicket(ticketId);
}

void User::addTicket(std::shared_ptr<Ticket> ticket) {
    tickets.push_back(ticket);
}

void User::removeTicket(int ticketId) {
    tickets.erase(
        std::remove_if(tickets.begin(), tickets.end(),
            [ticketId](const std::shared_ptr<Ticket>& t) {
                return t->getId() == ticketId;
            }
        ),
        tickets.end()
    );
}

void User::display() const {
    std::cout << "Пользователь ID: " << id << "\n";
    std::cout << "Имя: " << name << "\n";
    std::cout << "Email: " << email << "\n";
    std::cout << "Телефон: " << phone << "\n";

    if (!tickets.empty()) {
        std::cout << "Билеты: " << tickets.size() << " шт.\n";
    }
}

void User::saveToFile() const {
    bool exists = false;
    int lineToReplace = -1;
    std::string line;
    std::ifstream inFile("users.txt");
    std::vector<std::string> lines;

    if (inFile.is_open()) {
        int lineNum = 0;
        while (std::getline(inFile, line)) {
            lines.push_back(line);
            std::istringstream iss(line);
            int userId;
            iss >> userId;
            if (userId == id) {
                exists = true;
                lineToReplace = lineNum;
            }
            lineNum++;
        }
        inFile.close();
    }

    if (exists) {
        std::ostringstream newLine;
        newLine << id << "\t" << name << "\t" << email << "\t" << phone;
        lines[lineToReplace] = newLine.str();

        std::ofstream outFile("users.txt", std::ios::trunc);
        if (outFile.is_open()) {
            for (const auto& l : lines) {
                outFile << l << std::endl;
            }
            outFile.close();
        }
    }
    else {
        std::ofstream file("users.txt", std::ios::app);
        if (file.is_open()) {
            file << id << "\t" << name << "\t" << email << "\t" << phone << std::endl;
            file.close();
        }
    }
}
