/**
 * Author: Daniel Dusek, xdusek21
 * Brno, University of Technology
 * BMS class of 2017/2018, Project #2
 */
#include <iostream>
#include <cstdlib>
#include <math.h>

#include "sndfile.hh"
#include "bms2B.h"

int main(int argc, char** argv) 
{
    // Check parameters
    std::string inputFileName = retrieveInputFileName(argc, argv);
    if (inputFileName.compare(EMPTY_STRING) == 0)
    {
        std::cerr << "Please specify input file as the first parameter.\n";
        return EXIT_FAILURE_PARAMS;
    }

    // Extract information about input file 
    SndfileHandle inputFile = SndfileHandle(inputFileName);
    int sampleRate = inputFile.samplerate();
    double freq = (1000.0 / sampleRate);
    int channels = inputFile.channels();
    int format = inputFile.format();
    double amplitude = (1.0 * 0x7F000000);
    int numberOfSamples = inputFile.frames();

    int *buffer = new int[sampleRate];

    std::cout << "Discovered values: \n" << "\tSample rate: " << sampleRate;
    std::cout << "\n\tFrequency: " << freq << "\n\tChannels: " << channels;
    std::cout << "\n\tFormat: " << format << "\n\tAmplitude: " << amplitude;
    std::cout << "\n\tSamples: " << numberOfSamples << "\n";  

    inputFile.read(buffer, sampleRate);
    


    
    delete [] buffer;
    return EXIT_SUCCESS;
}


/**
 * Retrieves input file name from parameters passed to application.
 *
 * int argc Number of parameters with which the application was called.
 * char** argv Array of parameters provided on input.
 * This implementation is very similar to the processParameters/2 function
 * I implemented in Project #1 (this is a anti auto-plagiarism remark). 
 *
 * return std::string Input file passed from parameters.
 */
std::string retrieveInputFileName(int argc, char *argv[])
{
    // Terminate execution if no input file is specified
    if (argc < 2)
    {
        return std::string(EMPTY_STRING);
    }

    return std::string(argv[1]);
}