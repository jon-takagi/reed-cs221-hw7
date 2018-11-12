#define CATCH_CONFIG_MAIN //Have catch provide our main function
#include "catch.hpp"
#include "htree.hh"

TEST_CASE( "Trees! Trees should be able to store data, and all four relevant functions should work.", "[Tree]" ) 
{
    /*First we set up our trees. We build from the leaves up, so the code looks a bit strange, but it follows:
    Tree tree_key_identity is a tree with key "key" and an identifying integer, which is then assigned to value,
    because value is not actually used in the tree structure anywhere but is still part of it. 
    Once we have built our tree, we should end up with something like:
            126
           /   \
          /     \
         4       7
        / \       \
      -3   2       5
      / \   \     / \
     4   5   7   0   4
     
    Which should be sufficient to test all of our features.
    [This could have been made better/more modular by replacing all the values in the tree with variables, thus 
    allowing for easy editing of the values in the tree; but it's too late now and the current values are (probably) fine.
    */
    HTree::tree_ptr_t tree_4_01  (new HTree(  4,  1));
    HTree::tree_ptr_t tree_5_02  (new HTree(  5, 12));
    HTree::tree_ptr_t tree_n3_03 (new HTree( -3,  3,  tree_4_01,  tree_5_02));//n here stands for negative, can't use the - sign in the name or else it causes an error
    HTree::tree_ptr_t tree_7_04  (new HTree(  7,  4));
    HTree::tree_ptr_t tree_2_05  (new HTree(  2,  5,    nullptr,  tree_7_04));
    HTree::tree_ptr_t tree_4_06  (new HTree(  4,  6, tree_n3_03,  tree_2_05));
    HTree::tree_ptr_t tree_0_07  (new HTree(  0,  7));
    HTree::tree_ptr_t tree_4_08  (new HTree(  4,  8));
    HTree::tree_ptr_t tree_5_09  (new HTree(  5,  9,   tree_0_07, tree_4_08));
    HTree::tree_ptr_t tree_7_10  (new HTree(  7, 10,     nullptr, tree_5_09));
    HTree::tree_ptr_t tree_126_11(new HTree(126, 11,   tree_4_06, tree_7_10));
    
    
    
    SECTION( "Data used in tree creation should be assigned properly." )
    {
        //This section just tests to make sure that everything we set up actually went into the intended values, and that the trees exist.
        REQUIRE( tree_n3_03->get_key() == -3);
        REQUIRE( tree_4_01->get_value() == 1);
        REQUIRE( tree_2_05->get_child(HTree::Direction::LEFT) == nullptr);
        REQUIRE( tree_126_11->get_child(HTree::Direction::RIGHT) == tree_7_10);
    }
    
    SECTION( "Path_to function should return a path in terms of L and R to the leftmost of a given key in the tree." )
    {
        //This section tests that path_to functions as expected
        REQUIRE( tree_126_11->path_to(4) == HTree::path_t {HTree::Direction::LEFT, HTree::Direction::LEFT, HTree::Direction::LEFT} );
        REQUIRE( tree_126_11->path_to(5) == HTree::path_t {HTree::Direction::LEFT, HTree::Direction::LEFT, HTree::Direction::RIGHT});
        REQUIRE( tree_126_11->path_to(2) == HTree::path_t {HTree::Direction::LEFT, HTree::Direction::RIGHT});
        REQUIRE( tree_126_11->path_to(7) == HTree::path_t {HTree::Direction::LEFT, HTree::Direction::RIGHT, HTree::Direction::RIGHT});
        REQUIRE( tree_126_11->path_to(0) == HTree::path_t {HTree::Direction::RIGHT, HTree::Direction::RIGHT, HTree::Direction::LEFT});
        REQUIRE( tree_126_11->path_to(126) == HTree::path_t {});
        //Next we test a few other paths, this time starting from somewhere other than the root of our tree
        REQUIRE( tree_4_06->path_to(4) == HTree::path_t {HTree::Direction::LEFT, HTree::Direction::LEFT});
        REQUIRE( tree_4_06->path_to(7) == HTree::path_t {HTree::Direction::RIGHT, HTree::Direction::RIGHT});
        REQUIRE( tree_7_10->path_to(4) == HTree::path_t {HTree::Direction::RIGHT, HTree::Direction::RIGHT});
    }
}