/*
 * test_huffman: test the Huffman class
 */

#include "huffman.hh"
#include <iostream>
#include <cassert>
#include <climits>
#include "hforest.hh"
#include <iomanip>
#include <fstream>
//////////////////////////////////////////////////////////////////////////////
// Ensure the encoder actually modifies the coding' length as expected
void test_encode()
{
    Huffman huff;

    auto bits = huff.encode('a');

    assert(bits.size() > CHAR_BIT);

    bits = huff.encode('b');
    assert(bits.size() > CHAR_BIT);

    bits = huff.encode('a');
    assert(bits.size() < CHAR_BIT);

    bits = huff.encode('b');
    assert(bits.size() < CHAR_BIT);

    bits = huff.encode('b');
    assert(bits.size() == huff.encode('a').size());

    bits = huff.encode('b');
    bits = huff.encode('b');
    bits = huff.encode('b');
    assert(bits.size() < huff.encode('a').size());
}

//////////////////////////////////////////////////////////////////////////////
// A utility function that takes a collection of bits and a Huffman tree, and
// figures out which symbol these bits encode.
int decode_symbol(Huffman& huff, const Huffman::bits_t& bits)
{
    int symbol = -1;

    for (auto b : bits) {
    symbol = huff.decode(b);
    }
    assert(symbol >= 0);
    return symbol;
}

//////////////////////////////////////////////////////////////////////////////
// Ensure decoding returns the same symbol that was encoded, in different
// scenarios.
void test_decode()
{
    Huffman encoder, decoder;

    auto bits = encoder.encode('a');
    assert(decode_symbol(decoder, bits) == 'a');
    bits = encoder.encode('a');
    assert(decode_symbol(decoder, bits) == 'a');
    bits = encoder.encode('b');
    assert(decode_symbol(decoder, bits) == 'b');
    bits = encoder.encode('a');
    assert(decode_symbol(decoder, bits) == 'a');
    bits = encoder.encode(Huffman::HEOF);
    assert(decode_symbol(decoder, bits) == Huffman::HEOF);
}

