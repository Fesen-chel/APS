#ifndef SIMULATION_H
#define SIMULATION_H
#include <vector>
#include <memory>
#include "buffer.h"
#include "expert.h"
#include "client.h"
#include "placement_dispatcher.h"
#include "selection_dispatcher.h"

void calculate_statistics(const std::vector<std::shared_ptr<Request>> &completed_requests, const std::vector<Expert> &experts, double total_time);
bool is_simulation_complete(const Buffer &buffer, const std::vector<Expert> &experts);
void print_status(const Buffer &buffer, const std::vector<Expert> &experts);
void run_simulation(bool step_by_step = false);
#endif