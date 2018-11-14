#include <iostream>
#include <fstream>
#include <string>
#include "huffman.hh"
#include "bitio.hh"

//Helper function that encodes a single file
void encode_func(char* file)
{
    
    //First establishing our streams and files and checking that they're working
    std::ifstream input_file(file);
    std::string infile_name(file);//Get the name of our file as a string
    if (!input_file.is_open())
    {
        std::cerr << "It seems that the file '" + infile_name + "' can't be opened.";
        return;
    }
    std::ofstream output_file;
    output_file.open(infile_name + ".comp");
    if (!output_file.is_open())
    {
        std::cerr << "We can't seem to compress this file; if you see this, something has really gone wrong.";
        return;
    }
    
    //Next section simply goes through our input file character by character (byte by byte), then uses
    //huffman to get the compressed path to that character in bits. BitIO then pushes those bits
    //into our output file.
    Huffman huff;
    BitIO bit_io(&output_file, nullptr);//I honestly don't know why this works here, as it seems the destructor would be called
    //after our output file is closed, so we can't "flush" extra bits out. That being said, it does seem to work, so I guess all's well.
    char byte;
    while (input_file.get(byte))
    {
        Huffman::bits_t bit_path = huff.encode(byte);
        for (bool bit : bit_path)
        {
            bit_io.output_bit(bit);
        }
    }
    output_file.close();
}


//Checks there are an appropriate number of arguments, then encodes each one with a helper function
//In general, takes one or more files as input, and produces .comp compressed versions of those files.
int main(int arg_count, char** args)
{
    if (arg_count < 2)//arg_count counts itself so we make sure there is some other argument passed as well
    {
        std::cerr << "You must have an argument to use this program.";
        return -1;
    }
    int i = 1;
    while (i < arg_count)
    {
        encode_func(args[i]);
        i++;
    }
    return 0;
}






