// checks if a node is a leaf. helper to verify shape
bool is_leaf(HTree::tree_ptr_t node) {
    HTree::Direction l = HTree::Direction::LEFT;
    HTree::Direction r = HTree::Direction::RIGHT;
    return node -> get_child(l) == nullptr && node -> get_child(r) == nullptr;
}
//recursively confirms that the tree is all left leaves
bool verify_shape(HTree::tree_ptr_t node) {
    HTree::Direction l = HTree::Direction::LEFT;
    HTree::Direction r = HTree::Direction::RIGHT;
    if(is_leaf(node -> get_child(l)) && is_leaf(node -> get_child(r))) {
        return true;
    }
    return is_leaf(node -> get_child(l)) && verify_shape(node -> get_child(r));
}
//pretty prints a tree, stolen from stackoverflow
void pretty_print(HTree::tree_ptr_t node, int indent=0) {
    std::ofstream huff_of;
    huff_of.open("huffman.txt", std::ios::app);
    HTree::Direction l = HTree::Direction::LEFT;
    HTree::Direction r = HTree::Direction::RIGHT;
    if(node != nullptr) {

        if(node -> get_child(l)) {
            pretty_print(node -> get_child(l), indent+4);
        }
        if(node -> get_child(r)) {
            pretty_print(node -> get_child(r), indent+4);
        }
        if(indent) {
             huff_of << std::setw(indent) << ' ';
        }
        huff_of<< node -> get_key() << "\n ";
    }
}
bool no_fake_leaves(HTree::tree_ptr_t node) {
    if(is_leaf(node)) {
        return node -> get_key() >= 0;
    } else {
        bool l = true;
        bool r = true;
        if(node->get_child(HTree::Direction::LEFT)) {
        	l = l && no_fake_leaves(node->get_child(HTree::Direction::LEFT));
        }
        if (node->get_child(HTree::Direction::RIGHT)) {
        	r = r && no_fake_leaves(node->get_child(HTree::Direction::RIGHT));
        }
        bool res = l && r;
        return res;
    }
}
void test_build_tree() {
    // HForest heap({});
    // heap.add_tree(HTree::tree_ptr_t(new HTree(0, 0)));
    // heap.add_tree(HTree::tree_ptr_t(new HTree(1, 1)));
    // heap.add_tree(HTree::tree_ptr_t(new HTree(2, 2)));
    // heap.add_tree(HTree::tree_ptr_t(new HTree(3, 3)));
	// HTree::tree_ptr_t l = heap.get_min(); //pop the lowest value off the heap
	// HTree::tree_ptr_t r = heap.get_min(); //pop the second lowest off the heap
    // // std::cout << l -> get_key() << "\n";
    // // std::cout << r -> get_key() << "\n";
    // int temp_val = l->get_value() + r->get_value();
    // HTree::tree_ptr_t temp = HTree::tree_ptr_t(new HTree(-1, temp_val,l,r));
    // heap.add_tree(temp);
    //
    // l = heap.get_min();
    // r = heap.get_min();
    // temp_val = l->get_value() + r->get_value();
    // temp = HTree::tree_ptr_t(new HTree(-1, temp_val,l,r));
    // heap.add_tree(temp);
    //
    // l = heap.get_min();
    // r = heap.get_min();
    // temp_val = l->get_value() + r->get_value();
    // temp = HTree::tree_ptr_t(new HTree(-1, temp_val,l,r));
    // heap.add_tree(temp);
    //
    // std::cout <<"heap size: " << heap.size() << "\n";
    // std::cout << "popping" << "\n";
    // auto root = heap.pop_top();
    // std::cout <<"heap size: " << heap.size() << "\n";

    Huffman huff;
    // HTree::Direction l = HTree::Direction::LEFT;
    // HTree::Direction r = HTree::Direction::RIGHT;
    // std::cout << "root   :" << huff.get_root() -> get_key()<< "\n";
    // std::cout << "l      :" << huff.get_root() -> get_child(l) -> get_key()<< "\n";
    // std::cout << "r      :" << huff.get_root() -> get_child(r) -> get_key()<< "\n";
    // std::cout << "r l    :" << huff.get_root() -> get_child(r) -> get_child(l) -> get_key()<< "\n";
    // std::cout << "r r    :" << huff.get_root() -> get_child(r) -> get_child(r) -> get_key()<< "\n";
    // std::cout << "r r l  :" << huff.get_root() -> get_child(r) -> get_child(r)  -> get_child(l) -> get_key()<< "\n";
    // std::cout << "r r r  :" << huff.get_root() -> get_child(r) -> get_child(r)  -> get_child(r) -> get_key()<< "\n";
    // std::cout << "r r r r:" << huff.get_root() -> get_child(r) -> get_child(r)  -> get_child(r) -> get_child(r) -> get_key()<< "\n";
    // std::cout << "r r r l:" << huff.get_root() -> get_child(r) -> get_child(r)  -> get_child(r) -> get_child(l) -> get_key()<< "\n";

    // auto path = huff.get_root() -> path_to(3);
    // Huffman::bits_t bin_path = {};
    // for(HTree::Direction d : path) {
    //     if(d == l) {
    //         std::cout << "l ";
    //         bin_path.push_back(true);
    //     } else {
    //         std::cout <<"r ";
    //         bin_path.push_back(false);
    //     }
    // }
    std::cout <<"\n";
    // if(verify_shape(huff.get_root())) {
    //     std::cout << "shape verified" <<"\n";
    // } else {
    //     std::cout << "wrong shape" <<"\n";
    // }
    // std::cout <<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" <<"\n";
    pretty_print(huff.get_root());
    if(no_fake_leaves(huff.get_root())) {
        std::cout << "no fake leaves" << "\n";
    } else {
        std::cout << "found leaves with negative key" << "\n";
    }
}
//////////////////////////////////////////////////////////////////////////////
// void cout_test_decode() {
//     Huffman encoder, decoder;
//
//     auto bits = encoder.encode('a');
//     for(bool b:bits) {
//         std::cout << b;
//     }
//     std::cout <<"\n";
//
//     Huffman huff;
//     auto huff_bits = huff.encode('a');
//
//     for(bool b:huff_bits) {
//         // std::cout << (huff.decode(b)) << "\n";
//     }
//     std::cout <<"\n";
//
//
// }
int main()
{
    // test_build_tree();

  // test_encode();
  test_decode();
  return 0;
}
