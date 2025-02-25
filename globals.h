#ifndef GLOBALS_H
#define GLOBALS_H
#include <random>

extern const int NUM_CLIENTS;
extern const int NUM_DEVICES;
extern const int BUFFER_SIZE;
extern const int TOTAL_REQUESTS;
extern const double LYAMBDA;
extern const double MIN_INTERARRIVAL_TIME;
extern const double MAX_INTERARRIVAL_TIME;

double random_exponential(double lambda);
double random_uniform(double min, double max);
int random_int(int min, int max);
std::string random_photo_name();
std::vector<std::string> generate_random_photos(int count);
#endif