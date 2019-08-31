#include "donation.h"

Donation::Donation() : AbstractValue(), _name("Unnamed") { }

Donation::Donation(const time_t time, const double amount, const std::string name)
    : AbstractValue(time, amount) {
    setName(name);
}

Donation::Donation(const Donation& d) : AbstractValue(d), _name(d._name) { }

Donation::~Donation() { }

std::string Donation::getType() const { return "Donation"; }

std::string Donation::getName() const { return _name; }

void Donation::setName(const std::string name) {
    if (name != "") {
        _name = name;
    }
}

bool Donation::operator ==(const Donation& d) const {
    return ((*dynamic_cast<const AbstractValue*>(this)
            == *dynamic_cast<const AbstractValue*>(&d)) && (_name == d._name));
}

bool Donation::operator !=(const Donation& d) const {
    return !(*this == d);
}
