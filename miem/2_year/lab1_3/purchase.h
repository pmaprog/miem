#ifndef PURCHASE_H
#define PURCHASE_H

#include "abstractvalue.h"

class Purchase : public AbstractValue {
public:
    Purchase();
    Purchase(const time_t, const double);
    Purchase(const Purchase&);
    ~Purchase();

    std::string getType() const;

    bool operator ==(const Purchase&) const;
    bool operator !=(const Purchase&) const;
};

#endif // PURCHASE_H
