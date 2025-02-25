#include "expert.h"
#include <iostream>
#include "globals.h"

Expert::Expert(int id) : id(id), is_busy(false), current_request(nullptr), completion_time(0), busy_time(0) {}

void Expert::complete_request(double current_time) {
  if (current_request != nullptr && current_time >= completion_time) {
    current_request->update_status("processed");
    current_request->set_completion_time(current_time);
    current_request = nullptr;
    is_busy = false;
    std::cout << "Expert " << id << " completed request " << id << std::endl;
  }
}

void Expert::assign_request(std::shared_ptr<Request> request, double current_time) {
  if (!is_busy) {
    current_request = request;
    is_busy = true;
    completion_time = current_time + random_exponential(LYAMBDA);
    busy_time += completion_time - current_time;
    request->set_wait_time(current_time - request->get_arrival_time());
    request->update_status("processing");
    std::cout << "Expert " << id << " assigned request " << request->get_id()
      << " with completion time " << completion_time << std::endl;
  }
}

int Expert::get_id() const { return id; }

bool Expert::get_is_busy() const { return is_busy; }

double Expert::get_busy_time() const { return busy_time; }

double Expert::get_completion_time() const { return completion_time; }