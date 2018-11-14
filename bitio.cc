#include "bitio.hh"
#include <cassert>

// Construct with one of either an input stream or output (the other null)
//constructor checks and assigns as either in or out, also establishes pending
//and count and initializes them to zero.
BitIO::BitIO(std::ostream* os, std::istream* is)
{
    assert ((os && !is) || (!os && is));//Effectively an XOR on os, is
    instream_ = is;
    outstream_ = os;
    pending_ = static_cast<char>(0);//Sets pending to its default, with binary representation = 0. ( = 00000000).
    count_ = 0;//We count left to right, starting at zero, for the bit in pending_ that we are currently working on
}

// Flushes out any remaining output bits and trailing zeros, if any:
//Nothing else to be done since all data memembers are variables in 
//stack or are smart data types
BitIO::~BitIO()
{
    if (outstream_)
    {
        if (count_ > 0)//unless we just finished a byte, spew out what we had stored
        {
            outstream_->put(pending_);
        }
    }
}


// Output a single bit (buffered)
//stores an input boolean s a bit into out pending_ character,
//then pushes those bits into outsteam once we hit 8.
void BitIO::output_bit(bool bit)
{
    assert (outstream_);
    unsigned int MASK = 1; 
    MASK = MASK << (7 - count_);//get the 1 in the count place in the binary representation
    if (bit)
    {
        pending_ = (pending_ | MASK);//set the count place of pending to 1
    }
    else
    {
        pending_ = (pending_ & ~MASK);//set count place to 0.
    }
    count_++;
    if (count_ > 7)//If we've finished the byte, then flush out the character
    {
        outstream_->put(pending_);
        pending_ = static_cast<char>(0);
        count_ = 0;
    }
}
        

// Read a single bit (or trailing zero)
//Reads in a single bit from the instream and returns
//that bit as a boolean
bool BitIO::input_bit()
{
    assert (instream_);
    if (count_ == 0)
    {
        instream_->get(pending_);
    }
    unsigned int MASK = 1;
    MASK = MASK << (7-count_);//same process as before
    count_++;
    unsigned int ret_pending = pending_;//this is what we return for pending so that we can set it to zero before returning without screwing anything up
    if (count_ > 7)
    {
        pending_ = static_cast<char>(0);
        count_ = 0;
    }
    unsigned int temp_bit = ret_pending & MASK;//Extracts tbe count position bit of pending
    return temp_bit;
}











