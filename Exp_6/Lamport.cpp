#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void displayTimestamps(const vector<vector<int>>& processEvents, int numProcesses) {
    cout << "\n--- Logical Clock Timestamps ---\n";
    for (int i = 0; i < numProcesses; i++) {
        cout << "Process P" << i + 1 << ": ";
        for (int j = 0; j < processEvents[i].size(); j++) {
            cout << processEvents[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int numProcesses, numEvents;

    cout << "Enter the number of processes: ";
    cin >> numProcesses;

    vector<vector<int>> p(numProcesses);

    for (int i = 0; i < numProcesses; i++) {
        cout << "Enter number of events in Process P" << i + 1 << ": ";
        cin >> numEvents;
        
        for (int j = 1; j <= numEvents; j++) {
            p[i].push_back(j);
        }
    }

    char choice;
    do {
        int senderP, senderE, receiverP, receiverE;

        cout << "\nEnter communication (SenderProcess SenderEvent ReceiverProcess ReceiverEvent): ";
        cin >> senderP >> senderE >> receiverP >> receiverE;

        senderP--; senderE--; receiverP--; receiverE--;

        if (p[receiverP][receiverE] <= p[senderP][senderE]) {
            p[receiverP][receiverE] = p[senderP][senderE] + 1;

            for (int k = receiverE + 1; k < p[receiverP].size(); k++) {
                p[receiverP][k] = p[receiverP][k - 1] + 1;
            }
        }

        displayTimestamps(p, numProcesses);

        cout << "\nAdd another communication? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}