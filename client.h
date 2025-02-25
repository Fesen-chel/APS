#ifndef CLIENT_H
#define CLIENT_H
#include <memory>
#include "request.h"

class Client {
  private:
  int id;

  public:
  Client(int id);

  int get_id() const;
  std::shared_ptr<Request> generate_request(int request_id, double current_time);
};
#endif