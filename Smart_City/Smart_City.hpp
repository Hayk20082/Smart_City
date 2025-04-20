#ifndef SMART_CITY
#define SMART_CITY
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

struct Traffic_Jam {};
struct Road_Closure {};
struct Weather_Impact {};

template<typename E>
class Event {
public:
    Event(int d, int s, std::vector<int> af, std::string ds)
        : duration(d), severity(s), affected_routes(std::move(af)), description(std::move(ds)) {}

    int get_severity() const { return severity; }
    const std::vector<int>& get_affected_routes() const { return affected_routes; }
    const std::string& get_description() const { return description; }

private:
    int duration;
    int severity;
    std::vector<int> affected_routes;
    std::string description;
};

template<typename E>
class ScheduledEvent {
public:
    ScheduledEvent(int t, const Event<E>& e) : time(t), event(e) {}

    int get_time() const { return time; }
    const Event<E>& get_event() const { return event; }

private:
    int time;
    Event<E> event;
};

struct Bus {};
struct Taxi {};

template <class T>
class Vehicle {
private:
    int speed;
    int capacity;
    int route_ID;
    std::string current_location;
    int estimated_arrival_time;

public:
    Vehicle() : speed(50), capacity(0), route_ID(0), current_location("0"), estimated_arrival_time(0) {}

    void update_location(std::string loc) { current_location = loc; }
    void adjust_speed(int delta) { speed += delta; if (speed < 0) speed = 0; }
    void recalculating_routes() { std::cout << "Recalculating route for vehicle on route " << route_ID << std::endl; }
    void Logs() { std::cout << "Vehicle on route " << route_ID << " at location " << current_location << std::endl; }

    int getRouteID() const { return route_ID; }
    int getSpeed() const { return speed; }
    std::string getCurrentLocation() const { return current_location; }
    int getSeverityImpact(int severity) const { return speed - severity * 2; }
    void setRouteID(int id) { route_ID = id; }
};

template <>
class Vehicle<Bus> {
private:
    int speed;
    int capacity;
    int route_ID;
    std::string current_location;
    int estimated_arrival_time;
    int fuel_efficiency;

public:
    Vehicle() : speed(50), capacity(75), route_ID(0), current_location("0"), estimated_arrival_time(0), fuel_efficiency(120) {}

    void update_location(std::string loc) { current_location = loc; }
    void adjust_speed(int delta) { speed += delta; if (speed < 0) speed = 0; }
    void recalculating_routes() { std::cout << "Bus on route " << route_ID << " is recalculating route.\n"; }
    void Logs() { std::cout << "Bus on route " << route_ID << " at " << current_location << std::endl; }

    int getRouteID() const { return route_ID; }
    int getSpeed() const { return speed; }
    std::string getCurrentLocation() const { return current_location; }
    int getSeverityImpact(int severity) const { return speed - severity * 2; }
    void setRouteID(int id) { route_ID = id; }
};

template <>
class Vehicle<Taxi> {
private:
    int speed;
    int capacity;
    int route_ID;
    std::string current_location;
    int estimated_arrival_time;
    int battery_level;

public:
    Vehicle() : speed(60), capacity(4), route_ID(0), current_location("0"), estimated_arrival_time(0), battery_level(100) {}

    void update_location(std::string loc) { current_location = loc; }
    void adjust_speed(int delta) { speed += delta; if (speed < 0) speed = 0; }
    void recalculating_routes() { std::cout << "Taxi on route " << route_ID << " is recalculating route.\n"; }
    void Logs() { std::cout << "Taxi on route " << route_ID << " at " << current_location << std::endl; }

    int getRouteID() const { return route_ID; }
    int getSpeed() const { return speed; }
    std::string getCurrentLocation() const { return current_location; }
    int getSeverityImpact(int severity) const { return speed - severity * 2; }
    void setRouteID(int id) { route_ID = id; }
};

template<typename V, typename E>
class EventManager {
public:
    void subscribe(const std::string& type, V* v) {
        subscriptions[type].push_back(v);
    }

    void process_event(std::vector<V>& vehicles, const ScheduledEvent<E>& event) {
        const auto& affected = event.get_event().get_affected_routes();
        int severity = event.get_event().get_severity();
        for (auto& vehicle : vehicles) {
            if (std::find(affected.begin(), affected.end(), vehicle.getRouteID()) != affected.end()) {
                vehicle.adjust_speed(-severity * 2);
                vehicle.recalculating_routes();
                vehicle.Logs();
            }
        }
    }

private:
    std::map<std::string, std::vector<V*>> subscriptions;
};

template<typename T>
class SimulationEngine {
public:
    SimulationEngine(int duration = 10) : sim_duration(duration) {}

    void add_vehicle(const Vehicle<T>& vehicle, const std::string& event_type) {
        vehicles.push_back(vehicle);
        event_manager.subscribe(event_type, &vehicles.back());
    }

    void schedule_event(const ScheduledEvent<Traffic_Jam>& event) {
        scheduled_jams.push_back(event);
    }

    void run_simulation() {
        for (const auto& event : scheduled_jams) {
            std::cout << "Processing event at time: " << event.get_time() << std::endl;
            event_manager.process_event(vehicles, event);
        }
    }

private:
    int sim_duration;
    std::vector<Vehicle<T>> vehicles;
    std::vector<ScheduledEvent<Traffic_Jam>> scheduled_jams;
    EventManager<Vehicle<T>, Traffic_Jam> event_manager;
};

#endif
