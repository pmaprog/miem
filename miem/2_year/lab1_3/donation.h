#ifndef DONATION_H
#define DONATION_H

#include "abstractvalue.h"

class Donation : public AbstractValue {
    std::string _name;
public:
    Donation();
    Donation(const time_t, const double, const std::string = "Unnamed");
    Donation(const Donation&);
    ~Donation();

    std::string getType() const;

    std::string getName() const;

    void setName(const std::string);

    bool operator ==(const Donation&) const;
    bool operator !=(const Donation&) const;
};

#endif // DONATION_H
