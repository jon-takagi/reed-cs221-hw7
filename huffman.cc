#include "huffman.hh"
#include <vector>
#include "htree.hh"
#include "hforest.hh"
#include <iostream>
#include <cassert>


Huffman::~Huffman(){
}

// frequency table starts out empty
// has size of ALPHABET_SIZE
// build the tree initially so that root isn't nullptr
Huffman::Huffman()
{
    freq = {};
    freq.assign(ALPHABET_SIZE, 0);
    root = build_tree();
}

// Encode a symbol into a sequence of bits, then update frequency table.
Huffman::bits_t Huffman::encode(int symbol) {
    // rebuild the tree, since frequency changes at the end
    root = Huffman::build_tree();
    // path_to is a list of Directions
    // convert it to a vector of booleans
    std::list<Direction> path = root->path_to(symbol);
    Huffman::bits_t bin_path = {};
    for(HTree::Direction d : path) {
        bin_path.push_back(d == HTree::Direction::RIGHT);
        // left is 0; 0 is false
        // right is 1; 1 is true
        // push_back puts it at the end, and the iterator goes from front to back
    }
    freq.at(symbol) += 1;
    // update the frequency
    return bin_path;
}

// Decode a single bit into a symbol. If no symbol can be unmabiguously decoded
// without additional bits, returns a negative value.
// Subsequent calls with more bits should eventually resolve to either
// a character symbol or HEOF.
// Finally, updates the frequency table with this additional symbol.
int Huffman::decode(bool bit) {
    if(root) {
        // if root isn't nullptr, you were already in the middle of decoding a symbol
        // ie you were partway through traversing the tree
    } else {
    	root = build_tree();
    }
    // no way to 1 liner like I did in Encode
    // fun fact i spent over an hour debugging before i realized that i had d == left
    if(bit == 0) {
    	root = root->get_child(HTree::Direction::LEFT);
    } else {
    	root = root->get_child(HTree::Direction::RIGHT);
    }
    // if root is a leaf
        // update the frequency
        // reset root to nullptr
        // return its symbol
    if(!root->get_child(HTree::Direction::RIGHT) && !root->get_child(HTree::Direction::LEFT)) {
    	int symbol = root->get_key();

    	root = nullptr;
    // reset node after returning
    // this causes the next call to decode to rebuild the tree
    // using a single huff as encoder and decoder won't cause issues with root being nullptr
    // because encode immediately rebuilds the tree

    	freq.at(symbol) += 1;
    // update frequency
    	return symbol;
    } else {
// root is still a node, so calling it again doesn't call build_tree
// root isn't a leaf, so its key is negative
    	return root->get_key();
    }
}

// This method assumes that there exists a member called `freq` that is a vector storing the frequencies
// after completing, it returns a huffman tree
HTree::tree_ptr_t Huffman::build_tree() {
    HForest heap({});
    //Create one new HTree node for every possible character + EOF (257 total). Each node will have its symbol (character) as the key, and its count from the frequency table as a value.
    // if you use the character representation of i
    // char(i)
    // then it could be a leaf
    // and have key_ < 0
    // which is a problem
    // key_ < 0 indicates a fake leaf
    for(int i = 0; i < ALPHABET_SIZE; i++) {
    	HTree::tree_ptr_t newTree(new HTree(i, freq.at(i)));
    	heap.add_tree(newTree);
    }
    int i = -257;
    // i counts up to become the fake key for the new value
    // no real reason for this - could just use -1 each time
    // using different fake keys helped us debug
    // we tried counting up when we were messing with alternate ways to use HForest
    // turns out you don't modify it at all
    while(heap.size() > 1) {
    	HTree::tree_ptr_t l = heap.pop_top(); //pop the lowest value off the heap
    	HTree::tree_ptr_t r = heap.pop_top(); //pop the second lowest off the heap
        // this helps us get a nicely shaped tree
        // not necessary though
        // this way, all the leaves are a left child
        // really only useful for debugging, but could maybe make it faster?
        if(l -> get_key() < 0) {
            HTree::tree_ptr_t temp = l;
            l = r;
            r = temp;
        }
        // negative key means fake node
        // its value is the sum of the frequencies of its children
        // ie the value of l and r
    	int temp_val = l->get_value() + r->get_value();
    	HTree::tree_ptr_t temp = HTree::tree_ptr_t(new HTree(i, temp_val,l,r));
        i += 1;
    	heap.add_tree(temp);
    }
    // we want the tree, we don't care about the hforest
    // strictly speaking, hforest isn't necessary and we could have a plain old vector
    // and just define some operations on HTree
    // but this is more fun
    // once heap_size is 1 and the loop exits
    // pop_top returns the huffman tree
    return heap.pop_top();
}
