#define EMPTY_STRING ""
#define EXIT_SUCCESS 0
#define EXIT_FAILURE_PARAMS 1

#define P00_SHIFT (M_PI/4.0)
#define P01_SHIFT (7.0*M_PI/4.0)
#define P10_SHIFT (3.0*M_PI/4.0)
#define P11_SHIFT (5.0*M_PI/4.0)


std::string retrieveInputFileName(int argc, char *argv[]);
int retrieveSamplesPerBaud(int *buffer, int &tick, double freq, double amplitude, int samplesTotal);
int selectClosestMatch(int reference, int op1, int op2, int op3, int op4);