#ifndef SELECTIONDISPATCHER_H
#define SELECTIONDISPATCHER_H
#include <vector>
#include <memory>
#include "buffer.h"
#include "expert.h"

class SelectionDispatcher {
  private:
  Buffer &buffer;
  std::vector<Expert> &experts;
  size_t buffer_index;
  size_t experts_index;

  public:
  SelectionDispatcher(std::vector<Expert> &dev, Buffer &buf);

  std::shared_ptr<Request> select_request();
  void assign_request_to_device(std::shared_ptr<Request> request, double current_time, std::vector<std::shared_ptr<Request>> &completed_requests);
};
#endif