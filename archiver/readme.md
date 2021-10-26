# Archiver

In this task, you will implement an archiver using the [Huffman algorithm] (https://en.wikipedia.org/wiki/Huffman_coding).

The archiver program must have the following command line interface:
* `archiver -c archive_name file1 [file2 ...]` - archive files `fil1, file2, ...` and save the result to file `archive_name`.
* `archiver -d archive_name` - unzip files from the archive` archive_name` and put them in the current directory. File names must be preserved when zipping and unzipping.
* `archiver -h` - display help on using the program.

## Algorithm

The compression algorithm is structured as follows:
1. The frequency of 8-bit characters in the file is calculated. In addition to the contents of the file, it is necessary to take into account the frequency of characters in the file name, and also add three service characters `FILENAME_END = 256`,` ONE_MORE_FILE = 257`, `ARCHIVE_END = 258` with frequencies of 1. The purpose of these characters will be described later. Thus, 9 bits are needed to encode the extended alphabet.
1. A binary [boron] (https://en.wikipedia.org/wiki/Trie) encoding is constructed by the following procedure:
    1. For each character of the alphabet, a corresponding vertex is added to the priority queue. The ordering of the vertices in the queue is carried out according to the non-decreasing frequency of characters in the file (at the "head" of the queue there is always the vertex with the symbol with the least occurrence in the file).
    1. While there is more than one element in the queue, two vertices A and B with minimum priorities are sequentially retrieved from it. A new vertex C is created, the children of which are vertices A and B.
       Vertex C is placed in a queue with a priority equal to the sum of the priorities of vertices A and B.
    1. At the end of the procedure, exactly one vertex remains in the queue, which is the root of the constructed bore. Leaf tops are terminal.
       Each terminal vertex contains a symbol from the source file.
       Each nonterminal vertex of the tree contains two edges: left and right, which are marked with bits 0 and 1, respectively.
       Each terminal vertex corresponds to a bit sequence obtained by descending from the root of the boron to the terminal vertex and writing out the bits of all traversed edges.
       For clarity, you can use the following illustrations:
    * [gif demo] (https://commons.wikimedia.org/wiki/File:Huffmantree_ru_animated.gif?uselang=ru)
    * [gif demo] (https://commons.wikimedia.org/wiki/File:Huffman_huff_demo.gif)
    * [graphic demo] (https://people.ok.ubc.ca/ylucet/DS/Huffman.html).
1. All symbols are assigned a binary code sequence by means of the constructed boron.
1. The code is converted to [canonical form] (https://en.wikipedia.org/wiki/Canonical_Huffman_code). The canonical form of the code differs in that it allows you to unambiguously recover codes from the list of symbols and the lengths of the codes for them. There is an algorithm for restoring the canonical code [in Wikipedia] (https://en.wikipedia.org/wiki/Canonical_Huffman_code).
1. All characters of the file are replaced with the corresponding code binary sequences, and the result is written together with auxiliary information into the file. The archive file format is described below.

The decoding algorithm is generally the opposite of the encoding algorithm and is structured as follows:
1. The coding table (correspondence between sivols and their codes) is restored from the file.
1. A binary boron is constructed from the coding table.
1. Binary sequences read from the input file are traced along the bore from the root to the leaf vertices. When the next leaf vertex of the boron is reached, the corresponding symbol is determined, which is written to the output file.

## File format
Nine-bit values are written in low-endian format (analogous to little-endian for bits). Those. first comes the bit corresponding to `2 ^ 0`, then -` 2 ^ 1`, etc. to the bit corresponding to `2 ^ 8`. For a program to be portable, you cannot rely on the order of the bits in the file to match the order of the bits in the binary representation of the numbers in memory.

The archive file must have the following format:
1.9 bits - the number of characters in the alphabet `SYMBOLS_COUNT`
1. Data block for recovering the canonical code
1.` SYMBOLS_COUNT` values ​​of 9 bits - alphabet characters in the order of canonical codes
1. A list of `MAX_SYMBOL_CODE_SIZE` values ​​of 9 bits, the i-th (when numbered from 0) element of which is the number of characters with the code length` i + 1`. `MAX_SYMBOL_CODE_SIZE` - maximum code length in the current encoding. The canonical codes correspond in order to the characters from the previous paragraph. `MAX_SYMBOL_CODE_SIZE` is not written explicitly to the file, because it can be restored from the available data.
1. The encoded file name
1. The encoded service symbol `FILENAME_END`
1. The encoded content of the file
1. If there are more files in the archive, then the encoded service symbol `ONE_MORE_FILE` and the encoding continues from item 1.
1. The encoded service symbol `ARCHIVE_END`.

## Implementation
Try to make all the components of the program as possible
and more universal and not tied to the specifics of a specific task.
For example, encoding and decoding algorithms should work with I / O streams, not files.

The program must correctly process very large (many times larger than the RAM) files. This means that when the program is running, data should not accumulate in memory.

Files with a format that does not correspond to the specification should not lead to a crash of the program. All exceptions must be correctly handled with a user-understandable error message.

You will most likely need the following components:
- Wrappers over I / O streams that allow reading and writing values ​​of 1 and 9 bits.
- Bor with an interface that allows you to conveniently bypass it.
- Priority queue.

It is recommended to discuss the detailed design of the program with the teacher at the seminars.

## Submission and Grading

The project is checked by teachers without using an automatic verification system. All submissions will go through a code review.

You have to write the entire program from scratch, including the `CMakeLists.txt` files.

Unlike usual homework, the project must be committed / pushed to the repository manually, without using the `submit.py` script.

To submit a project, you need to create a new branch `projects / archiver` (it is executed once, when re-submitting the code, you do not need to create a branch):
`git checkout -b projects / archiver`

To commit changes to a branch, use the `git add` and` git commit` commands. To push changes to the remote repository, run the command `git push student projects / archiver`. Try to write meaningful commit notes.

After uploading the code to the remote repository, create a merge request in it from the `projects / archiver` branch in` main` and set your teacher as the reviewer.
