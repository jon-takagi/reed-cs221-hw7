#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "huffman.hh"
#include "bitio.hh"

//This helper function takes a file as input and produces a .plaintext of that file, which 
//will be a decompression of the .comp of that file. If that's phrased oddly it's because this
//can also be applied to other file extensions, and will produce a terrible mess (func does
//not check what file extensions you feed it).
void decode_func(char* file)
{
    std::ifstream input_file(file);
    std::string infile_name(file);//Gets the name so we can use it in our decoded file name
    if (!input_file.is_open())
    {
        std::cerr << "It seems that the file '" + infile_name + "' can't be opened.";
        return;
    }
    std::ofstream output_file;
    output_file.open(infile_name + ".plaintext");
    if (!output_file.is_open())
    {
        std::cerr << "We can't seem to decompress this file; if you see this, something's broken and I don't know what it is.";
        return;
    }
    //Next section goes through input bit by bit through bitio, then feeds each bit into huffman
    //and huffman then pushes to the outfile when it has a completed character.
    Huffman huff;//This will be our decoder
    BitIO bit_io(nullptr, &input_file);//The same case as encoder for where the destruction of this object occurs;
    //apparently it still works.
    while (input_file)
    {
        bool bit = bit_io.input_bit();
        int symbol = huff.decode(bit);
        if (symbol >= 0)
        {
            output_file << char(symbol);
        }        
    }
    output_file.close();
    return;
}


//This function takes one or more files as arguments and produces .plaintext decompressions of them.
//It does this by first checking arguments then iterating through file arguments and calling a decoder
//helper function on each one.
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
        decode_func(args[i]);
        i++;
    }
    return 0;
}















