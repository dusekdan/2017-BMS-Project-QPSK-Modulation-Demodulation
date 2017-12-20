#define EMPTY_STRING ""
#define EXIT_SUCCESS 0
#define EXIT_FAILURE_PARAMS 1
#define EXIT_FAILURE_INPUTFILE 2
#define EXIT_FAILURE_CALCULATION 4
#define ASCII_NUMBER_CODES_OFFSET 48

#define DEBUG_ACTIVE 1
#define SAMPLE_RATE 18000
#define CHANNELS 1
#define FORMAT (SF_FORMAT_WAV | SF_FORMAT_PCM_24)
#define AMPLITUDE (1.0 * 0x7F000000)
#define FREQ (1000.0 / SAMPLE_RATE)

#define SAMPLING_SHIFT 30
#define SYNC_SEQUENCE "00110011"

void QPSKModulation(std::istream* dataStream, int &startTick, SndfileHandle handle);
std::string retrieveInputFileName(int argc, char *argv[]);