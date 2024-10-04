// C++ program to demonstrate read/write of class objects in C++
#include <cstring>
#include <fstream>
#include <iostream>
using namespace std;

class Contestant {
public:
    // Instance variables
    char Name[32];
    int Age, Ratings;

    // Function declaration of imput() to input info
    int write(string str, int age, int ratings);

    // Function declaration of output_highest_rated() to
    // extract info from file Data Base
    int read();
};

int Contestant::write(string str, int age, int ratings)
{
    // Object to write in file
    ofstream file_obj;

    // Opening file in append mode
    file_obj.open("Input.txt", ios::app);

    // Object of the class
    Contestant obj;

    // Assigning data to the object
    strcpy(obj.Name, str.c_str());
    obj.Age = age;
    obj.Ratings = ratings;

    // Wirting the object's data in file
    file_obj.write((char*)&obj, sizeof(obj));

    // Closing the opened file
    file_obj.close();

    return 0;
}

int Contestant::read()
{
    // Object to read from file
    ifstream file_obj;

    // Opening file in input mode
    file_obj.open("Input.txt", ios::in);

    // Object of the class
    Contestant obj;

    // Reading from file into object "obj"
    file_obj.read((char*)&obj, sizeof(obj));

    // max to store maximum ratings
    int max = 0;

    // Highest_rated stores the name of highest rated contestant
    string Highest_rated;

    // Checking till we have the feed
    while (!file_obj.eof()) {
        // Assigning max ratings
        if (obj.Ratings > max) {
            max = obj.Ratings;
            Highest_rated = obj.Name;
        }
        // Checking further
        file_obj.read((char*)&obj, sizeof(obj));
    }

    // Closing the opened file
    file_obj.close();

    // Output is the highest rated contestant
    cout << Highest_rated;
    return 0;
}

// Driver code
int main()
{
    // Creating object of the class
    Contestant obj;

    // Writing in the file
    obj.write("Aman", 21, 100);
    obj.write("Raman", 22, 200);
    obj.write("Shubham", 23, 400);
    obj.write("Rahul", 24, 300);
    obj.write("Rohit", 25, 50);

    // Reading from the file
    obj.read();

    return 0;
}