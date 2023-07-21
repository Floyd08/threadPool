#include <iostream>
#include <queue>
#include <string>

using namespace std;

int main(void) {
    
    string hello = "Hello!";

    queue<string> q;

    for (int i = 0; i < 6; ++i) {
        q.push(hello + to_string(i) + "\n");
    }

    while(!q.empty()) {
        cout << q.front();
        q.pop();
    }

    return 0;
}