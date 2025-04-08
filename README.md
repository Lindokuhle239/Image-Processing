## File Structure

Assign3/
|---Assignment Resources/
| |--Birds_Colours.png
| |--Birds.png
| |--Chess_Colours.png
| |--Shapes_Colours.png
| |--Shapes.png
| |--Values.txt
|---pgm_file/
| |--Birds_Colours.pgm
| |--Birds.pgm
| |--Chess_Colours.pgm
| |--Shapes_Colours.pgm
| |--Shapes.pgm
|---ppm_files/
| |--Birds_Colours.ppm
| |--Birds.ppm
| |--Chess_Colours.ppm
| |--Shapes_Colours.ppm
| |--Shapes.ppm
|---src/
| |--catch.hpp
| |--ConnectedComponents.h
| |--main.cpp
| |--PGMimageProcessor.cpp
| |--PGMimageProcessor.h
| |--PPMimageProcessor.cpp
| |--PPMimageProcessor.h
| |--test.cpp
|---Makefile
|---README.md

## Compilation

make            #Build main executable (findcomp)
make tests      #Builds and runs unit tests
make clean      #Removes build artifacts

##  Usage

# Full processing pipeline
./findcomp [options] <inputImage>

# Example:
    ./findcomp -t <threshold> -m <minValidSize> -w <output_PGM_File> <input_PGM_File>
    ./findcomp -t 128 -m 1 -w Shapes.pgm pgm_files/Shapes.pgm

# Mastery Work:
convert input.pgm -type truecolor input.ppm     # First convert PGM->PPM
./findcomp -t 128 -b output.ppm input.ppm