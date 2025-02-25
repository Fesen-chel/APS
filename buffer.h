#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <memory>
#include "request.h"

class Buffer {
  private:
  std::vector<std::shared_ptr<Request>> requests;
  size_t capacity;
  size_t pointer;
  size_t last_added;

  public:
  Buffer(int cap);

  bool add_request(std::shared_ptr<Request> request);
  std::shared_ptr<Request> remove_request(size_t index);
  bool is_full() const;
  std::vector<std::shared_ptr<Request>> &get_requests();
  const std::vector<std::shared_ptr<Request>> &get_requests() const;
  size_t get_capacity() const;
  size_t get_pointer() const;
  size_t get_last_added() const;
};
#endif