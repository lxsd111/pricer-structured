#pragma once

#include <string>
#include <vector>

// Define the CashFlow structure used throughout the project
struct CashFlow {
    double amount;
    double time;
};

class StructuredProduct {
public:
    StructuredProduct(std::string underlying,
                      std::vector<double> observationTimes)
        : underlying_(std::move(underlying)),
          observationTimes_(std::move(observationTimes)) {}

    virtual ~StructuredProduct() = default;

    // NEW SIGNATURE: Returns a list of cash flows instead of a single discounted double
    virtual std::vector<CashFlow> cashFlows(const std::vector<double> &path) const = 0;

    const std::vector<double> &observationTimes() const {
        return observationTimes_;
    }
    const std::string &underlying() const { return underlying_; }

private:
    std::string underlying_;
    std::vector<double> observationTimes_;
};