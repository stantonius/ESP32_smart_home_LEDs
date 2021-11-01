#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

#define LOG(x) cout << x << endl

class DeviceProximityHolder
{
public:
    vector<int> holder;

    // I always get confused by this : notation following the class constructor
    // however this is the only place you can legally initialise member variables with data
    DeviceProximityHolder() : holder(2)
    {
        // initialise the vector with 3 elements, all with value 0
        holder.reserve(3);
    }

    void add(int adder)
    {
        // emplace is used instead of insert as it does not copy the data but rather updates vector directly
        holder.emplace(holder.begin(), adder);
        // I think that because we have set the vector to start as a size of 3,
        // we don't have to track size and always add an then pop the last element
        holder.pop_back();
    }

    int sum()
    {
        // standard library function that sums all the elements in the vector
        // possible because the vector data is contiguous
        return accumulate(holder.begin(), holder.end(), 0);
    }
};