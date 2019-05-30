#ifndef SAMPLE_H
#define SAMPLE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Sample : public std::vector<double> {
public:
    void load_from_file(const std::string &);

    double mean() const;
    double variance() const;
    double variance_seq() const;
    double std() const;
    double v() const;
};

#endif // SAMPLE_H
