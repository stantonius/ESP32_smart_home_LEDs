#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

#define LOG(x) cout << x << endl

class DeviceProximityHolder
{
public:
    vector<int> holder;
    DeviceProximityHolder() : holder(2)
    {
        holder.reserve(3);
    }

    void add(int adder)
    {
        holder.emplace(holder.begin(), adder);
        // I think that because we have set the vector to start as a size of 2,
        // we don't have to track size and always add an then pop the last element
        holder.pop_back();
    }

    int sum()
    {
        return accumulate(holder.begin(), holder.end(), 0);
    }
};