#include "simulation.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <thread>
#include "globals.h"

void calculate_statistics(const std::vector<std::shared_ptr<Request>> &completed_requests, const std::vector<Expert> &experts, double total_time) {
  std::map<int, std::map<std::string, double>> source_stats;

  for (const auto &request : completed_requests) {
    if (request == nullptr) continue;
    int client_id = request->get_id();
    if (source_stats.find(client_id) == source_stats.end()) {
      source_stats[client_id] = {
          {"total", 0},
          {"rejected", 0},
          {"buffer_time", 0},
          {"processing_time", 0},
          {"buffer_time_sq", 0},
          {"processing_time_sq", 0}
      };
    }

    source_stats[client_id]["total"]++;
    if (request->get_status() == "rejected") {
      source_stats[client_id]["rejected"]++;
    } else {
      double buffer_time = request->get_wait_time();
      double processing_time = request->get_completion_time() - request->get_arrival_time();
      source_stats[client_id]["buffer_time"] += buffer_time;
      source_stats[client_id]["processing_time"] += processing_time;
      source_stats[client_id]["buffer_time_sq"] += buffer_time * buffer_time;
      source_stats[client_id]["processing_time_sq"] += processing_time * processing_time;
    }
  }

  std::cout << "\n=== Source Statistics ===\n";
  std::cout << std::setw(15) << "Client ID" << std::setw(15) << "Total" << std::setw(15) << "Rejected" << std::setw(15) << "P(reject)"
    << std::setw(20) << "Avg T(stay)" << std::setw(20) << "Avg T(service)" << std::setw(20) << "Avg T(buffer)"
    << std::setw(20) << "D(T(service))" << std::setw(20) << "D(T(buffer))" << "\n";

  double p_sum = 0;
  double utilization_sum = 0;
  double avg_total_sum = 0;
  double total_buf_sum = 0;
  double total_proc_sum = 0;
  int completed_req = 0;
  int rejected_req = 0;

  for (auto it = source_stats.begin(); it != source_stats.end(); ++it) {
    int client_id = it->first;
    const std::map<std::string, double> &stats = it->second;

    int total = stats.at("total");
    int rejected = stats.at("rejected");
    double p_reject = (total > 0) ? ((double)rejected / total) : 0;
    double avg_buffer_time = (total - rejected > 0) ? (stats.at("buffer_time") / (total - rejected)) : 0;
    double avg_processing_time = (total - rejected > 0) ? (stats.at("processing_time") / (total - rejected)) : 0;
    double avg_total_time = avg_buffer_time + avg_processing_time;
    double buffer_time_dispersion =
      (total - rejected > 0) ?
      ((double)stats.at("buffer_time_sq") / (total - rejected)) - (avg_buffer_time * avg_buffer_time) : 0;
    double processing_time_dispersion =
      (total - rejected > 0) ?
      ((double)stats.at("processing_time_sq") / (total - rejected)) - (avg_processing_time * avg_processing_time) : 0;

    p_sum += p_reject;
    avg_total_sum += avg_total_time;
    total_buf_sum += stats.at("buffer_time");
    total_proc_sum += stats.at("processing_time");
    completed_req += total - rejected;
    rejected_req += rejected;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(15) << client_id
      << std::setw(15) << total
      << std::setw(15) << rejected
      << std::setw(15) << p_reject
      << std::setw(20) << avg_total_time
      << std::setw(20) << avg_processing_time
      << std::setw(20) << avg_buffer_time
      << std::setw(20) << processing_time_dispersion
      << std::setw(20) << buffer_time_dispersion << "\n";
  }

  std::cout << "\n=== Expert Statistics ===\n";
  std::cout << std::setw(15) << "Expert ID" << std::setw(15) << "Utilization" << std::setw(15) << "Busy Time\n";

  for (const auto &expert : experts) {
    double utilization = total_time > 0 ? (expert.get_busy_time() / total_time) : 0;
    utilization_sum += utilization;
    std::cout << std::setw(15) << expert.get_id()
      << std::setw(15) << utilization
      << std::setw(15) << expert.get_busy_time() << "\n";
  }

  double p_mean = p_sum / NUM_CLIENTS;
  double utilization_mean = utilization_sum / NUM_DEVICES;
  double avg_total_mean = avg_total_sum / NUM_CLIENTS;
  double total_sys_sum = total_buf_sum + total_proc_sum;

  std::cout << "\nMean P(reject): " << p_mean
    << "\nMean Utilization: " << utilization_mean
    << "\nMean Total Time: " << avg_total_mean
    << "\nCompleted Requests: " << completed_req
    << "\nRejected Requests: " << rejected_req
    << "\nTotal System Time: " << total_sys_sum << "\n";
}

bool is_simulation_complete(const Buffer &buffer, const std::vector<Expert> &experts) {
  for (size_t i = 0; i < buffer.get_capacity(); ++i) {
    if (buffer.get_requests()[i] != nullptr) {
      return false;
    }
  }
  for (const auto &expert : experts) {
    if (expert.get_is_busy()) {
      return false;
    }
  }
  return true;
}

void print_status(const Buffer &buffer, const std::vector<Expert> &experts) {
  std::cout << "Buffer: [ ";
  for (size_t i = 0; i < buffer.get_capacity(); ++i) {
    if (buffer.get_requests()[i] != nullptr) {
      std::cout << buffer.get_requests()[i]->get_id() << " ";
    } else {
      std::cout << "- ";
    }
  }
  std::cout << "]\n";

  for (const auto &expert : experts) {
    std::cout << "Expert " << expert.get_id() << ": " << (expert.get_is_busy() ? "busy" : "free");
    if (expert.get_is_busy()) {
      std::cout << " (completes at " << expert.get_completion_time() << ")";
    }
    std::cout << "\n";
  }
}

void run_simulation(bool step_by_step) {
  std::vector<Expert> experts;
  for (int i = 0; i < NUM_DEVICES; ++i) {
    experts.emplace_back(i + 1);
  }

  Buffer buffer(BUFFER_SIZE);
  PlacementDispatcher placement_dispatcher(buffer);
  SelectionDispatcher selection_dispatcher(experts, buffer);

  std::vector<Client> clients;
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    clients.emplace_back(i + 1);
  }

  std::vector<std::shared_ptr<Request>> completed_requests;
  int requestId = 1;
  double current_time = 0.0;
  int processed_requests = 0;

  while (processed_requests < TOTAL_REQUESTS || !is_simulation_complete(buffer, experts)) {
    if (processed_requests < TOTAL_REQUESTS) {
      Client &client = clients[random_int(0, NUM_CLIENTS - 1)];
      auto request = client.generate_request(requestId, current_time);
      placement_dispatcher.place_request(request, completed_requests);
      processed_requests++;
      requestId++;
    }

    selection_dispatcher.select_request();
    selection_dispatcher.assign_request_to_device(current_time, completed_requests);

    for (auto &expert : experts) {
      expert.complete_request(current_time);
    }

    if (step_by_step) {
      std::cout << "\n=== Step ===\n";
      print_status(buffer, experts);
      std::cout << "Current time: " << current_time << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    current_time += 0.1;
  }

  calculate_statistics(completed_requests, experts, current_time);
}