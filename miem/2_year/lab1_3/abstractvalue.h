#ifndef ABSTRACTVALUE_H
#define ABSTRACTVALUE_H

#include <string>
#include <ctime>

class AbstractValue {
    time_t _time;
    double _amount;
public:
    AbstractValue();
    AbstractValue(const time_t, const double);
    AbstractValue(const AbstractValue&);
    virtual ~AbstractValue();

    virtual std::string getType() const = 0;

    time_t getTime() const;
    double getAmount() const;

    void setTime(const time_t);
    void setAmount(const double);

    bool operator == (const AbstractValue&) const;
};

#endif // ABSTRACTVALUE_H
