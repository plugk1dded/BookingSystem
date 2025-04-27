#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "interfaces.h"
#include "datetime.h"

class Ticket;
class User;
class BookingSystem;

class Event : public IIdentifiable, public IStorable, public std::enable_shared_from_this<Event> {
protected:
    std::string name;
    DateTime eventDate;
    std::string venue;
    int totalSeats;
    int availableSeats;
    double basePrice;
    std::vector<std::shared_ptr<Ticket>> tickets;
    std::string description;
    std::string category;

public:
    Event(int _id, const std::string& _name, const std::string& _date,
        const std::string& _venue, int _totalSeats, double _basePrice,
        const std::string& _description = "", const std::string& _category = "Общее");

    virtual ~Event() = default;

    const std::string& getName() const { return name; }
    const DateTime& getEventDate() const { return eventDate; }
    std::string getDate() const { return eventDate.toDateString(); }
    const std::string& getVenue() const { return venue; }
    int getTotalSeats() const { return totalSeats; }
    int getAvailableSeats() const { return availableSeats; }
    double getBasePrice() const { return basePrice; }
    const std::string& getDescription() const { return description; }
    const std::string& getCategory() const { return category; }

    void setName(const std::string& _name) { name = _name; }
    void setDate(const std::string& _date) { eventDate = DateTime(_date); }
    void setVenue(const std::string& _venue) { venue = _venue; }
    void setBasePrice(double _price) { basePrice = _price; }
    void setDescription(const std::string& _description) { description = _description; }
    void setCategory(const std::string& _category) { category = _category; }

    bool isExpired() const;

    virtual double calculateTicketPrice() const;

    virtual std::shared_ptr<Ticket> createTicket(std::shared_ptr<User> user);

    void saveToFile() const override;

    virtual void display() const;

    void decreaseAvailableSeats();

    void increaseAvailableSeats();
};

class Concert : public Event {
private:
    std::string artist;
    std::string genre;
    int duration;

public:
    Concert(int _id, const std::string& _name, const std::string& _date,
        const std::string& _venue, int _totalSeats, double _basePrice,
        const std::string& _artist, const std::string& _genre,
        int _duration = 120, const std::string& _description = "",
        const std::string& _category = "Концерт");

    double calculateTicketPrice() const override;

    void display() const override;

    const std::string& getArtist() const { return artist; }
    const std::string& getGenre() const { return genre; }
    int getDuration() const { return duration; }

    void setArtist(const std::string& _artist) { artist = _artist; }
    void setGenre(const std::string& _genre) { genre = _genre; }
    void setDuration(int _duration) { duration = _duration; }

    void saveToFile() const override;
};

class TheatrePlay : public Event {
private:
    std::string director;
    std::string genre;
    int duration;
    int ageLimit;

public:
    TheatrePlay(int _id, const std::string& _name, const std::string& _date,
        const std::string& _venue, int _totalSeats, double _basePrice,
        const std::string& _director, const std::string& _genre,
        int _duration = 180, int _ageLimit = 0,
        const std::string& _description = "", const std::string& _category = "Театр");

    double calculateTicketPrice() const override;

    void display() const override;

    const std::string& getDirector() const { return director; }
    const std::string& getGenre() const { return genre; }
    int getDuration() const { return duration; }
    int getAgeLimit() const { return ageLimit; }

    void setDirector(const std::string& _director) { director = _director; }
    void setGenre(const std::string& _genre) { genre = _genre; }
    void setDuration(int _duration) { duration = _duration; }
    void setAgeLimit(int _ageLimit) { ageLimit = _ageLimit; }

    void saveToFile() const override;
};
#endif
