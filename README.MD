# SaCompGC Decompressor

This is a library to decompress .REL files with the "SaCompGC" header used in the Gamecube version of Sonic Adventure DX. The decompression code is based on a Ghidra decompilation of the function used in the original game and is provided as-is.

The code can be compiled as either a program or a DLL. The command line program can be used as follows:

`SACompGC InputFile.rel OutputFile.rel`

Original decompilation and code by @Exant64
