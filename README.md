# Image Component Analyzer

A C++ program for detecting and analyzing connected components in PGM/PPM images.

## Project Overview
This implementation processes grayscale (PGM) and color (PPM) images to:
- Identify 4-connected components using thresholding
- Filter components by size
- Generate output images highlighting retained components
- Draw bounding boxes around components (PPM only)
- Provide component statistics (count, sizes)

## Key Features
- **PGM Processing**: Threshold-based component extraction
- **PPM Support**: Color image handling (Mastery Work)
- **Bounding Boxes**: Visual markup of components (Mastery Work)
- **CLI Interface**: Flexible command-line control
- **Unit Tests**: Validation of core functionality

## File Structure

├── src/

│ ├── ConnectedComponent.[h/cpp] # Stores component data (pixels, ID, count)

│ ├── PGMimageProcessor.[h/cpp] # Processes PGM images

│ ├── PPMimageProcessor.[h/cpp] # Handles PPM images + bounding boxes

│ ├── main.cpp # Command-line interface

│ └── test.cpp # Unit tests (Catch2)

├── Makefile # Build configuration

└── pgm_test_files/ # Sample images


## Installation & Usage
```bash
# Clone repository
git clone https://github.com/yourusername/image-component-analyzer.git
cd image-component-analyzer

# Build program
make

# Basic usage
./findcomp -t <threshold> <input_image>

# Example: Process PGM with threshold 128
./findcomp -t 128 -w output.pgm pgm_test_files/Shapes.pgm

# Mastery Work: Process PPM with bounding boxes
convert input.pgm input.ppm  # Convert to PPM first
./findcomp -t 128 -b boxes.ppm input.ppm

## Command Options
Option		Description
-t N		Threshold (0-255)
-m N		Minimum component size
-f MIN MAX	Size range filter
-w FILE		Write output image
-b FILE		Draw bounding boxes (PPM only)
-p			Print component stats

## Requirements
- C++17 compiler (GCC/Clang)
- GNU Make
- ImageMagick (for conversions - sudo apt install imagemagick)

## Known Limitations
- Input PPMs must use P6 binary format
- Bounding boxes require PPM output
