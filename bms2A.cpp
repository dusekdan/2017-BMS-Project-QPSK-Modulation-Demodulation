/**
 * Author: Daniel Dusek, xdusek21
 * Brno, University of Technology
 * BMS class of 2017/2018, Project #2
 */
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <fstream>
#include <sstream>

#include "sndfile.hh"
#include "bms2A.h"

int main(int argc, char *argv[]) 
{
    int lastTick = 0;

    // Check parameters
    std::string inputFile = retrieveInputFileName(argc, argv);
    if (inputFile.compare(EMPTY_STRING) == 0)
    {
        std::cerr << "Please specify input file as the first parameter.\n";
        return EXIT_FAILURE_PARAMS;
    }

    // Open and read input text file
    std::ifstream file(inputFile);
    if (!file.is_open())
    {
        std::cerr << "Inptu file can not be opened.\n";
        return EXIT_FAILURE_INPUTFILE;
    }

    // Prepare output file
    SndfileHandle outputFile;
    outputFile = SndfileHandle(inputFile.substr(0, inputFile.find_last_of(".")).append(".wav"), SFM_WRITE, FORMAT, CHANNELS, SAMPLE_RATE);

    // Modulate sync sequence and then data from input file
    std::stringstream sequence(SYNC_SEQUENCE);
    QPSKModulation(&sequence, lastTick, outputFile);
    QPSKModulation(&file, lastTick, outputFile);

    return EXIT_SUCCESS;
}


/**
 * Modulates input stream using QPSK modulation.
 *
 * Function reads input stream by two characters (by which it couples them 
 * before modulation starts.
 * Based on whether these values are 00, 01, 10 or 11 function proceeds to
 * calculate corresponding cos(x + shift) value for total of SAMPLING_SHIFT
 * values that will be subsequently stored into the buffer and then wrote down
 * to output wav file.
 * The idea behind only one cosinus calculation per couple of bits is as follows:
 * - Use BMS01 Slide 48 for determining what about how many degrees to shift cos
 * - Realize that cos() is shifter by 90 degrees in comparison to sin()
 * - 00 will have cos() shifted by PI/4
 * - 01 will have cos() shifted by (7/4)*PI
 * - 10 will have cos() shifted by (3/4)*PI
 * - 11 will ahve cos() shifted by (5/4)*PI
 * Each 'turn', global time represented by startTick pointer is increased by 
 * one so the time keeps on flowing.
 *
 * std::istream* dataStream Pointer to data stream to modulate.
 * int &starTick Reference to variable representing time
 * SndfileHandle handle Handle to output wav file.
 */
void QPSKModulation(std::istream* dataStream, int &startTick, SndfileHandle handle)
{
    char c0, c1;
    int  value0, value1;
    int *buffer = new int[30];

    while (dataStream->get(c0) && dataStream->get(c1))
    {
        value0 = c0 - ASCII_NUMBER_CODES_OFFSET; 
        value1 = c1 - ASCII_NUMBER_CODES_OFFSET;   

        // Compute corresponding cosine based on pair values
        if (value0 == 0 && value1 == 0)
        {
            for (int i = 0; i < SAMPLING_SHIFT; i++)
            {
                buffer [i] = AMPLITUDE * cos((FREQ * 2 * startTick * M_PI) + (M_PI/4));
                startTick++;
                std::cout << "Value: " << buffer [i] << "\n";
            }
        }
        else if (value0 == 0 && value1 == 1)
        {
            for (int i = 0; i < SAMPLING_SHIFT; i++)
            {
                buffer [i] = AMPLITUDE * cos((FREQ * 2 * startTick * M_PI) + (7*M_PI/4));
                startTick++;
                std::cout << "Value: " << buffer [i] << "\n";
            }
        }
        else if (value0 == 1 && value1 == 0)
        {
            for (int i = 0; i < SAMPLING_SHIFT; i++)
            {
                buffer [i] = AMPLITUDE * cos((FREQ * 2 * startTick * M_PI) + (3*M_PI/4));    
                startTick++;
                std::cout << "Value: " << buffer [i] << "\n";
            }
        }
        else
        {
            for (int i = 0; i < SAMPLING_SHIFT; i++)
            {
                buffer [i] = AMPLITUDE * cos((FREQ * 2 * startTick * M_PI) + (5*M_PI/4));
                startTick++;
                std::cout << "Value: " << buffer [i] << "\n";
            }
         }
    
        handle.write(buffer, SAMPLING_SHIFT);
    }

    delete [] buffer;
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