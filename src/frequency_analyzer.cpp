#include "huffman/frequency_analyzer.hpp"

#include <iomanip>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <fstream>

namespace huffman {

FrequencyTable FrequencyAnalyzer::analyze(const std::vector<std::uint8_t>& bytes) {
    FrequencyTable result;
    for (const auto byte : bytes) {
        if (result.counts[byte] == std::numeric_limits<std::uint64_t>::max() ||
            result.totalBytes == std::numeric_limits<std::uint64_t>::max()) {
            throw std::overflow_error("Input is too large to count with 64-bit frequencies");
        }
        ++result.counts[byte];
        ++result.totalBytes;
    }
    return result;
}

FrequencyTable FrequencyAnalyzer::analyzeFile(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("Could not open input file: " + path.string());

    FrequencyTable result;
    char buffer[64 * 1024];
    while (input) {
        input.read(buffer, sizeof(buffer));
        const auto count = input.gcount();
        for (std::streamsize i = 0; i < count; ++i) {
            const auto byte = static_cast<std::uint8_t>(static_cast<unsigned char>(buffer[i]));
            if (result.counts[byte] == std::numeric_limits<std::uint64_t>::max() ||
                result.totalBytes == std::numeric_limits<std::uint64_t>::max()) {
                throw std::overflow_error("Input is too large to count with 64-bit frequencies");
            }
            ++result.counts[byte];
            ++result.totalBytes;
        }
        if (input.bad()) throw std::runtime_error("Error while reading input file: " + path.string());
    }
    return result;
}

void printFrequencyTable(const FrequencyTable& table, std::ostream& out) {
    out << "Bytes: " << table.totalBytes << "\nSymbol  Frequency\n";
    for (std::size_t i = 0; i < table.counts.size(); ++i) {
        if (table.counts[i] == 0) continue;
        out << "0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << i
            << std::dec << std::setfill(' ') << "     " << table.counts[i] << '\n';
    }
}

} // namespace huffman
