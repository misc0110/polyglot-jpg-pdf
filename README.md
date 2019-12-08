# Polyglot: JPG+PDF

A tool to combine a PDF and a JPG into one polyglot file which is both a PDF and JPG.
The tool is based on [Ange Albertini](https://twitter.com/@angealbertini)'s PoC from [PoC||GTFO 0x03](https://github.com/angea/pocorgtfo/blob/master/contents/issue03.pdf#page=8).

# Usage

Compile the tool by running

    make

Then, just run

    ./pdfjpg <pdf file> <jpg file> <output file>

to generate a file which is both a PDF and a JPG file. See PoC||GTFO 0x03 for more details.

# Limitations

Acrobat Reader since version 10 cannot open the resulting PDF.
