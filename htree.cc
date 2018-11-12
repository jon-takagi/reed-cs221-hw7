#include "htree.hh"

HTree::path_t HTree::path_to(int key) const
  {
    assert(find(key));

    path_t ret;
    if (key_ != key) {
      if (left_ && left_->find(key)) {
        ret = left_->path_to(key);
        ret.push_front(Direction::LEFT);
      } else {              // key must be in right_
        ret = right_->path_to(key);
        ret.push_front(Direction::RIGHT);
      }
    }
    return ret;
  }
  
// Return true IFF the given key is contained somewhere in my tree
bool HTree::find(int key) const
  {
    return key_ == key
       || (left_ && left_->find(key))
       || (right_ && right_->find(key));
  }
  
HTree::tree_ptr_t HTree::get_child(Direction dir) const
  {
    return (dir == Direction::LEFT)? left_ : right_;
  }