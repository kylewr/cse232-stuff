#include "Measurement.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <vector>

Measurement::Measurement(double value, double uncertainty, const std::string& unit)
    : value_(value), uncertainty_(uncertainty) {
    unit_map_[unit] = 1;
}

std::string Measurement::GetOrderedUnits() {
    std::vector<std::string> units;
    for (const auto& pair : unit_map_) {
        if (pair.second == 0) {
            continue;
        }

        units.push_back(pair.first + (pair.second != 1 ? "^" + std::to_string(pair.second) : ""));
    }

    std::sort(units.begin(), units.end());
    std::ostringstream oss;
    for (const auto& unit : units) {
        oss << unit << ' ';
    }
    return oss.str();
}

std::string Measurement::ToString() {
    std::ostringstream oss;

    oss.precision(2);
    oss << std::scientific << value_ << " +- " << uncertainty_ << ' ' << GetOrderedUnits() << "";
    return oss.str();
}

Measurement Measurement::Add(Measurement& other) {
    if (!CompareUnits(*this, other)) {
        throw std::invalid_argument("Units do not match for addition.");
    }

    double new_value = value_ + other.GetValue();
    double new_uncertainty =
        std::sqrt(std::pow(uncertainty_, 2) + std::pow(other.GetUncertainty(), 2));

    return Measurement(new_value, new_uncertainty, GetUnitMap());
}

Measurement Measurement::Subtract(Measurement& other) {
    if (!CompareUnits(*this, other)) {
        throw std::invalid_argument("Units do not match for subtraction.");
    }

    double new_value = value_ - other.GetValue();
    double new_uncertainty =
        std::sqrt(std::pow(uncertainty_, 2) + std::pow(other.GetUncertainty(), 2));

    return Measurement(new_value, new_uncertainty, GetUnitMap());
}

Measurement Measurement::Multiply(Measurement& other) {
    double new_value = value_ * other.GetValue();
    double new_rel_uncertainty = GetProductUncertainty(other);
    double new_uncertainty = new_value * new_rel_uncertainty;

    std::unordered_map<std::string, int> new_unit_map = GetUnitMap();
    const auto& other_unit_map = other.GetUnitMap();
    for (const auto& pair : other_unit_map) {
        new_unit_map[pair.first] += pair.second;
    }

    return Measurement(new_value, new_uncertainty, new_unit_map);
}

Measurement Measurement::Divide(Measurement& other) {
    double new_value = value_ / other.GetValue();
    double new_rel_uncertainty = GetProductUncertainty(other);
    double new_uncertainty = new_value * new_rel_uncertainty;

    std::unordered_map<std::string, int> new_unit_map = GetUnitMap();
    const auto& other_unit_map = other.GetUnitMap();
    for (const auto& pair : other_unit_map) {
        new_unit_map[pair.first] -= pair.second;
    }

    return Measurement(new_value, new_uncertainty, new_unit_map);
}

Measurement Measurement::RaisedToPower(int power) {
    double new_value = std::pow(value_, power);
    double rel_uncertainty = uncertainty_ / value_;
    double new_rel_uncertainty = std::abs(power) * rel_uncertainty;
    double new_uncertainty = new_value * new_rel_uncertainty;

    std::unordered_map<std::string, int> new_unit_map;
    for (const auto& pair : unit_map_) {
        new_unit_map[pair.first] = pair.second * power;
    }

    return Measurement(new_value, new_uncertainty, new_unit_map);
}

bool Measurement::CompareUnits(const Measurement& m1, const Measurement& m2) {
    std::vector<std::string> units1;
    for (const auto& pair : m1.unit_map_) {
        units1.push_back(pair.first);
    }
    std::vector<std::string> units2;
    for (const auto& pair : m2.unit_map_) {
        units2.push_back(pair.first);
    }

    std::sort(units1.begin(), units1.end());
    std::sort(units2.begin(), units2.end());

    return units1 == units2;
}

double Measurement::GetProductUncertainty(Measurement& other) {
    double rel_uncertainty_1 = uncertainty_ / value_;
    double rel_uncertainty_2 = other.GetUncertainty() / other.GetValue();
    double new_rel_uncertainty =
        std::sqrt(std::pow(rel_uncertainty_1, 2) + std::pow(rel_uncertainty_2, 2));
    return new_rel_uncertainty;
}
