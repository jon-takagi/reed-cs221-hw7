# README
## Contents
1. Declarations
2. Implementations
3. Testing
4. Results

## Declarations
### hforest.hh
This file was provided by the instructor. No changes were made, despite their descriptions as "My solutions to HW6.", which had us build a maxheap. This file describes a minheap instead.
### htree.hh
This file defines a node for a tree. It was provided by the instructor and no changes were made. Also, the functions are implemented in the header file, which I'm pretty sure is a crime.
### bitio.hh
Eli wrote this one. Blame him.
### huffman.hh
This file defines the huffman class. A `huffman` object has two member functions as specified by the assignment: `encode` a character and get back a vector of booleans, and `decode`, which takes a single bit and returns the integer representation of the character it encodes.
It has one private method, which builds a huffman tree, and two private data members: the frequency table, stored as a vector of ints, and a pointer to the huffman tree it stores.
We chose to store the frequency table as a vector of integers because we use raw frequency rather than normalized frequency - every time we use a character, it goes up by one. Therefore, integers are sufficient, and simpler. We store a pointer to the huffman tree to use in the decode method.
## Implementations
### hforest.cc
This file is the instructor's implementation of `hforest` as assigned in hw5. Usefully, this defines a minheap, so `pop_top` returns the tree with the lowest value. This isn't the behavior I handed in for hw5, so I flipped it and then spent two hours trying to figure out why everything was backwards.
### huffman.cc
The design choices made in each method of the `huffman` class are described below.
#### Constructor
The constructor uses the `assign` function to set the initial frequency of every character to 0. It also begins by building a tree. This isn't strictly necessary, as encoding begins by rebuilding the tree, and if `root` is nullptr when decode is called (as it would be when the first call is to decode) then decode rebuilds the tree. However, this helped us avoid errors during debugging.
#### encode
This method relies on the `path_to` method of the HTree class, which was written by the instructor. I then convert the vector of `Direction` to a vector of `bool` using a very clever one line method. This was a mistake - I was using left = 1 in my encode when I wanted to use left = 0, and I didn't notice because I left myself a comment indicating otherwise. However, now it works, and it's still only 1 line :) Using `push_back` isn't the most efficient, but it saves me type problems and ensures that the vector stays in the right order.
#### decode
This method takes a bit, and traverses the tree to locate the symbol that it represents. If the single bit doesn't unambigously identify a symbol, it returns a negative value. If it returned a negative value, the next time it is called, then it attempts to parse the new bit as an addition to the previous sequence. If you pass a sequence that isn't there, it just loops and returns nonsense data, but won't crash. It WILL screw up your results, so don't screw up your results.
#### build_tree
This method is the meat of the class - while this method didn't work, it was impossible to debug errors in the other two. It follows the algorithm described in the assignment. We gave the dummy nodes negative keys starting at -257 and counting up to aid in debugging. They could also all have a key of -1, and the method would work the same way. Also, we chose to ensure that if l is a long tree rather than a node containing a symbol, it is on the left. This gives the tree a less efficient shape that was easier for us to visualize and debug. Allowing it to match left and right by the smallest value will give a smaller tree, giving smaller codes.
###bitio.cc
dunno shit about this - eli did this one.
## Testing
`verify.sh` takes two arguments, the first being the relative path to the directory containing *input* files in any text format and the second being a relative path to the directory containg *output* files with the extension `.comp.plaintext`. It expects every file in the output directory to have a matching source in the input directory. It uses the builtin `comp` program to compare the decompressed file and the corresponding source file. If any differences are found, it prints which file had errors and exits with error code 1. To test this script, I included several types of file, including one without any extension, to ensure that the script correctly matches the decompressed version and the original. The script was also tested against a file with spaces in the name to ensure correct behavior.
Also, the program fails gracelessly if the file being encoded contains a character not in the extended ASCII set. To prevent this, I used [this](https://pteo.paranoiaworks.mobi/diacriticsremover/) site to remove diacritics, which are common characters not in the ASCII set.
## Results
First, several test files of various lengths were chosen. The largest, the bee movie script, is 53Kb. The `encoder` program was run on each of the files located in `txt-files` directory. The encoded files are then created in the same directory and then passed to `decoder`. Because `verify` looks for `.comp.plaintext` files and matches them with their originals, the presence of the `.comp` files in `txt-files` doesn't cause errors. Note that encoding and decoding can be somewhat time consuming. Encoding `constitution.txt`, a 49Kb file, took about 1 minute on a 2017 macbook pro. Decoding the resulting file took approximately 56 seconds on the same machine.
|Filename                   | Original size (bytes) | Compressed size (bytes) | Percent of original|
|---------------------------|-----------------------|-------------------------|--------------------|
|bee-movie-script.txt       |52,755                 |31,409                   |59.40               |
|constitution.txt           |45,119                 |25,690                   |56.94               |
|Lab Ticket 1.md            | 1,135                 |   810                   |71.37               |
|If.txt                     | 1,523                 | 1,032                   |67.76               |
|never-gonna-give-you-up.txt| 1,758                 | 1,110                   |63.14               |
|torch-of-life              |   963                 |   737                   |76.53               |
