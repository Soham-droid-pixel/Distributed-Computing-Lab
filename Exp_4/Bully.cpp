#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    bool active;

    Process(int pid) {
        id = pid;
        active = true;
    }
};

vector<Process> processes;
int coordinator_id = -1;

void announce_coordinator(int id) {
    cout << "\n*** Process " << id << " becomes the NEW COORDINATOR! ***" << endl;
    coordinator_id = id;
    cout << "Coordinator " << id << " informs all active processes.\n" << endl;
}

void start_election(int initiator_id) {
    cout << "\nProcess " << initiator_id << " initiates ELECTION." << endl;
    
    if (!processes[initiator_id - 1].active) {
        cout << "Process " << initiator_id << " is dead and cannot start election." << endl;
        return;
    }

    bool higher_process_responded = false;

    for (const auto& p : processes) {
        if (p.id > initiator_id) {
            cout << "  -> Process " << initiator_id << " sends ELECTION to " << p.id << endl;
            
            if (p.active) {
                cout << "  <- Process " << p.id << " responds OK." << endl;
                higher_process_responded = true;
                
                start_election(p.id); 
                return; 
            } else {
                 cout << "  X  Process " << p.id << " does not respond (Crashed)." << endl;
            }
        }
    }

    if (!higher_process_responded) {
        announce_coordinator(initiator_id);
    }
}

int main() {
    int num_processes;
    
    cout << "Enter number of processes: ";
    cin >> num_processes;

    for (int i = 1; i <= num_processes; i++) {
        processes.push_back(Process(i));
    }

    coordinator_id = num_processes;
    cout << "Initial Coordinator is Process " << coordinator_id << endl;

    int choice;
    do {
        cout << "\n---------------------------------";
        cout << "\n1. Crash a process";
        cout << "\n2. Recover a process";
        cout << "\n3. Check current Coordinator";
        cout << "\n4. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch(choice) {
            case 1: {
                int crash_id;
                cout << "Enter Process ID to crash: ";
                cin >> crash_id;
                if (crash_id > 0 && crash_id <= num_processes) {
                    processes[crash_id - 1].active = false;
                    cout << "Process " << crash_id << " crashed." << endl;
                    
                    if (crash_id == coordinator_id) {
                        cout << "Coordinator has crashed!" << endl;
                        for(auto& p : processes) {
                            if (p.active && p.id != crash_id) {
                                start_election(p.id);
                                break;
                            }
                        }
                    }
                }
                break;
            }
            case 2: {
                int recover_id;
                cout << "Enter Process ID to recover: ";
                cin >> recover_id;
                if (recover_id > 0 && recover_id <= num_processes) {
                    processes[recover_id - 1].active = true;
                    cout << "Process " << recover_id << " recovered." << endl;
                    start_election(recover_id);
                }
                break;
            }
            case 3:
                cout << "Current Coordinator is Process " << coordinator_id << endl;
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }

    } while (choice != 4);

    return 0;
}