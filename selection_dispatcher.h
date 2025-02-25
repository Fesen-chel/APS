#ifndef SELECTIONDISPATCHER_H
#define SELECTIONDISPATCHER_H
#include <vector>
#include <memory>
#include "request.h"
#include "buffer.h"
#include "expert.h"

class SelectionDispatcher {
  private:
  Buffer &buffer;
  std::shared_ptr<Request> request;
  std::vector<Expert> &experts;
  size_t buffer_index;
  size_t experts_index;

  public:
  SelectionDispatcher(std::vector<Expert> &dev, Buffer &buf);

  void select_request();
  void assign_request_to_device(double current_time, std::vector<std::shared_ptr<Request>> &completed_requests);
};
#endif