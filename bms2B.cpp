/**
 * Author: Daniel Dusek, xdusek21
 * Brno, University of Technology
 * BMS class of 2017/2018, Project #2
 */
#include <iostream>
#include <cstdlib>
#include <fstream>
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

    // Extract usefull information about input file 
    SndfileHandle inputFile = SndfileHandle(inputFileName);
    int sampleRate = inputFile.samplerate();
    double freq = (1000.0 / sampleRate);
    double amplitude = (1.0 * 0x7F000000);
    int numberOfSamples = inputFile.frames();
    int tick = 0;

    // Create buffer and load its contents from file
    int *buffer = new int[numberOfSamples];
    inputFile.read(buffer, numberOfSamples);

    // Using known synchronization sequence to determine number of samples used for modulating one baud
    int samplesPerBaud = retrieveSamplesPerBaud(buffer, tick, freq, amplitude, numberOfSamples);

    // Prepare flow control variables and output aggregator
    std::string outputData = "";
    int baudProgress = 1; int baudCounter = 1;
    int c00 = 0, c01 = 0, c10 = 0, c11 = 0;
    
    // Loop over the rest of the samples (in bauds)
    for (int i = tick; i < numberOfSamples; i++)
    {
        // Calculate value for current time for each possible signal
        int p00 = amplitude * cos((freq * 2 * tick * M_PI) + P00_SHIFT);
        int p01 = amplitude * cos((freq * 2 * tick * M_PI) + P01_SHIFT);
        int p10 = amplitude * cos((freq * 2 * tick * M_PI) + P10_SHIFT);
        int p11 = amplitude * cos((freq * 2 * tick * M_PI) + P11_SHIFT);
        
        // And try to approximate which signal is the most similar to the one 
        // received.
        int currentClosest = selectClosestMatch(buffer[i], p00, p01, p10, p11);
        if (currentClosest == p00)
        {
            c00++;
        }
        else if(currentClosest == p01)
        {
            c01++;
        }
        else if (currentClosest == p10)
        {
            c10++;
        }
        else if (currentClosest == p11) 
        {
            c11++;
        }
        
        // When full baud is processed generate output based on most frequented
        // signal.
        if (baudProgress % samplesPerBaud == 0)
        {
            baudProgress = 1;
            baudCounter++;

            int maxProbability = selectClosestMatch(samplesPerBaud, c00, c01, c10, c11);
            if (maxProbability == c00)
            {
                outputData.append("00");
            }
            else if (maxProbability == c01)
            {
                outputData.append("01");
            }
            else if (maxProbability == c10)
            {
                outputData.append("10");
            }
            else if (maxProbability == c11)
            {
                outputData.append("11");
            }

            c00 = 0; c01 = 0; c10 = 0; c11 = 0; // Reset counters
        }
        else
        {
            baudProgress++;
        }

        tick++;
    }
    
    // Write output file
    std::ofstream of(inputFileName.substr(0, inputFileName.find_last_of(".")).append(".txt"));
    of << outputData;

    delete [] buffer;
    return EXIT_SUCCESS;
}


/**
 * Calculate number of samples used for modulating one baud.
 *
 * We need to figure out number of samples per two modulated bits (baud) using
 * exclusively the knowledge that we will have sequence of 00 at the begining
 * of a signal followed by 11, this whole repeated twice. We generate 00 signal
 * in a same manner as in QPSK modulation and then calculate how many samples
 * match with decent tolerance.
 *
 * Side effect: Skips the synchronization sequence. 
 *
 * int *buffer Array containing received signal values.
 * int &tick Pointer to global tick-based timer.
 * double freq Signal frequency.
 * double amplitude Signal amplitude.
 * int samplesTotal Total number of samples in buffer.
 *
 * return int Number of samples used in signal to modulate one baud.
 *
 */
int retrieveSamplesPerBaud(int *buffer, int &tick, double freq, double amplitude, int samplesTotal)
{
    int matchingLines00 = 0, matchingLines11 = 0, samplesPerBaud = 0;
    double currentShift = (M_PI/4);

    for (int i = 0; i < samplesTotal; i++)
    {
        double computedValue = cos((freq * 2 * tick * M_PI) + currentShift);
        double receivedValue = buffer[i] / amplitude;

        if (currentShift == (M_PI/4))
        {
            // Constant 0.1 was empirically determined to be the most suitable 
            // one to be used as tollerance between computed and received cos()
            if (abs(computedValue - receivedValue) > 0.1)
            {
                // Update current shift to handle 11 modulation
                currentShift = (5*M_PI/4);

                // We hit the first unfitting sample, which should be compared 
                // in 11 sync sequence part. So virtually we need to go back in
                // time by one tick, so the computedValue is recalculated with 
                // the right value (new currentShift).
                i--;
                tick--;
            }
            else
            {
                matchingLines00++;
            }
        }
        else if (currentShift == (5*M_PI/4))
        {
            if (abs(computedValue - receivedValue) > 0.1)
            {
                // Average of number of samples for 00 and 11 sequence
                samplesPerBaud = (matchingLines00+matchingLines11)/2;
                
                // Skip remaining samples from synchronization sequence by 
                // updating global timer accordingly.
                tick += samplesPerBaud*2;
                break;
            }
            else
            {
                matchingLines11++;
            }
        }

        tick++;
    }

    return samplesPerBaud;
}


/**
 * Returns closest operand to the reference value.
 *
 * int reference Reference value to which op1-op4 are compared.
 * int op1 Operand to be compared to reference.
 * int op2 Operand to be compared to reference.
 * int op3 Operand to be compared to reference.
 * int op4 Operand to be compared to reference.
 *
 * return int Operand closest to the reference value. 
 */
int selectClosestMatch(int reference, int op1, int op2, int op3, int op4)
{
    int originals[] = { op1, op2, op3, op4 };
    int diffs[] = {abs(reference - op1), abs(reference - op2), abs(reference - op3), abs(reference - op4)};

    int index = 0;
    for (int i = 1; i < 4; i++)
    {
        if (diffs[i] < diffs[index])
            index = i;
    }

    return originals[index];
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