#include "selection_dispatcher.h"
#include <iostream>

SelectionDispatcher::SelectionDispatcher(std::vector<Expert> &dev, Buffer &buf)
  : buffer(buf), experts(dev), buffer_index(0), experts_index(0) {}

std::shared_ptr<Request> SelectionDispatcher::select_request() {
  size_t buffer_capacity = buffer.get_capacity();
  while (true) {
    if (buffer_index < buffer_capacity && buffer.get_requests()[buffer_index] != nullptr) {
      std::shared_ptr<Request> request = buffer.get_requests()[buffer_index];
      buffer.get_requests()[buffer_index] = nullptr;
      std::cout << "Request " << request->get_id() << " selected from buffer at position " << buffer_index << std::endl;
      buffer_index = (buffer_index + 1) % buffer_capacity;
      return request;
    }
    buffer_index = (buffer_index + 1) % buffer_capacity;
    if (buffer_index == 0) break;
  }
  return nullptr;
}

void SelectionDispatcher::assign_request_to_device(std::shared_ptr<Request> request, double current_time, std::vector<std::shared_ptr<Request>> &completed_requests) {
  if (request == nullptr) return;

  size_t experts_size = experts.size();
  while (true) {
    if (experts_index < experts_size && !experts[experts_index].get_is_busy()) {
      experts[experts_index].assign_request(request, current_time);
      completed_requests.push_back(request);
      experts_index = (experts_index + 1) % experts_size;
      break;
    }
    experts_index = (experts_index + 1) % experts_size;
    if (experts_index == 0) break;
  }
}