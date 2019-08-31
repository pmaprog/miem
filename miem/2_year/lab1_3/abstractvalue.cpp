#include "abstractvalue.h"

AbstractValue::AbstractValue() : _time(0), _amount(0.0) { }

AbstractValue::AbstractValue(const time_t time, const double amount) : AbstractValue() {
    setTime(time);
    setAmount(amount);
}

AbstractValue::AbstractValue(const AbstractValue &p) : _time(p._time), _amount(p._amount) { }

AbstractValue::~AbstractValue() { }

time_t AbstractValue::getTime() const { return _time; }

double AbstractValue::getAmount() const { return _amount; }

void AbstractValue::setTime(const time_t time) {
    if (time >= 0) {
        _time = time;
    }
}

void AbstractValue::setAmount(const double amount) {
    if (amount >= 0) {
        _amount = amount;
    }
}

bool AbstractValue::operator ==(const AbstractValue& p) const {
    return (_time == p._time && _amount == p._amount);
}
