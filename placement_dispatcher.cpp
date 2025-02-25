#include "placement_dispatcher.h"
#include <iostream>

PlacementDispatcher::PlacementDispatcher(Buffer &buf) : buffer(buf) {}

void PlacementDispatcher::place_request(std::shared_ptr<Request> request, std::vector<std::shared_ptr<Request>> &completed_requests) {
  if (request == nullptr) {
    return;
  }
  if (!buffer.add_request(request)) {
    size_t last_index = buffer.get_last_added();
    std::shared_ptr<Request> rejected_request = buffer.remove_request(last_index);
    if (rejected_request != nullptr) {
      rejected_request->update_status("rejected");
      completed_requests.push_back(rejected_request);
      std::cout << "Request " << rejected_request->get_id() << " rejected due to buffer overflow" << std::endl;
    }
    buffer.add_request(request);
  }
}