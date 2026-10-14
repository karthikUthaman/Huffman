#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <iosfwd>
#include <vector>

namespace huffman {

struct FrequencyTable {
    std::array<std::uint64_t, 256> counts{};
    std::uint64_t totalBytes{};
};

class FrequencyAnalyzer {
public:
    static FrequencyTable analyze(const std::vector<std::uint8_t>& bytes);
    // Reads in binary mode and throws std::runtime_error on open/read failures.
    static FrequencyTable analyzeFile(const std::filesystem::path& path);
};

void printFrequencyTable(const FrequencyTable& table, std::ostream& out);

} // namespace huffman
