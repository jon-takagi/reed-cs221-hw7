#include "huffman.hh"
#include <vector>
#include "htree.cc"
#include "hforest.cc"


Huffman:: Huffman();
Huffman:: ~Huffman();
Huffman:: Huffman(const Huffman&) = delete;
Huffman::Huffman(Huffman&&) = delete;
Huffman& Huffman::operator=(const Huffman&) = delete;
Huffman& Huffman::operator=(Huffman&&) = delete;

// Encode a symbol into a sequence of bits, then update frequency table.
Huffman::bits_t Huffman::encode(int symbol) {
    root = build_tree();
    path = root.path_to(symbol);
    Huffman::bits_t bin_path = {};
    for(HTree::Direction d : path) {
        bin_path.push_back(d == HTree::Direction::LEFT);
        // left is 0
        // this statement is equivalent to if left push true else push right
    }
    freq.at(symbol) += 1;
    return bin_path;
}

// Decode a single bit into a symbol. If no symbol can be unmabiguously decoded
// without additional bits, returns a negative value.
// Subsequent calls with more bits should eventually resolve to either
// a character symbol or HEOF.
// Finally, updates the frequency table with this additional symbol.
int Huffman::decode(bool bit) {
    if(root) {

    } else {
        root = build_tree();
    }
    if(bit == 0) {
        root = root.get_child(HTree::Direction::LEFT);
    } else {
        root = root.get_child(HTree::Direction::RIGHT);
    }
    if(!root.get_child(HTree::Direction::RIGHT) && !root.get_child(HTree::Direction::LEFT)) {
        auto return_val = root.get_value();

        root = nullptr;
        // reset node after returning

        freq.at(symbol) += 1;
        // update frequency
        return return_val;
    } else {
        // root is still a node, so calling it again doesn't call build_tree
        return root.get_value();
    }
}

// This method assumes that there exists a member called `freq` that is a vector storing the frequencies
// after completing, it returns a huffman tree
HTree::tree_ptr_t build_tree() {
    HForest heap();
    for(int i = 0; i < freq.size(); i++) {
        HTree::tree_ptr_t newTree(char(i), freq.at(i));
        heap.add_tree(newTree);
    }
    while(heap.size() > 1){
        auto l = heap.get_min();
        auto r = heap.get_min();
        auto temp = tree_ptr_t(-1, l.get_value() + r.get_value(),l,r);
        heap.push(temp);
    }
}
