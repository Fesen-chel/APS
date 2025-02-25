#include "client.h"
#include <vector>
#include <string>
#include "globals.h"

Client::Client(int id) : id(id) {}

int Client::get_id() const { return id; }

std::shared_ptr<Request> Client::generate_request(int request_id, double current_time) {
  int numPhotos = random_int(1, 5);
  std::vector<std::string> photos = generate_random_photos(numPhotos);
  return std::make_shared<Request>(request_id, photos, current_time);
}