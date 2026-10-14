#include "huffman/huffman_tree.hpp"

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <stdexcept>
#include <utility>

namespace huffman {
namespace {
struct LaterNode {
    bool operator()(const std::shared_ptr<HuffmanNode>& a,
                    const std::shared_ptr<HuffmanNode>& b) const noexcept {
        if (a->frequency != b->frequency) return a->frequency > b->frequency;
        if (a->minSymbol != b->minSymbol) return a->minSymbol > b->minSymbol;
        return false;
    }
};

void makeCodes(const HuffmanNode* node, std::string& path,
               const FrequencyTable& frequencies, std::vector<HuffmanCode>& codes) {
    if (!node) return;
    if (node->isLeaf) {
        // A one-symbol alphabet still needs a decodable, non-empty code.
        codes.push_back({node->symbol, frequencies.counts[node->symbol], path.empty() ? "0" : path});
        return;
    }
    path.push_back('0');
    makeCodes(node->left.get(), path, frequencies, codes);
    path.back() = '1';
    makeCodes(node->right.get(), path, frequencies, codes);
    path.pop_back();
}

void describe(const HuffmanNode* node, const std::string& prefix, std::vector<std::string>& out) {
    if (!node) return;
    if (node->isLeaf) {
        const char digits[] = "0123456789ABCDEF";
        std::string byte = "0x00";
        byte[2] = digits[node->symbol >> 4];
        byte[3] = digits[node->symbol & 15];
        out.push_back(prefix + "leaf " + byte + " freq=" + std::to_string(node->frequency));
    } else {
        out.push_back(prefix + "node freq=" + std::to_string(node->frequency));
        describe(node->left.get(), prefix + "  0-> ", out);
        describe(node->right.get(), prefix + "  1-> ", out);
    }
}
}

HuffmanTree HuffmanTree::build(const FrequencyTable& frequencies) {
    HuffmanTree result;
    std::priority_queue<std::shared_ptr<HuffmanNode>,
                        std::vector<std::shared_ptr<HuffmanNode>>, LaterNode> queue;
    std::uint64_t sum = 0;
    for (std::size_t symbol = 0; symbol < frequencies.counts.size(); ++symbol) {
        const auto frequency = frequencies.counts[symbol];
        if (frequency == 0) continue;
        if (sum > std::numeric_limits<std::uint64_t>::max() - frequency)
            throw std::overflow_error("Frequency total overflows 64-bit count");
        sum += frequency;
        auto leaf = std::make_shared<HuffmanNode>();
        leaf->frequency = frequency;
        leaf->minSymbol = static_cast<std::uint16_t>(symbol);
        leaf->isLeaf = true;
        leaf->symbol = static_cast<std::uint8_t>(symbol);
        queue.push(std::move(leaf));
    }
    if (sum != frequencies.totalBytes)
        throw std::invalid_argument("Frequency table totalBytes does not match its symbol counts");
    while (queue.size() > 1) {
        auto left = queue.top();
        queue.pop();
        auto right = queue.top();
        queue.pop();
        if (left->frequency > std::numeric_limits<std::uint64_t>::max() - right->frequency)
            throw std::overflow_error("Combined node frequency overflows 64-bit count");
        auto parent = std::make_shared<HuffmanNode>();
        parent->frequency = left->frequency + right->frequency;
        parent->minSymbol = std::min(left->minSymbol, right->minSymbol);
        parent->left = std::move(left);
        parent->right = std::move(right);
        queue.push(std::move(parent));
    }
    if (!queue.empty()) {
        result.root_ = queue.top();
    }
    std::string path;
    makeCodes(result.root_.get(), path, frequencies, result.codes_);
    std::sort(result.codes_.begin(), result.codes_.end(),
              [](const HuffmanCode& a, const HuffmanCode& b) { return a.symbol < b.symbol; });
    return result;
}

const HuffmanCode* HuffmanTree::codeFor(std::uint8_t symbol) const noexcept {
    const auto it = std::lower_bound(codes_.begin(), codes_.end(), symbol,
        [](const HuffmanCode& code, std::uint8_t value) { return code.symbol < value; });
    return it != codes_.end() && it->symbol == symbol ? &*it : nullptr;
}

std::vector<std::string> HuffmanTree::describeTree() const {
    std::vector<std::string> lines;
    describe(root_.get(), "", lines);
    return lines;
}

} // namespace huffman
