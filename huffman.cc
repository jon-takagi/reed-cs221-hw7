#include "huffman.hh"
#include <vector>
#include "htree.hh"
#include "hforest.hh"
#include <iostream>


Huffman::~Huffman(){
}

Huffman::Huffman()
{
    freq = {};
    freq.assign(ALPHABET_SIZE, 0);
    root = build_tree();
}

void inorder_traversal(HTree::tree_ptr_t node) {
    if(node->get_child(HTree::Direction::LEFT)) {
    	inorder_traversal(node->get_child(HTree::Direction::LEFT));
    }
    std::cout << std::to_string(node->get_key()) << "\n";
    if (node->get_child(HTree::Direction::RIGHT)) {
    	inorder_traversal(node->get_child(HTree::Direction::RIGHT));
    }
}
void preorder_traversal(HTree::tree_ptr_t node) {
    std::cout << std::to_string(node->get_key()) << "\n";
    if(node->get_child(HTree::Direction::LEFT)) {
    	preorder_traversal(node->get_child(HTree::Direction::LEFT));
    }
    if (node->get_child(HTree::Direction::RIGHT)) {
    	preorder_traversal(node->get_child(HTree::Direction::RIGHT));
    }
}
// Encode a symbol into a sequence of bits, then update frequency table.
Huffman::bits_t Huffman::encode(int symbol) {
    root = Huffman::build_tree();
// inorder_traversal(root);
    auto path = root->path_to(symbol);
    Huffman::bits_t bin_path = {};
    for(HTree::Direction d : path) {
	bin_path.push_back(d == HTree::Direction::LEFT);
// left is 0
// this statement is equivalent to if left push true else push right
// bin_path is now full of 'true' and 'false'
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
    	root = root->get_child(HTree::Direction::LEFT);
    } else {
    	root = root->get_child(HTree::Direction::RIGHT);
    }
    if(!root->get_child(HTree::Direction::RIGHT) && !root->get_child(HTree::Direction::LEFT)) {
    	auto symbol = root->get_value();

    	root = nullptr;
    // reset node after returning

    	freq.at(symbol) += 1;
    // update frequency
    	return symbol;
    } else {
// root is still a node, so calling it again doesn't call build_tree
    	return root->get_value();
    }
}

// This method assumes that there exists a member called `freq` that is a vector storing the frequencies
// after completing, it returns a huffman tree
HTree::tree_ptr_t Huffman::build_tree() {
    HForest heap({});

    for(int i = 0; i < ALPHABET_SIZE; i++) {
    	HTree::tree_ptr_t newTree(new HTree(char(i), freq.at(i)));
    	heap.add_tree(newTree);
    }
    int i = -1;
    // i counts up to become the fake key for the new value
    while(heap.size() > 1) {
    	HTree::tree_ptr_t l = heap.get_min(); //pop the lowest value off the heap
    	HTree::tree_ptr_t r = heap.get_min(); //pop the second lowest off the heap
    	int temp_val = l->get_value() + r->get_value();
    	HTree::tree_ptr_t temp = HTree::tree_ptr_t(new HTree(i, temp_val,l,r));
    	heap.add_tree(temp);
        i = i - 1;
    }
    return heap.pop_top();
}
