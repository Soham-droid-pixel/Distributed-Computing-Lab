#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// The Token object that is passed between sites
struct Token {
    vector<int> LN; // Last request number satisfied for each site
    queue<int> Q;   // Queue of waiting sites
};

class Site {
public:
    int id;
    int numSites;
    vector<int> RN; // Local Request Numbers seen by this site
    bool hasToken;
    bool inCS;      // Is the site currently in Critical Section?

    Site(int i, int n, bool startToken) {
        id = i;
        numSites = n;
        RN.assign(n, 0);
        hasToken = startToken;
        inCS = false;
    }

    // Step 1: Requesting the Critical Section (CS)
    void requestCS(vector<Site>& sites, Token& token) {
        if (hasToken) {
            cout << "Site " << id << " already has the token." << endl;
            return;
        }

        // Increment local request count and broadcast
        RN[id]++;
        cout << "Site " << id << " requesting CS (Sequence: " << RN[id] << ")" << endl;

        for (int i = 0; i < numSites; i++) {
            if (i != id) {
                sites[i].receiveRequest(id, RN[id], sites, token);
            }
        }
    }

    // Step 2: Receiving a request from another site
    void receiveRequest(int senderId, int seqNum, vector<Site>& sites, Token& token) {
        RN[senderId] = max(RN[senderId], seqNum);

        // If site has idle token and sender's request is fresh, send token
        if (hasToken && !inCS && RN[senderId] == token.LN[senderId] + 1) {
            cout << "Site " << id << " sending token to Site " << senderId << endl;
            hasToken = false;
            sites[senderId].takeToken(token);
        }
    }

    void takeToken(Token& token) {
        hasToken = true;
        cout << "Site " << id << " received the token. Entering CS..." << endl;
        inCS = true;
    }

    // Step 3: Releasing the Critical Section
    void releaseCS(Token& token, vector<Site>& sites) {
        cout << "Site " << id << " finishing work. Releasing CS..." << endl;
        inCS = false;
        token.LN[id] = RN[id]; // Mark this request as finished in token

        // Check for outstanding requests to add to token queue
        for (int j = 0; j < numSites; j++) {
            bool alreadyInQueue = false;
            // Simplified check: usually the token Q is unique IDs
            if (RN[j] == token.LN[j] + 1) {
                token.Q.push(j);
            }
        }

        // If queue isn't empty, pass token to next site
        if (!token.Q.empty()) {
            int nextSite = token.Q.front();
            token.Q.pop();
            hasToken = false;
            cout << "Site " << id << " passing token from queue to Site " << nextSite << endl;
            sites[nextSite].takeToken(token);
        }
    }
};

int main() {
    int n = 5; // Total sites
    vector<Site> sites;
    Token token;
    token.LN.assign(n, 0);

    // Initialize sites, Site 0 starts with the token
    for (int i = 0; i < n; i++) {
        sites.push_back(Site(i, n, (i == 0)));
    }

    // Simulation Flow
    sites[2].requestCS(sites, token); // Site 2 wants access
    sites[2].releaseCS(token, sites); // Site 2 finishes

    sites[4].requestCS(sites, token); // Site 4 wants access
    
    return 0;
}