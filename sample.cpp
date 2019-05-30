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

}

double Sample::variance() const {

}

double Sample::variance_seq() const {

}

double Sample::std() const {

}

double Sample::v() const {

}
