#include "request.h"

Request::Request(int id, const std::vector<std::string> &photos, double arv_time)
  : id(id), photos(photos), status("pending"), arrival_time(arv_time), completion_time(-1), wait_time(0) {}

void Request::update_status(const std::string &new_status) {
  status = new_status;
  std::cout << "Request " << id << ": Status updated to " << status << std::endl;
}

int Request::get_id() const { return id; }

const std::string &Request::get_status() const { return status; }

double Request::get_wait_time() const { return wait_time; }

double Request::get_arrival_time() const { return arrival_time; }

double Request::get_completion_time() const { return completion_time; }

void Request::set_wait_time(double time) { wait_time = time; }

void Request::set_arrival_time(double time) { arrival_time = time; }

void Request::set_completion_time(double time) { completion_time = time; }