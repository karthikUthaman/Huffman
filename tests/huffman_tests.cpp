#include "huffman/frequency_analyzer.hpp"
#include "huffman/huffman_tree.hpp"

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

void testEmpty() {
    const auto frequencies = huffman::FrequencyAnalyzer::analyze({});
    const auto tree = huffman::HuffmanTree::build(frequencies);
    require(frequencies.totalBytes == 0, "empty input byte count");
    require(tree.root() == nullptr && tree.codes().empty(), "empty input has no tree or codes");
}

void testSingleSymbol() {
    const auto frequencies = huffman::FrequencyAnalyzer::analyze({0xFF, 0xFF, 0xFF, 0xFF});
    const auto tree = huffman::HuffmanTree::build(frequencies);
    const auto* code = tree.codeFor(0xFF);
    require(code && code->bits == "0" && code->frequency == 4, "single symbol gets code 0");
    require(tree.codes().size() == 1, "single symbol code count");
}

void testBinaryAndDeterminism() {
    const std::vector<std::uint8_t> input{0, 255, 0, 128, 255, 1, 128, 255};
    const auto frequencies = huffman::FrequencyAnalyzer::analyze(input);
    require(frequencies.totalBytes == 8 && frequencies.counts[255] == 3 &&
            frequencies.counts[0] == 2 && frequencies.counts[128] == 2,
            "binary byte counts");
    const auto first = huffman::HuffmanTree::build(frequencies);
    const auto second = huffman::HuffmanTree::build(frequencies);
    require(first.codes().size() == 4, "four distinct symbols");
    for (const auto& code : first.codes()) {
        const auto* repeated = second.codeFor(code.symbol);
        require(repeated && repeated->bits == code.bits, "tree construction is deterministic");
    }
    for (const auto& a : first.codes()) for (const auto& b : first.codes()) {
        if (a.symbol != b.symbol)
            require(b.bits.compare(0, a.bits.size(), a.bits) != 0, "codes are prefix-free");
    }
}

void testFileAnalyzer() {
    const auto path = std::filesystem::temp_directory_path() / "huffmanforge_phase1_test.bin";
    {
        std::ofstream out(path, std::ios::binary);
        const char bytes[] = {0, static_cast<char>(0x80), static_cast<char>(0xFF), 0};
        out.write(bytes, sizeof(bytes));
    }
    const auto frequencies = huffman::FrequencyAnalyzer::analyzeFile(path);
    std::filesystem::remove(path);
    require(frequencies.totalBytes == 4 && frequencies.counts[0] == 2 &&
            frequencies.counts[128] == 1 && frequencies.counts[255] == 1,
            "file analyzer preserves arbitrary byte values");
}
}

int main() {
    try {
        testEmpty();
        testSingleSymbol();
        testBinaryAndDeterminism();
        testFileAnalyzer();
        std::cout << "All HuffmanForge Phase 1 tests passed.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Test failure: " << error.what() << '\n';
        return 1;
    }
}
