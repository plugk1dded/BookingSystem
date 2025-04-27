#include "bookingsystem.h"
#include <fstream>
#include <sstream>
#include <numeric>

BookingSystem* BookingSystem::instance = nullptr;

BookingSystem::BookingSystem() : dataDirectory("./") {
    system(("mkdir " + dataDirectory + " 2>nul").c_str());
}

BookingSystem& BookingSystem::getInstance() {
    if (!instance) {
        instance = new BookingSystem();
    }
    return *instance;
}

void BookingSystem::destroy() {
    delete instance;
    instance = nullptr;
}

std::shared_ptr<Concert> BookingSystem::createConcert(
    const std::string& name, const std::string& date, const std::string& venue,
    int totalSeats, double basePrice, const std::string& artist, const std::string& genre,
    int duration, const std::string& description, const std::string& category) {

    auto concert = std::make_shared<Concert>(
        nextEventId++, name, date, venue, totalSeats, basePrice,
        artist, genre, duration, description, category
    );
    events.push_back(concert);
    concert->saveToFile();
    return concert;
}

std::shared_ptr<TheatrePlay> BookingSystem::createTheatrePlay(
    const std::string& name, const std::string& date, const std::string& venue,
    int totalSeats, double basePrice, const std::string& director, const std::string& genre,
    int duration, int ageLimit, const std::string& description, const std::string& category) {

    auto play = std::make_shared<TheatrePlay>(
        nextEventId++, name, date, venue, totalSeats, basePrice,
        director, genre, duration, ageLimit, description, category
    );
    events.push_back(play);
    play->saveToFile();
    return play;
}

std::shared_ptr<User> BookingSystem::createUser(
    const std::string& name, const std::string& email, const std::string& phone) {

    auto user = std::make_shared<User>(nextUserId++, name, email, phone);
    users.push_back(user);
    user->saveToFile();
    return user;
}

std::shared_ptr<Ticket> BookingSystem::createTicket(
    std::shared_ptr<Event> event, std::shared_ptr<User> user) {

    if (event->getAvailableSeats() <= 0) {
        std::cout << "Ошибка: нет доступных мест для события " << event->getName() << std::endl;
        return nullptr;
    }

    double price = event->calculateTicketPrice();
    auto ticket = std::make_shared<Ticket>(nextTicketId++, event->getId(), user->getId(), price);
    tickets.push_back(ticket);
    user->addTicket(ticket);
    event->decreaseAvailableSeats();
    ticket->saveToFile();

    event->saveToFile();

    return ticket;
}

bool BookingSystem::cancelTicket(int ticketId) {
    auto ticketIt = std::find_if(tickets.begin(), tickets.end(),
        [ticketId](const std::shared_ptr<Ticket>& t) {
            return t->getId() == ticketId;
        });

    if (ticketIt == tickets.end() || !(*ticketIt)->getIsActive()) {
        return false;
    }

    (*ticketIt)->setIsActive(false);

    auto eventIt = std::find_if(events.begin(), events.end(),
        [&ticketIt](const std::shared_ptr<Event>& e) {
            return e->getId() == (*ticketIt)->getEventId();
        });

    if (eventIt != events.end()) {
        (*eventIt)->increaseAvailableSeats();
        (*eventIt)->saveToFile();
    }

    auto userIt = std::find_if(users.begin(), users.end(),
        [&ticketIt](const std::shared_ptr<User>& u) {
            return u->getId() == (*ticketIt)->getUserId();
        });

    if (userIt != users.end()) {
        (*userIt)->removeTicket(ticketId);
    }

    (*ticketIt)->saveToFile();

    return true;
}

std::shared_ptr<Event> BookingSystem::findEventById(int id) {
    auto it = std::find_if(events.begin(), events.end(),
        [id](const std::shared_ptr<Event>& e) {
            return e->getId() == id;
        });
    return (it != events.end()) ? *it : nullptr;
}

std::shared_ptr<User> BookingSystem::findUserById(int id) {
    auto it = std::find_if(users.begin(), users.end(),
        [id](const std::shared_ptr<User>& u) {
            return u->getId() == id;
        });
    return (it != users.end()) ? *it : nullptr;
}

std::shared_ptr<Ticket> BookingSystem::findTicketById(int id) {
    auto it = std::find_if(tickets.begin(), tickets.end(),
        [id](const std::shared_ptr<Ticket>& t) {
            return t->getId() == id;
        });
    return (it != tickets.end()) ? *it : nullptr;
}

