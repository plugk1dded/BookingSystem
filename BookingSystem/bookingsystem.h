#ifndef BOOKINGSYSTEM_H
#define BOOKINGSYSTEM_H

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include "event.h"
#include "user.h"
#include "ticket.h"

class BookingSystem {
private:
    std::vector<std::shared_ptr<Event>> events;
    std::vector<std::shared_ptr<User>> users;
    std::vector<std::shared_ptr<Ticket>> tickets;
    int nextEventId = 1;
    int nextUserId = 1;
    int nextTicketId = 1;

    static BookingSystem* instance;

    std::string dataDirectory;

    BookingSystem();

public:
    static BookingSystem& getInstance();

    BookingSystem(const BookingSystem&) = delete;
    BookingSystem& operator=(const BookingSystem&) = delete;

    static void destroy();

    std::shared_ptr<Concert> createConcert(
        const std::string& name, const std::string& date, const std::string& venue,
        int totalSeats, double basePrice, const std::string& artist, const std::string& genre,
        int duration = 120, const std::string& description = "", const std::string& category = "Концерт");

    std::shared_ptr<TheatrePlay> createTheatrePlay(
        const std::string& name, const std::string& date, const std::string& venue,
        int totalSeats, double basePrice, const std::string& director, const std::string& genre,
        int duration = 180, int ageLimit = 0, const std::string& description = "", const std::string& category = "Театр");

    std::shared_ptr<User> createUser(
        const std::string& name, const std::string& email, const std::string& phone);

    std::shared_ptr<Ticket> createTicket(
        std::shared_ptr<Event> event, std::shared_ptr<User> user);

    bool cancelTicket(int ticketId);

    std::shared_ptr<Event> findEventById(int id);
    std::shared_ptr<User> findUserById(int id);
    std::shared_ptr<Ticket> findTicketById(int id);

    std::vector<std::shared_ptr<Event>> findEventsByName(const std::string& nameSubstr);
    std::vector<std::shared_ptr<Event>> findEventsByCategory(const std::string& category);
    std::vector<std::shared_ptr<Event>> findEventsByDate(const std::string& date);
    std::vector<std::shared_ptr<Event>> getUpcomingEvents();
    std::vector<std::shared_ptr<Event>> getEventsSortedByDate(bool ascending = true);
    std::vector<std::shared_ptr<Event>> getEventsSortedByPrice(bool ascending = true);
    std::vector<std::shared_ptr<User>> findUsersByName(const std::string& nameSubstr);
    std::vector<std::shared_ptr<Ticket>> getTicketsByUser(int userId);
    std::vector<std::shared_ptr<Ticket>> getTicketsByEvent(int eventId);
    std::vector<std::shared_ptr<Ticket>> getActiveTickets();

    void displayAllEvents() const;
    void displayAllUsers() const;
    void displayAllTickets() const;

    double getTotalSales() const;
    int getActiveTicketsCount() const;
    int getCanceledTicketsCount() const;
    double getAverageTicketPrice() const;

    void setDataDirectory(const std::string& dir);

    void saveAllData() const;
    void loadData();
};
#endif
