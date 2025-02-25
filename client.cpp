#include "client.h"
#include <vector>
#include <string>
#include "globals.h"

Client::Client(int id) : id(id), next_request_time(0) {}

int Client::get_id() const { return id; }

std::shared_ptr<Request> Client::generate_request(int request_id, double current_time) {
  if (current_time < next_request_time) {
    return nullptr;
  }
  int numPhotos = random_int(1, 5);
  std::vector<std::string> photos = generate_random_photos(numPhotos);
  next_request_time = current_time + random_uniform(MIN_INTERARRIVAL_TIME, MAX_INTERARRIVAL_TIME);
  return std::make_shared<Request>(request_id, photos, current_time);
}