std::vector<std::shared_ptr<Event>> BookingSystem::findEventsByName(const std::string& nameSubstr) {
    std::vector<std::shared_ptr<Event>> result;

    for (const auto& event : events) {
        if (event->getName().find(nameSubstr) != std::string::npos) {
            result.push_back(event);
        }
    }

    return result;
}

std::vector<std::shared_ptr<Event>> BookingSystem::findEventsByCategory(const std::string& category) {
    std::vector<std::shared_ptr<Event>> result;

    for (const auto& event : events) {
        if (event->getCategory() == category) {
            result.push_back(event);
        }
    }

    return result;
}

std::vector<std::shared_ptr<Event>> BookingSystem::findEventsByDate(const std::string& date) {
    std::vector<std::shared_ptr<Event>> result;
    DateTime searchDate(date);

    for (const auto& event : events) {
        if (event->getEventDate().toDateString() == searchDate.toDateString()) {
            result.push_back(event);
        }
    }

    return result;
}

std::vector<std::shared_ptr<Event>> BookingSystem::getUpcomingEvents() {
    std::vector<std::shared_ptr<Event>> result;
    DateTime now = DateTime::now();

    for (const auto& event : events) {
        if (event->getEventDate() > now) {
            result.push_back(event);
        }
    }

    return result;
}

std::vector<std::shared_ptr<Event>> BookingSystem::getEventsSortedByDate(bool ascending) {
    std::vector<std::shared_ptr<Event>> result = events;

    std::sort(result.begin(), result.end(),
        [ascending](const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b) {
            return ascending ?
                (a->getEventDate() < b->getEventDate()) :
                (a->getEventDate() > b->getEventDate());
        });

    return result;
}

std::vector<std::shared_ptr<Event>> BookingSystem::getEventsSortedByPrice(bool ascending) {
    std::vector<std::shared_ptr<Event>> result = events;

    std::sort(result.begin(), result.end(),
        [ascending](const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b) {
            return ascending ?
                (a->getBasePrice() < b->getBasePrice()) :
                (a->getBasePrice() > b->getBasePrice());
        });

    return result;
}

std::vector<std::shared_ptr<User>> BookingSystem::findUsersByName(const std::string& nameSubstr) {
    std::vector<std::shared_ptr<User>> result;

    for (const auto& user : users) {
        if (user->getName().find(nameSubstr) != std::string::npos) {
            result.push_back(user);
        }
    }

    return result;
}

std::vector<std::shared_ptr<Ticket>> BookingSystem::getTicketsByUser(int userId) {
    std::vector<std::shared_ptr<Ticket>> result;

    for (const auto& ticket : tickets) {
        if (ticket->getUserId() == userId) {
            result.push_back(ticket);
        }
    }

    return result;
}

std::vector<std::shared_ptr<Ticket>> BookingSystem::getTicketsByEvent(int eventId) {
    std::vector<std::shared_ptr<Ticket>> result;

    for (const auto& ticket : tickets) {
        if (ticket->getEventId() == eventId) {
            result.push_back(ticket);
        }
    }

    return result;
}

std::vector<std::shared_ptr<Ticket>> BookingSystem::getActiveTickets() {
    std::vector<std::shared_ptr<Ticket>> result;

    for (const auto& ticket : tickets) {
        if (ticket->getIsActive()) {
            result.push_back(ticket);
        }
    }

    return result;
}

// Другие методы для работы с системой
void BookingSystem::displayAllEvents() const {
    std::cout << "=================== Список событий ===================\n";
    for (const auto& event : events) {
        event->display();
        std::cout << "----------------------------------------------------\n";
    }
}

void BookingSystem::displayAllUsers() const {
    std::cout << "=================== Список пользователей ===================\n";
    for (const auto& user : users) {
        user->display();
        std::cout << "----------------------------------------------------\n";
    }
}

void BookingSystem::displayAllTickets() const {
    std::cout << "=================== Список билетов ===================\n";
    for (const auto& ticket : tickets) {
        ticket->display();
        std::cout << "----------------------------------------------------\n";
    }
}

double BookingSystem::getTotalSales() const {
    return std::accumulate(tickets.begin(), tickets.end(), 0.0,
        [](double sum, const std::shared_ptr<Ticket>& t) {
            return sum + (t->getIsActive() ? t->getPrice() : 0);
        });
}

