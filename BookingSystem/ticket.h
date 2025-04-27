#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <memory>
#include <iostream>
#include "interfaces.h"
#include "datetime.h"

class Ticket : public IIdentifiable, public IStorable, public std::enable_shared_from_this<Ticket> {
private:
    int eventId;
    int userId;
    double price;
    std::string bookingTime;
    bool isActive;

public:
    Ticket(int _id, int _eventId, int _userId, double _price);

    int getEventId() const { return eventId; }
    int getUserId() const { return userId; }
    double getPrice() const { return price; }
    const std::string& getBookingTime() const { return bookingTime; }
    bool getIsActive() const { return isActive; }

    void setIsActive(bool status) { isActive = status; }

    void display() const;

    void saveToFile() const override;
};
#endif
