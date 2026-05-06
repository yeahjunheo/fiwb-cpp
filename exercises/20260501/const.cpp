int main() {
    const int this_is_a_promise = 4;
    this_is_a_promise = ++this_is_a_promise;
}

/*
❯ g++ -std=c++17 -Wall const.cpp -o const      
const.cpp:3:25: error: cannot assign to variable 'this_is_a_promise'
      with const-qualified type 'const int'
    3 |     this_is_a_promise = ++this_is_a_promise;
      |                         ^ ~~~~~~~~~~~~~~~~~
const.cpp:2:15: note: variable 'this_is_a_promise' declared const here
    2 |     const int this_is_a_promise = 4;
      |     ~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~
1 error generated.
*/
// Pretty cool~~~
