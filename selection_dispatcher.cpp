#include "selection_dispatcher.h"
#include <iostream>

SelectionDispatcher::SelectionDispatcher(std::vector<Expert> &dev, Buffer &buf)
  : buffer(buf), request(nullptr), experts(dev), buffer_index(0), experts_index(0) {}

void SelectionDispatcher::select_request() {
  if (request != nullptr) return;
  size_t buffer_capacity = buffer.get_capacity();
  for (size_t i = 0; i < buffer_capacity; i++) {
    if (buffer.get_requests()[buffer_index] != nullptr) {
      request = buffer.get_requests()[buffer_index];
      buffer.get_requests()[buffer_index] = nullptr;
      std::cout << "Request " << request->get_id() << " selected from buffer at position " << buffer_index << std::endl;
      buffer_index = (buffer_index + 1) % buffer_capacity;
      break;
    }
    buffer_index = (buffer_index + 1) % buffer_capacity;
  }
}

void SelectionDispatcher::assign_request_to_device(double current_time, std::vector<std::shared_ptr<Request>> &completed_requests) {
  if (request == nullptr) return;

  size_t experts_size = experts.size();
  for (size_t i = 0; i < experts_size; i++) {
    if (!experts[experts_index].get_is_busy()) {
      experts[experts_index].assign_request(request, current_time);
      completed_requests.push_back(request);
      experts_index = (experts_index + 1) % experts_size;
      request = nullptr;
      break;
    }
    experts_index = (experts_index + 1) % experts_size;
  }
}