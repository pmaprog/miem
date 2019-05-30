#include "sample.h"

void Sample::load_from_file(const std::string &path) {
    if (path == "")
        return;

    this->clear();

    std::ifstream fin(path);

    while (fin) {
        std::string row;
        fin >> row;

        size_t comma_pos = row.find(",");
        if (comma_pos != std::string::npos)
            row.replace(comma_pos, 1, ".");

        if (row != "") {
            std::istringstream i(row);
            double d;
            i >> d;
            this->push_back(d);
        }
    }
}

double Sample::mean() const {
    double result = 0;

    for (auto i : *this) {
        result += i;
    }

    result /= size();

    return result;
}

double Sample::variance() const {
    double avg = mean();
    double result = 0;

    for (auto i : *this) {
        result += (i - avg) * (i - avg);
    }

    result /= size() - 1;

    return result;
}

double Sample::variance_seq() const {
    double result = 0;

    for (int i = 0; i < size()-1; ++i) {
        result += ((*this)[i+1] - (*this)[i]) * ((*this)[i+1] - (*this)[i]);
    }

    result /= 2 * (size() - 1);

    return result;
}

double Sample::std() const {
    return sqrt(variance());
}

double Sample::v() const {
    return variance_seq() / variance();
}
