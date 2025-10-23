#pragma once

class Temperature {
public:
    Temperature(double temp, char unit) : unit_(unit) {
        if (unit_ == 'F') {
            tempF_ = temp;
        } else if (unit_ == 'C') {
            tempF_ = temp * 9.0 / 5.0 + 32.0;
        } else if (unit_ == 'K') {
            tempF_ = (temp - 273.15) * 9.0 / 5.0 + 32.0;
        } else if (unit_ == 'R') {
            tempF_ = temp - 459.67;
        } else {
            tempF_ = temp;
        }
    }

    double AsCelsius() const {
        return (tempF_ - 32.0) * 5.0 / 9.0;
    };
    double AsFahrenheit() const {
        return tempF_;
    };
    double AsKelvin() const {
        return AsCelsius() + 273.15;
    };
    double AsRankine() const {
        return tempF_ + 459.67;
    };

private:
    double tempF_;
    char unit_;
};