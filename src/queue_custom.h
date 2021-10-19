#include <iostream>
#include <queue>

using namespace std;

int maxQueueLength = 2;

class BeaconQueue
{
public:
    queue<int> beaconReadingQueue;

    void add(int adder)
    {
        if (beaconReadingQueue.size() == maxQueueLength)
        {
            beaconReadingQueue.pop();
        }
        beaconReadingQueue.push(adder);
        cout << beaconReadingQueue.front() << beaconReadingQueue.back() << endl;
    }

    int front()
    {
        return beaconReadingQueue.front();
    }
    int back()
    {
        return beaconReadingQueue.back();
    }

    int sum()
    {

        return beaconReadingQueue.front() + beaconReadingQueue.back();
    }
};