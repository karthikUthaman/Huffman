#pragma once

#include "huffman/frequency_analyzer.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace huffman {

struct HuffmanNode {
    std::uint64_t frequency{};
    std::uint16_t minSymbol{}; // Stable tie-break key; 0..255 for leaves.
    bool isLeaf{};
    std::uint8_t symbol{};
    std::shared_ptr<HuffmanNode> left;
    std::shared_ptr<HuffmanNode> right;
};

struct HuffmanCode {
    std::uint8_t symbol{};
    std::uint64_t frequency{};
    std::string bits;
};

class HuffmanTree {
public:
    static HuffmanTree build(const FrequencyTable& frequencies);
    const HuffmanNode* root() const noexcept { return root_.get(); }
    const std::vector<HuffmanCode>& codes() const noexcept { return codes_; }
    const HuffmanCode* codeFor(std::uint8_t symbol) const noexcept;
    // Stable preorder text view for diagnostics and later visualization adapters.
    std::vector<std::string> describeTree() const;

private:
    std::shared_ptr<HuffmanNode> root_;
    std::vector<HuffmanCode> codes_;
};

} // namespace huffman
