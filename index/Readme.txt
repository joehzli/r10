Hanzhou Li
PolyID: 0490250

Language
C++

How to Compile
This project is developed with XCode. Please use XCode to open this project to compile and run.

Program Running Folder Structure
|-- index // Compiled Program
|-- data // All the data files have to be put here, subfolders are supported. Also final outputs are here.
|-- tmp // Folder to hold the temporary files.

Steps of Generating Inverted Index
1. List all the files recursively in the data folder to get all the files' names.
2. Read each index file and parsed it. Use the parsed index to read each page from the data file. In this step, all the gzip files are uncompressed in memory. The tar file should be unpack first.
3. Use provided parser to parse each page to get (docID, word, position, context) tuples.
4. When the amount of (docID, word, position, context) tuples in memeory is more than a certain number, use merge sort(stable sort) to sort this array on docID and word. Then write the sorted array to a file.
5. Repeat above steps until all the data is parsed. We will get a bunch of temporary index files. The temporary files are written as text files, because I will use unix sort to merge them in the next step. The data in each temporary file is sorted. I don't use word ID so that I can generate the final inverted index in alphabet order.
6. Write the url table.
7. Use unix sort to merge all the sorted temporary index files into a one file.
8. Parsed the merged index file to reformat it as the final inverted index format. In this step, currently I set the maximum file size as 4GBytes, if more thant that, than write it to multiple files.
9. Write the lexicon table.

URL Table Output Format
(Length of URL, bin only) URL, fileID, pointer
- I don't record the docID in this file, since the URL is in order in the file.
- I don't record the size of the page of this URL in this file, since I can calculate it with two pointers.
- fileID is the number of the data file that contains this url.
- pointer points to the page in the data file.
- In binary file mode, the length of the URL is also record.

Lexicon Table Output Format
(Length of the word, bin only), Word, Number, fileID, invertedPointer
- Word is the word.
- Number is the total number of docs that containing this word.
- fileID is the number of the inverted index file containing this word's inverted index list.
- invertedPointer points to the inverted indexlist.
- In binary file mode, the length of the word is also record.

Inverted Index Output Format
docID, Number, pos1, context1, pos2, context2...... docID, Number, pos1, context1, pos2, context2...
- docID is compressed.
- Number is the the occurence of the word in this doc.
- position is compressed.

Compress
- Use difference of the docID for the same word to compress the docID
- Use difference of the position for the same word and same docID
- The URL in the URL table is in order, so don't need to record the the docID in URL table.

What Can be Configured
MAX_POSTING_PER_TMP_INDEX - The maximum items in each temporary index file. Over this number will write to a new file.
MAX_FILE_SIZE - Max size of final inverted index file. Over this number will write to a new file.
FILE_MODE - Write the output files in binary mode or ascii mode.

Result and Performance
Use the nz2.tar as the test data set. (I only own a MacbookAir and its remaining disk space is very small(about 5GBytes), so I haven't tested the full data set yet. I will find another machine to test it.)
Size of inverted index: 271MB
Cost time: 3.5 minutes

Limitation
The maximum of each inverted index file can't be larger than 4GBytes, since I use uint32_t as the pointer's data type.


