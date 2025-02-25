#ifndef PLACEMENTDISPATCHER_H
#define PLACEMENTDISPATCHER_H
#include <vector>
#include <memory>
#include "buffer.h"

class PlacementDispatcher {
  private:
  Buffer &buffer;

  public:
  PlacementDispatcher(Buffer &buf);

  void place_request(std::shared_ptr<Request> request, std::vector<std::shared_ptr<Request>> &completed_requests);
};
#endif