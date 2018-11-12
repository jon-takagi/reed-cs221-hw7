#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "huffman.hh"
#include "bitio.hh"

int main(int arg_count, char** args)
{
    if (arg_count < 2)//arg_count counts itself so we make sure there is some other argument passed as well
    {
        std::cerr << "You must have an argument to use this program.";
        return -1;
    }
    std::ifstream input_file(args[1]);
    if (!input_file.is_open())
    {
        std::cerr << "Something is broken";
        return -2;
    }
    std::ofstream output_file;
    std::string infile_name(args[1]);
    output_file.open(infile_name + ".plaintext");
    if (!output_file.is_open())
    {
        std::cerr << "Something else is broken";
        return -3;
    }
    //Next section goes through input byte by byte and calls bitio on it
    BitIO bit_io(nullptr, &input_file);
    //char byte;
    Huffman huff;//This will be our decoder
    while (input_file)
    {
        bool bit = bit_io.input_bit();
        int symbol = huff.decode(bit);
        if (symbol >= 0)
        {
            output_file << char(symbol);
        }        
    }
    //output_file << bit_out_stream;//And now we jam all that binary into our file
    output_file.close();
    return 0;
}