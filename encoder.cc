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
    output_file.open(infile_name + ".comp");
    if (!output_file.is_open())
    {
        std::cerr << "Something else is broken";
        return -3;
    }
    //Next section goes through input byte by byte and calls bitio on it
    BitIO bit_io(&output_file, nullptr);
    char byte;
    Huffman huff;//This will be our encoder
    while (input_file.get(byte))
    {
        Huffman::bits_t bit_path = huff.encode(byte);
        for (bool bit : bit_path)
        {
            bit_io.output_bit(bit);
        }
    }
    //output_file << bit_out_stream;//And now we jam all that binary into our file
    output_file.close();
    return 0;
}





