#ifndef REQUEST_H
#define REQUEST_H
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class Request {
  private:
  int id;
  std::vector<std::string> photos;
  std::string status;
  double arrival_time;
  double completion_time;
  double wait_time;

  public:
  Request(int id, const std::vector<std::string> &photos, double arv_time);

  void update_status(const std::string &new_status);
  int get_id() const;
  const std::string &get_status() const;
  double get_wait_time() const;
  double get_arrival_time() const;
  double get_completion_time() const;
  void set_wait_time(double time);
  void set_arrival_time(double time);
  void set_completion_time(double time);
};
#endif