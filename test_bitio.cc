/*
 * Unit tests for class BitIO
 */

#include "bitio.hh"
#include <cassert>
#include <sstream>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////
void
test_1_bit()
{
  std::stringstream bits;
  {
    BitIO bitio(&bits, nullptr);
    bitio.output_bit(1);
  }
  {
    BitIO bitio(nullptr, &bits);
    assert(bitio.input_bit() == true);
    assert(bitio.input_bit() == false);  // Should just be a trailing zero
  }
}


//////////////////////////////////////////////////////////////////////////////
void
test_4_bits()
{
  std::stringstream bits;
  {
    BitIO bitio(&bits, nullptr);
    bitio.output_bit(0);
    bitio.output_bit(1);
    bitio.output_bit(0);
    bitio.output_bit(1);
  }
  {
    BitIO bitio(nullptr, &bits);
    assert(bitio.input_bit() == false);
    assert(bitio.input_bit() == true);
    assert(bitio.input_bit() == false);
    assert(bitio.input_bit() == true);
    assert(bitio.input_bit() == false);  // Should just be a trailing zero
  }
}

//////////////////////////////////////////////////////////////////////////////
void
test_8_bits()
{
  std::stringstream bits0, bits1;
  {
    BitIO bitio0(&bits0, nullptr);
    BitIO bitio1(&bits1, nullptr);
    for (int i = 0; i < 8; ++i) {
      bitio0.output_bit(0);
    }
    for (int i = 0; i < 8; ++i) {
      bitio1.output_bit(1);
    }
  }
  {
    BitIO bitio0(nullptr, &bits0);
    BitIO bitio1(nullptr, &bits1);
    for (int i = 0; i < 8; ++i) {
      assert(!bitio0.input_bit());
    }
    bitio0.input_bit();
    assert(bits0.eof()); // Should be no trailing zeros this time!

    for (int i = 0; i < 8; ++i) {
      assert(bitio1.input_bit());
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
void
test_9_bits()
{
  std::stringstream bits0, bits1;
  {
    BitIO bitio1(&bits1, nullptr);
    for (int i = 0; i < 9; ++i) {
      bitio1.output_bit(1);
    }
  }
  {
    BitIO bitio1(nullptr, &bits1);
    for (int i = 0; i < 9; ++i) {
      assert(bitio1.input_bit());
    }
    assert(!bitio1.input_bit());
  }
}

//////////////////////////////////////////////////////////////////////////////
void
test_16_bits()
{
  std::stringstream bits;
  {
    BitIO bitio(&bits, nullptr);
    for (int i = 0; i < 16; ++i) {
      bitio.output_bit(i % 2);
    }
  }
  {
    BitIO bitio(nullptr, &bits);
    for (int i = 0; i < 16; ++i) {
      assert(bitio.input_bit() == i % 2);
    }
    bitio.input_bit();
    assert(bits.eof()); // Should be no trailing zeros this time!
  }
}

//////////////////////////////////////////////////////////////////////////////
void
test_100_bits()
{
  std::stringstream bits;
  {
    BitIO bitio(&bits, nullptr);
    for (int i = 0; i < 100; ++i) {
      bitio.output_bit(!(i % 2));
    }
  }
  {
    BitIO bitio(nullptr, &bits);
    for (int i = 0; i < 100; ++i) {
      assert(bitio.input_bit() != i % 2);
    }
    assert(!bitio.input_bit());
  }
}


//////////////////////////////////////////////////////////////////////////////
//tests the case of (11110000|1010) and its inverse (00001111|0101)- Not a multiple of eight bits, and 
//importantly tests a change from 0 to 1 across the "byte gap," to ensure
//that the byte gap is actually being handled properly.
void edge_case_1()
{
  std::stringstream bits0, bits1;
  {
    BitIO bitio1(&bits1, nullptr);
    for (int i = 0; i < 4; ++i) {
      bitio1.output_bit(1);
    }
    for (int i = 0; i < 4; ++i) {
      bitio1.output_bit(0);
    }
    bitio1.output_bit(1);
    bitio1.output_bit(0);
    bitio1.output_bit(1);
    bitio1.output_bit(0);
  }
  {
    BitIO bitio1(nullptr, &bits1);
    for (int i = 0; i < 4; ++i) {
      assert(bitio1.input_bit());
    }
    for (int i = 0; i < 4; ++i) {
      assert(!bitio1.input_bit());
    }
    assert(bitio1.input_bit());
    assert(!bitio1.input_bit());
    assert(bitio1.input_bit());
    assert(!bitio1.input_bit());
  }
  //Now testing inverse
    {
    BitIO bitio0(&bits0, nullptr);
    for (int i = 0; i < 4; ++i) {
      bitio0.output_bit(0);
    }
    for (int i = 0; i < 4; ++i) {
      bitio0.output_bit(1);
    }
    bitio0.output_bit(0);
    bitio0.output_bit(1);
    bitio0.output_bit(0);
    bitio0.output_bit(1);
  }
  {
    BitIO bitio0(nullptr, &bits0);
    for (int i = 0; i < 4; ++i) {
      assert(!bitio0.input_bit());
    }
    for (int i = 0; i < 4; ++i) {
      assert(bitio0.input_bit());
    }
    assert(!bitio0.input_bit());
    assert(bitio0.input_bit());
    assert(!bitio0.input_bit());
    assert(bitio0.input_bit());
  }
}
//////////////////////////////////////////////////////////////////////////////
int
main() {
  test_1_bit();
  test_4_bits();
  test_8_bits();
  test_9_bits();
  test_16_bits();
  test_100_bits();
  void edge_case_1();

  return 0;
}
