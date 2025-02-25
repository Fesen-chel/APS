#include "globals.h"
#include <vector>
#include <string>
#include <random>

const int NUM_CLIENTS = 10;
const int NUM_DEVICES = 3;
const int BUFFER_SIZE = 10;
const int TOTAL_REQUESTS = 100;
const double LYAMBDA = 1;
const double MIN_INTERARRIVAL_TIME = 0.1;
const double MAX_INTERARRIVAL_TIME = 0.5;

std::random_device rd;
std::mt19937 gen(rd());

double random_exponential(double lambda) {
  std::exponential_distribution<double> dist(lambda);
  return dist(gen);
}

double random_uniform(double min, double max) {
  std::uniform_real_distribution<double> dist(min, max);
  return dist(gen);
}

int random_int(int min, int max) {
  std::uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

std::string random_photo_name() {
  std::vector<std::string> photoNames = {
      "photo1.jpg", "photo2.jpg", "photo3.jpg",
      "damage1.jpg", "damage2.jpg", "damage3.jpg",
      "inspection1.jpg", "inspection2.jpg", "inspection3.jpg"
  };
  return photoNames[random_int(0, photoNames.size() - 1)];
}

std::vector<std::string> generate_random_photos(int count) {
  std::vector<std::string> photos;
  for (int i = 0; i < count; ++i) {
    photos.push_back(random_photo_name());
  }
  return photos;
}