//
// Created by Frank on 2020-04-03.
//

#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s = "11";
    char ch = '+';

    s.push_back(ch);
    cout << s << endl;

    cout << "len : " << s.length() << endl;
    return 0;
}