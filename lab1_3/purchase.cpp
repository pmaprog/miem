#include "purchase.h"

Purchase::Purchase() : AbstractValue() { }

Purchase::Purchase(const time_t time, const double amount) : AbstractValue(time, amount) { }

Purchase::Purchase(const Purchase& p) : AbstractValue(p) { }

Purchase::~Purchase() { }

std::string Purchase::getType() const { return "Purchase"; }

bool Purchase::operator ==(const Purchase& p) const {
    return (*dynamic_cast<const AbstractValue*>(this) == *dynamic_cast<const AbstractValue*>(&p));
}

bool Purchase::operator !=(const Purchase& p) const {
    return !(*this == p);
}
