
#include "Smart_City.hpp"

int main() {
    SimulationEngine<Bus> sim(10);

    Vehicle<Bus> bus1;
    bus1.setRouteID(101);
    Vehicle<Bus> bus2;
    bus2.setRouteID(102);

    sim.add_vehicle(bus1, "TrafficJam");
    sim.add_vehicle(bus2, "TrafficJam");

    std::vector<int> routes1 = {101};
    std::vector<int> routes2 = {102};
    std::string desc1 = "Jam on route 101";
    std::string desc2 = "Jam on route 102";

    Event<Traffic_Jam> jam1(15, 6, routes1, desc1);
    Event<Traffic_Jam> jam2(10, 4, routes2, desc2);

    sim.schedule_event(ScheduledEvent<Traffic_Jam>(3, jam1));
    sim.schedule_event(ScheduledEvent<Traffic_Jam>(7, jam2));

    sim.run_simulation();



}
