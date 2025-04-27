#ifndef INTERFACES_H
#define INTERFACES_H

class IStorable {
public:
    virtual void saveToFile() const = 0;
    virtual ~IStorable() = default;
};

class IIdentifiable {
protected:
    int id;
public:
    IIdentifiable(int _id) : id(_id) {}
    int getId() const { return id; }
    virtual ~IIdentifiable() = default;
};

#endif
