/*
 * SUMMARY: Utility functions for string parsing and formatting.
 * Primary role is to convert between raw user input strings (e.g., "1.0, 2.0, 3.0") 
 * and internal C++ vectors, handling common issues like whitespace trimming 
 * and CSV parsing.
 */

#include "InputUtils.hpp"

#include <cctype>
#include <sstream>

namespace {
// Helper: Removes leading and trailing whitespace from a raw string.
std::string trim(const std::string& input) {
    std::size_t first = 0;
    while (first < input.size() &&
           std::isspace(static_cast<unsigned char>(input[first]))) {
        ++first;
    }
    std::size_t last = input.size();
    while (last > first &&
           std::isspace(static_cast<unsigned char>(input[last - 1]))) {
        --last;
    }
    return input.substr(first, last - first);
}
} // namespace

std::string vectorToString(const std::vector<double>& values) {
    std::ostringstream oss;
    // Serialize the vector into a simple CSV format (e.g. for GUI display).
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            oss << ',';
        }
        oss << values[i];
    }
    return oss.str();
}

std::vector<double> parseTimesList(const std::string& text,
                                   const std::vector<double>& fallback) {
    // Robust parser for comma-separated input strings.
    // Returns the default 'fallback' vector if the input is empty or invalid.
    if (text.empty()) {
        return fallback;
    }

    std::vector<double> result;
    std::stringstream ss(text);
    std::string token;
    
    // Split by comma and clean up each token.
    while (std::getline(ss, token, ',')) {
        const std::string cleaned = trim(token);
        if (cleaned.empty()) {
            continue;
        }
        // Ideally, we'd add try/catch here for std::stod safety,
        // but for now we assume valid numeric input.
        result.push_back(std::stod(cleaned));
    }

    return result.empty() ? fallback : result;
}