int BookingSystem::getActiveTicketsCount() const {
    return std::count_if(tickets.begin(), tickets.end(),
        [](const std::shared_ptr<Ticket>& t) {
            return t->getIsActive();
        });
}

int BookingSystem::getCanceledTicketsCount() const {
    return std::count_if(tickets.begin(), tickets.end(),
        [](const std::shared_ptr<Ticket>& t) {
            return !t->getIsActive();
        });
}

double BookingSystem::getAverageTicketPrice() const {
    if (tickets.empty()) {
        return 0.0;
    }

    double totalPrice = std::accumulate(tickets.begin(), tickets.end(), 0.0,
        [](double sum, const std::shared_ptr<Ticket>& t) {
            return sum + t->getPrice();
        });

    return totalPrice / tickets.size();
}

void BookingSystem::setDataDirectory(const std::string& dir) {
    dataDirectory = dir;
    system(("mkdir " + dataDirectory + " 2>nul").c_str());
}

void BookingSystem::saveAllData() const {
    for (const auto& event : events) {
        event->saveToFile();
    }

    for (const auto& user : users) {
        user->saveToFile();
    }

    for (const auto& ticket : tickets) {
        ticket->saveToFile();
    }
}

void BookingSystem::loadData() {
    std::ifstream userFile(dataDirectory + "users.txt");
    if (userFile.is_open()) {
        int id;
        std::string name, email, phone;

        std::string line;
        while (std::getline(userFile, line)) {
            std::istringstream iss(line);
            if (iss >> id >> name >> email >> phone) {
                nextUserId = std::max(nextUserId, id + 1);

                auto user = std::make_shared<User>(id, name, email, phone);
                users.push_back(user);
            }
        }
        userFile.close();
    }

    std::ifstream concertFile(dataDirectory + "concerts.txt");
    if (concertFile.is_open()) {
        int id, totalSeats, availableSeats, duration;
        double basePrice;
        std::string name, date, venue, artist, genre, description, category;

        std::string line;
        while (std::getline(concertFile, line)) {
            std::istringstream iss(line);
            if (iss >> id >> name >> date >> venue >> totalSeats >> availableSeats >>
                basePrice >> artist >> genre >> duration >> description >> category) {
                nextEventId = std::max(nextEventId, id + 1);

                auto concert = std::make_shared<Concert>(
                    id, name, date, venue, totalSeats, basePrice,
                    artist, genre, duration, description, category
                );
                events.push_back(concert);
            }
        }
        concertFile.close();
    }

    std::ifstream playFile(dataDirectory + "theatreplays.txt");
    if (playFile.is_open()) {
        int id, totalSeats, availableSeats, duration, ageLimit;
        double basePrice;
        std::string name, date, venue, director, genre, description, category;

        std::string line;
        while (std::getline(playFile, line)) {
            std::istringstream iss(line);
            if (iss >> id >> name >> date >> venue >> totalSeats >> availableSeats >>
                basePrice >> director >> genre >> duration >> ageLimit >> description >> category) {
                nextEventId = std::max(nextEventId, id + 1);

                auto play = std::make_shared<TheatrePlay>(
                    id, name, date, venue, totalSeats, basePrice,
                    director, genre, duration, ageLimit, description, category
                );
                events.push_back(play);
            }
        }
        playFile.close();
    }

    std::ifstream ticketFile(dataDirectory + "tickets.txt");
    if (ticketFile.is_open()) {
        int id, eventId, userId;
        double price;
        std::string bookingTime, status;

        std::string line;
        while (std::getline(ticketFile, line)) {
            std::istringstream iss(line);
            if (iss >> id >> eventId >> userId >> price >> bookingTime >> status) {
                nextTicketId = std::max(nextTicketId, id + 1);

                bool isActive = (status == "active");

                auto ticket = std::make_shared<Ticket>(id, eventId, userId, price);
                ticket->setIsActive(isActive);
                tickets.push_back(ticket);

                auto userIt = std::find_if(users.begin(), users.end(),
                    [userId](const std::shared_ptr<User>& u) {
                        return u->getId() == userId;
                    });

                if (userIt != users.end() && isActive) {
                    (*userIt)->addTicket(ticket);
                }
            }
        }
        ticketFile.close();
    }

    std::cout << "Данные успешно загружены из файлов." << std::endl;
    std::cout << "Загружено: " << users.size() << " пользователей, "
        << events.size() << " событий, " << tickets.size() << " билетов." << std::endl;
}
