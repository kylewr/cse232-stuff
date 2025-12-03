#pragma once

#include <string>
#include <unordered_map>

class Measurement {
    public:
        Measurement(double value, double uncertainty, const std::string& unit);
        Measurement(double value, double uncertainty,
                    const std::unordered_map<std::string, int>& unit_map)
            : value_(value), uncertainty_(uncertainty), unit_map_(unit_map) {}
        ~Measurement() = default;

        double GetValue() const {
            return value_;
        }
        double GetUncertainty() const {
            return uncertainty_;
        }
        const std::unordered_map<std::string, int>& GetUnitMap() {
            return unit_map_;
        }

        std::string GetOrderedUnits();
        std::string ToString();

        Measurement Add(Measurement& other);
        Measurement Subtract(Measurement& other);

        Measurement Multiply(Measurement& other);
        Measurement Divide(Measurement& other);

        Measurement RaisedToPower(int power);

        static bool CompareUnits(const Measurement& m1, const Measurement& m2);

    private:
        double value_;
        double uncertainty_;
        std::unordered_map<std::string, int> unit_map_;

        double GetProductUncertainty(Measurement& other);
};
