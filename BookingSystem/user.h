#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "interfaces.h"

class Ticket;
class Event;

class User : public IIdentifiable, public IStorable, public std::enable_shared_from_this<User> {
private:
    std::string name;
    std::string email;
    std::string phone;
    std::vector<std::shared_ptr<Ticket>> tickets;

public:
    User(int _id, const std::string& _name, const std::string& _email, const std::string& _phone);

    const std::string& getName() const { return name; }
    const std::string& getEmail() const { return email; }
    const std::string& getPhone() const { return phone; }

    void setName(const std::string& _name) { name = _name; }
    void setEmail(const std::string& _email) { email = _email; }
    void setPhone(const std::string& _phone) { phone = _phone; }

    bool bookTicket(std::shared_ptr<Event> event);

    bool cancelTicket(int ticketId);

    const std::vector<std::shared_ptr<Ticket>>& getTickets() const { return tickets; }

    void addTicket(std::shared_ptr<Ticket> ticket);

    void removeTicket(int ticketId);

    void display() const;

    void saveToFile() const override;
};
#endif
