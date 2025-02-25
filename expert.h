#ifndef EXPERT_H
#define EXPERT_H
#include <memory>
#include "request.h"

class Expert {
  private:
  int id;
  bool is_busy;
  std::shared_ptr<Request> current_request;
  double completion_time;
  double busy_time;

  public:
  Expert(int id);

  void complete_request(double current_time);
  void assign_request(std::shared_ptr<Request> request, double current_time);
  int get_id() const;
  bool get_is_busy() const;
  double get_busy_time() const;
  double get_completion_time() const;
};
#endif