#include <ios>
#include <iostream>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int A, B;
  cin >> A >> B;
  int count = (B + A - 3) / (A - 1);
  cout << count << "\n";
}

/*
we have cords with A extra sockets
each time we add another socket, we have to subtract by 1 to account for
the extra socket taking over one socket in the previous one.
TOT = A * count - (count - 1) would be the formula
if that's the case, then we just follow thing to check how many sockets we'll need.

Since B can also be 1, then essentially we can say
TOT = 1 + A(count) - count
    = 1 + (A-1)*count
count = (TOT - 1) / (A - 1)
*/