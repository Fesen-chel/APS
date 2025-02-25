#include "buffer.h"
#include <iostream>

Buffer::Buffer(int cap) : capacity(cap), pointer(0), last_added(0) {
  requests.resize(capacity, nullptr);
}

bool Buffer::add_request(std::shared_ptr<Request> request) {
  for (size_t i = 0; i < capacity; ++i) {
    if (requests[i] == nullptr) {
      requests[i] = request;
      last_added = i;
      std::cout << "Request " << request->get_id() << " added to buffer at position " << i << std::endl;
      return true;
    }
  }
  return false;
}

std::shared_ptr<Request> Buffer::remove_request(size_t index) {
  if (index < capacity && requests[index] != nullptr) {
    std::shared_ptr<Request> req = requests[index];
    requests[index] = nullptr;
    std::cout << "Request " << req->get_id() << " removed from buffer at position " << index << std::endl;
    return req;
  }
  return nullptr;
}

bool Buffer::is_full() const {
  for (const auto &req : requests) {
    if (req == nullptr) {
      return false;
    }
  }
  return true;
}

std::vector<std::shared_ptr<Request>> &Buffer::get_requests() { return requests; }

const std::vector<std::shared_ptr<Request>> &Buffer::get_requests() const { return requests; }

size_t Buffer::get_capacity() const { return capacity; }

size_t Buffer::get_pointer() const { return pointer; }

size_t Buffer::get_last_added() const { return last_added; }