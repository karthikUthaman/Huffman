#include "huffman/frequency_analyzer.hpp"
#include "huffman/huffman_tree.hpp"

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace {
void show(const huffman::FrequencyTable& frequencies) {
    huffman::printFrequencyTable(frequencies, std::cout);
    const auto tree = huffman::HuffmanTree::build(frequencies);
    std::cout << "\nHuffman codes (symbol order)\n";
    for (const auto& code : tree.codes()) {
        std::cout << "0x" << std::hex << std::uppercase;
        if (code.symbol < 16) std::cout << '0';
        std::cout << static_cast<unsigned>(code.symbol) << std::dec
                  << "  freq=" << code.frequency << "  code=" << code.bits << '\n';
    }
    std::cout << "\nTree (0=left, 1=right)\n";
    const auto lines = tree.describeTree();
    if (lines.empty()) std::cout << "(empty input: no tree)\n";
    for (const auto& line : lines) std::cout << line << '\n';
}
}

int main(int argc, char* argv[]) {
    try {
        if (argc == 1 || (argc == 2 && std::string(argv[1]) == "--demo")) {
            const std::string sample = "HuffmanForge Phase 1 demo\n";
            std::vector<std::uint8_t> bytes(sample.begin(), sample.end());
            std::cout << "HuffmanForge Phase 1 - built-in demo\n";
            show(huffman::FrequencyAnalyzer::analyze(bytes));
            return 0;
        }
        if (argc == 3 && std::string(argv[1]) == "analyze") {
            show(huffman::FrequencyAnalyzer::analyzeFile(argv[2]));
            return 0;
        }
        std::cerr << "Usage: huffforge [--demo | analyze <file-path>]\n";
        return 2;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
