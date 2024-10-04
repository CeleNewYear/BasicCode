#include <fstream>
#include <iostream>
using namespace std;

int main()
{
    // using ofstream for output file operation
    ofstream file;

    // Opening finle in write mode
    file.open("sample.txt");

    // Check if the file was successfully created
    if (!file.is_open()) {
        cout << "Error in creating file" << endl;
        return 1;
    }
    cout << "File created successfully" << endl;

    // Close the file to free up resources
    file.close();
    return 0;
}