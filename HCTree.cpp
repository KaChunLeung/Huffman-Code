#include "HCTree.hpp"
#include <queue>
#include <vector>
#include <stack>
using namespace std;

// Build the Huffman Tree from the frequency vector
void HCTree::build(const vector<int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    // Create leaf nodes for each symbol
    for (size_t i = 0; i < freqs.size(); ++i) {
        if (freqs[i] > 0) {
            HCNode* node = new HCNode(freqs[i], i);
            leaves[i] = node;
            pq.push(node);
        }
    }

    // Special cases: empty tree or single node tree
    if (pq.empty()) return;
    if (pq.size() == 1) {
        root = pq.top();
        return;
    }

    // Build the tree
    while (pq.size() > 1) {
        HCNode* left = pq.top(); pq.pop();
        HCNode* right = pq.top(); pq.pop();

        // Assuming HCNode only takes count and symbol for its constructor
        HCNode* parent = new HCNode(left->count + right->count, 0); // 0 or any dummy symbol
        parent->c0 = left;
        parent->c1 = right;

        pq.push(parent);
    }

    root = pq.top();
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    // Find the path from root to the leaf node corresponding to the symbol
    stack<bool> bits;
    findSymbol(root, symbol, bits);

    while (!bits.empty()) {
        out.write_bit(bits.top());
        bits.pop();
    }
}

// Helper function to find the path from root to the leaf node
bool HCTree::findSymbol(HCNode* node, unsigned char symbol, stack<bool>& bits) const {
    if (node == nullptr) return false;
    if (node->symbol == symbol && !node->c0 && !node->c1) {
        // Found the symbol
        return true;
    }
    // Try to find the symbol in the left subtree
    if (findSymbol(node->c0, symbol, bits)) {
        bits.push(false); // Left child represents a '0' bit
        return true;
    }
    // Try to find the symbol in the right subtree
    if (findSymbol(node->c1, symbol, bits)) {
        bits.push(true); // Right child represents a '1' bit
        return true;
    }
    return false;
}

// Decode a Huffman code from the input stream to its original symbol
unsigned char HCTree::decode(FancyInputStream & in) const {
    HCNode* node = root;
    while (node->c0 || node->c1) {
        if (in.read_bit()) {
            node = node->c1;
        } else {
            node = node->c0;
        }
    }
    return (unsigned char)node->symbol;
}

// Destructor to clean up memory
HCTree::~HCTree() {
    deleteTree(root);
}

// Private helper method for destructor to delete nodes in post-order
void HCTree::deleteTree(HCNode* node) {
    if (node) {
        deleteTree(node->c0);
        deleteTree(node->c1);
        delete node;
    }
}


