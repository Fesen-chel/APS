#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <random>
#include <cmath>
#include <iomanip>
#include <map>
#include <thread>
using namespace std;

// Глобальные константы
const int NUM_CLIENTS = 150;
const int NUM_DEVICES = 3;
const int BUFFER_SIZE = 4;
const int TOTAL_REQUESTS = 1000;
const double LYAMBDA = 1.0; 
const double MIN_INTERARRIVAL_TIME = 0.1; 
const double MAX_INTERARRIVAL_TIME = 0.5; 

random_device rd;
mt19937 gen(rd());

double random_exponential(double lambda) {
  exponential_distribution<double> dist(lambda);
  return dist(gen);
}

double random_uniform(double min, double max) {
  uniform_real_distribution<double> dist(min, max);
  return dist(gen);
}

int random_int(int min, int max) {
  uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

string random_photo_name() {
  vector<string> photoNames = {
      "photo1.jpg", "photo2.jpg", "photo3.jpg",
      "damage1.jpg", "damage2.jpg", "damage3.jpg",
      "inspection1.jpg", "inspection2.jpg", "inspection3.jpg"
  };
  return photoNames[random_int(0, photoNames.size() - 1)];
}

vector<string> generate_random_photos(int count) {
  vector<string> photos;
  for (int i = 0; i < count; ++i) {
    photos.push_back(random_photo_name());
  }
  return photos;
}

class Request {
  private:
  int id;
  vector<string> photos;
  string status;
  double arrival_time;
  double completion_time;
  double wait_time;

  public:
  Request(int id, const vector<string> &photos, double arv_time)
    : id(id), photos(photos), status("pending"), arrival_time(arv_time), completion_time(-1), wait_time(0) {}

  void update_status(const string &new_status) {
    status = new_status;
    cout << "Request " << id << ": Status updated to " << status << endl;
  }

  int get_id() const {
    return id;
  }

  const string &get_status() const {
    return status;
  }

  double get_wait_time() const {
    return wait_time;
  }

  double get_arrival_time() const {
    return arrival_time;
  }

  double get_completion_time() const {
    return completion_time;
  }

  void set_wait_time(double time) {
    wait_time = time;
  }

  void set_arrival_time(double time) {
    arrival_time = time;
  }

  void set_completion_time(double time) {
    completion_time = time;
  }
};

class Expert {
  private:
  int id;
  bool is_busy;
  shared_ptr<Request> current_request;
  double completion_time;
  double busy_time;

  public:
  Expert(int id) : id(id), is_busy(false), current_request(nullptr), completion_time(0), busy_time(0) {}

  void complete_request(double current_time) {
    if (current_request != nullptr && current_time >= completion_time) {
      current_request->update_status("processed");
      current_request->set_completion_time(current_time);
      current_request = nullptr;
      is_busy = false;
      cout << "Expert " << id << " completed request " << id << endl;
    }
  }

  void assign_request(shared_ptr<Request> request, double current_time) {
    if (!is_busy) {
      current_request = request;
      is_busy = true;
      completion_time = current_time + random_exponential(LYAMBDA);
      busy_time += completion_time - current_time;
      request->set_wait_time(current_time - request->get_arrival_time());
      request->update_status("processing");
      cout << "Expert " << id << " assigned request " << request->get_id()
        << " with completion time " << completion_time << endl;
    }
  }

  int get_id() const {
    return id;
  }

  bool get_is_busy() const {
    return is_busy;
  }

  double get_busy_time() const {
    return busy_time;
  }

  double get_completion_time() const {
    return completion_time;
  }
};

class Buffer {
  private:
  vector<shared_ptr<Request>> requests;
  size_t capacity;
  size_t pointer;
  size_t last_added;

  public:
  Buffer(int cap) : capacity(cap), pointer(0), last_added(0) {
    requests.resize(capacity, nullptr);
  }

  bool add_request(shared_ptr<Request> request) {
    for (size_t i = 0; i < capacity; ++i) {
      if (requests[i] == nullptr) {
        requests[i] = request;
        last_added = i;
        cout << "Request " << request->get_id() << " added to buffer at position " << i << endl;
        return true;
      }
    }
    return false;
  }

  shared_ptr<Request> remove_request(size_t index) {
    if (index < capacity && requests[index] != nullptr) {
      shared_ptr<Request> req = requests[index];
      requests[index] = nullptr;
      cout << "Request " << req->get_id() << " removed from buffer at position " << index << endl;
      return req;
    }
    return nullptr;
  }

  bool is_full() const {
    for (const auto &req : requests) {
      if (req == nullptr) {
        return false;
      }
    }
    return true;
  }

  vector<shared_ptr<Request>> &get_requests() {
    return requests;
  }

  const vector<shared_ptr<Request>> &get_requests() const {
    return requests;
  }

  size_t get_capacity() const {
    return capacity;
  }

  size_t get_pointer() const {
    return pointer;
  }

  size_t get_last_added() const {
    return last_added;
  }
};

class Client {
  private:
  int id;

  public:
  Client(int id) : id(id) {}

  int get_id() const {
    return id;
  }

  shared_ptr<Request> generate_request(int request_id, double current_time) {
    int numPhotos = random_int(1, 5);
    vector<string> photos = generate_random_photos(numPhotos);
    return make_shared<Request>(request_id, photos, current_time);
  }
};

class PlacementDispatcher {
  private:
  Buffer &buffer;

  public:
  PlacementDispatcher(Buffer &buf) : buffer(buf) {}

  void place_request(shared_ptr<Request> request, vector<shared_ptr<Request>> &completed_requests) {
    if (!buffer.add_request(request)) {
      size_t last_index = buffer.get_last_added();
      shared_ptr<Request> rejected_request = buffer.remove_request(last_index);
      if (rejected_request != nullptr) {
        rejected_request->update_status("rejected");
        completed_requests.push_back(rejected_request);
        cout << "Request " << rejected_request->get_id() << " rejected due to buffer overflow" << endl;
      }
      buffer.add_request(request);
    }
  }
};

class SelectionDispatcher {
  private:
  Buffer &buffer;
  vector<Expert> &experts;
  size_t buffer_index;
  size_t experts_index;

  public:
  SelectionDispatcher(vector<Expert> &dev, Buffer &buf)
    : buffer(buf), experts(dev), buffer_index(0), experts_index(0) {}

  shared_ptr<Request> select_request() {
    size_t buffer_capacity = buffer.get_capacity();
    while (true) {
      if (buffer_index < buffer_capacity && buffer.get_requests()[buffer_index] != nullptr) {
        shared_ptr<Request> request = buffer.get_requests()[buffer_index];
        buffer.get_requests()[buffer_index] = nullptr; 
        cout << "Request " << request->get_id() << " selected from buffer at position " << buffer_index << endl;
        buffer_index = (buffer_index + 1) % buffer_capacity; 
        return request;
      }
      buffer_index = (buffer_index + 1) % buffer_capacity; 
      if (buffer_index == 0) break; 
    }
    return nullptr;
  }

  void assign_request_to_device(shared_ptr<Request> request, double current_time, vector<shared_ptr<Request>> &completed_requests) {
    if (request == nullptr) return;

    size_t experts_size = experts.size();
    while (true) {
      if (experts_index < experts_size && !experts[experts_index].get_is_busy()) {
        experts[experts_index].assign_request(request, current_time);
        completed_requests.push_back(request); 
        experts_index = (experts_index + 1) % experts_size; 
        break;
      }
      experts_index = (experts_index + 1) % experts_size; 
      if (experts_index == 0) break; 
    }
  }
};

void calculate_statistics(const vector<shared_ptr<Request>> &completed_requests, const vector<Expert> &experts, double total_time) {
  map<int, map<string, double>> source_stats;

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

  cout << "\n=== Source Statistics ===\n";
  cout << setw(15) << "Client ID" << setw(15) << "Total" << setw(15) << "Rejected" << setw(15) << "P(reject)"
    << setw(20) << "Avg T(stay)" << setw(20) << "Avg T(service)" << setw(20) << "Avg T(buffer)"
    << setw(20) << "D(T(service))" << setw(20) << "D(T(buffer))" << "\n";

  double p_sum = 0;
  double utilization_sum = 0;
  double avg_total_sum = 0;
  double total_buf_sum = 0;
  double total_proc_sum = 0;
  int completed_req = 0;
  int rejected_req = 0;

  for (auto it = source_stats.begin(); it != source_stats.end(); ++it) {
    int client_id = it->first;
    const map<string, double> &stats = it->second;

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

    cout << fixed << setprecision(2);
    cout << setw(15) << client_id
      << setw(15) << total
      << setw(15) << rejected
      << setw(15) << p_reject
      << setw(20) << avg_total_time
      << setw(20) << avg_processing_time
      << setw(20) << avg_buffer_time
      << setw(20) << processing_time_dispersion
      << setw(20) << buffer_time_dispersion << "\n";
  }

  cout << "\n=== Expert Statistics ===\n";
  cout << setw(15) << "Expert ID" << setw(15) << "Utilization" << setw(15) << "Busy Time\n";

  for (const auto &expert : experts) {
    double utilization = total_time > 0 ? (expert.get_busy_time() / total_time) : 0;
    utilization_sum += utilization;
    cout << setw(15) << expert.get_id()
      << setw(15) << utilization
      << setw(15) << expert.get_busy_time() << "\n";
  }

  double p_mean = p_sum / NUM_CLIENTS;
  double utilization_mean = utilization_sum / NUM_DEVICES;
  double avg_total_mean = avg_total_sum / NUM_CLIENTS;
  double total_sys_sum = total_buf_sum + total_proc_sum;

  cout << "\nMean P(reject): " << p_mean
    << "\nMean Utilization: " << utilization_mean
    << "\nMean Total Time: " << avg_total_mean
    << "\nCompleted Requests: " << completed_req
    << "\nRejected Requests: " << rejected_req
    << "\nTotal System Time: " << total_sys_sum << "\n";
}

bool is_simulation_complete(const Buffer &buffer, const vector<Expert> &experts) {
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

void print_status(const Buffer &buffer, const vector<Expert> &experts) {
  cout << "Buffer: [";
  for (size_t i = 0; i < buffer.get_capacity(); ++i) {
    if (buffer.get_requests()[i] != nullptr) {
      cout << buffer.get_requests()[i]->get_id() << " ";
    } else {
      cout << "- ";
    }
  }
  cout << "]\n";

  for (const auto &expert : experts) {
    cout << "Expert " << expert.get_id() << ": " << (expert.get_is_busy() ? "busy" : "free");
    if (expert.get_is_busy()) {
      cout << " (completes at " << expert.get_completion_time() << ")";
    }
    cout << "\n";
  }
}

void run_simulation(bool step_by_step = false) {
  vector<Expert> experts;
  for (int i = 0; i < NUM_DEVICES; ++i) {
    experts.emplace_back(i + 1);
  }

  Buffer buffer(BUFFER_SIZE);
  PlacementDispatcher placement_dispatcher(buffer);
  SelectionDispatcher selection_dispatcher(experts, buffer);

  vector<Client> clients;
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    clients.emplace_back(i + 1);
  }

  vector<shared_ptr<Request>> completed_requests;
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

    for (auto &expert : experts) {
      expert.complete_request(current_time);
    }

    shared_ptr<Request> buffer_request = selection_dispatcher.select_request();
    if (buffer_request != nullptr) {
      selection_dispatcher.assign_request_to_device(buffer_request, current_time, completed_requests);
    }

    if (step_by_step) {
      cout << "\n=== Step ===\n";
      print_status(buffer, experts);
      cout << "Current time: " << current_time << "\n";
      //this_thread::sleep_for(chrono::milliseconds(100));
    }

    current_time += random_uniform(MIN_INTERARRIVAL_TIME, MAX_INTERARRIVAL_TIME);
  }

  calculate_statistics(completed_requests, experts, current_time);
}

int main() {
  run_simulation(true); 
  return 0;
}