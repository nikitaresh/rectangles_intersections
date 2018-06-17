#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>


static bool writeToFile(std::fstream& file, int numRectangles)
{
    if( !file.is_open() ) {
        std::cout << "Error: unable to create output file" << std::endl;
        return false;
    }

    if( numRectangles <= 0 ) {
        std::cout << "Error: numRectangles <= 0" << std::endl;
        return false;
    }

    file << "{" << std::endl;
    file << "    \"rects\": [" << std::endl;

    for( int index = 0; index < numRectangles; ++index )
    {
        int x = 10 + 5 * (index % 100);
        int y = 10 + 200 * (index / 100);
        int w = 50;
        int h = 100;
        file << "        {\"x\": " << x << ", \"y\": " << y << ", \"w\": " << w 
             << ", \"h\": " << h << (index == numRectangles -1 ? " }" : " },") << std::endl;
    }

    file << "    ]" << std::endl;
    file << "}" << std::endl;

    return true;
}

int main( int argc, char* argv[] )
{
    if ( argc != 3 ) {
        std::cout << "Usage: generate_input <output_filename> <num_rectangles>" << std::endl;
        return 1;
    }

    std::string outputFilename( argv[1] );
    int numRectangles = 0;
    try {
        numRectangles = std::stoi( std::string(argv[2]) );
    }
    catch (...) {
        std::cout << "Error: wrong format of <num_rectangles>" << std::endl;
        return 1;
    }

    std::fstream file( outputFilename, std::fstream::out );
    bool writeResult = writeToFile(file, numRectangles);
    file.close();

    std::string message = writeResult ? "A file was recorded successful"
                                      : "An error while writing a file";
    std::cout << message << std::endl;

    return 0;
}