How to run and use encode and decode.
================================================

What does encode do?
------------------
> Compresses the input file (default stdin) using huffman encoding. The resulting compressed file is then written to output (default stdout).
> 
> If the provided output file does not already exist, it will be created by the program.
>
What does decode do?
------------------
> Decompresses the compressed input file (default stdin), reversing the effect of the huffman encoding process. 
> The resulting uncompressed file is written to the output (default stdout).
>
> If the provided output file does not already exist, it will be created by the program.
>


Command Line Options
--------------------
>encode
>------------------
>Usage: ./encode [options]\
>
>  A Huffman encoder.\
>  Compresses a file using the Huffman coding algorithm.\
>\
>USAGE\
>  ./encode [-h] [-i infile] [-o outfile]\
>\
>OPTIONS\
>  -h             Program usage and help.\
>  -v             Print compression statistics.\
>  -i infile      Input file to compress.\
>  -o outfile     Output of compressed data.\
>
>
>
>decode
>------------------
>  A Huffman decoder.\
>  Decompresses a file using the Huffman coding algorithm.\
>\
>USAGE\
>  ./decode [-h] [-i infile] [-o outfile]\
>\
>OPTIONS\
>  -h             Program usage and help.\
>  -v             Print compression statistics.\
>  -i infile      Input file to decompress.\
>  -o outfile     Output of decompressed data.\




How to build and run the program.
=================================

Step One:
---------
> Ensure that the files:

> "encode.c"\
> "decode.c"\
> "defines.h"\
> "header.h"\
> "node.h"\
> "node.c"\
> "pq.h"\
> "pq.c"\
> "code.h"\
> "code.c"\
> "io.h"\
> "io.c"\
> "stack.h"\
> "stack.c"\
> "huffman.h"\
> "huffman.c"\
> "Makefile"\
>
> Ensure all the files are inside the same directory.
>
Step Two:
---------
> Open the terminal in the directory and type "make"
> Alternatively you can type "make all" to make all the executables,
> or "make encode" or "make decode" to make a specific 
>
Step Three:
-----------
> Use the usage methods of ./encode and ./decode are located above in the "Command Line Options" section
>
> You may also use the -h with the program to print the usage to the terminal instead
> of looking through the README

Cleanup:
-----------
> Type "make clean" to delete all derived files except the created binaries.
> type "make spotless" to delete all derived files includeing the created binaries.
