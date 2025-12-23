#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>

using namespace std;

// student class
class Student {
public:
    int listNumber;
    string name;
    string studentNumber;
    string section;
    string status;
    string address;
    string phoneNumber;
};

// auto incrementing list num 
int getNextListNumber() {
    ifstream file("students.txt");
    if (!file) return 1;

    string line;
    int highest = 0;

    while (getline(file, line)) {
        if (line.find("List Number: ") == 0) {
            int num;
            stringstream ss(line.substr(13));
            ss >> num;
            if (num > highest) highest = num;
        }
    }
    file.close();
    return highest + 1;
}

// add student
void addStudent() {
    Student s;
    s.listNumber = getNextListNumber();

    cout << "\nAssigned List Number: " << s.listNumber << endl;
    cin.ignore();

    cout << "Enter name: ";
    getline(cin, s.name);

    cout << "Enter student number: ";
    getline(cin, s.studentNumber);

    cout << "Enter section: ";
    getline(cin, s.section);

    cout << "Enter status: ";
    getline(cin, s.status);

    cout << "Enter address: ";
    getline(cin, s.address);

    cout << "Enter phone number: ";
    getline(cin, s.phoneNumber);

    ofstream file("students.txt", ios::app);
    if (!file) {
        cout << "Error opening file.\n";
        return;
    }

    file << "List Number: " << s.listNumber << endl;
    file << "Name: " << s.name << endl;
    file << "Student Number: " << s.studentNumber << endl;
    file << "Section: " << s.section << endl;
    file << "Status: " << s.status << endl;
    file << "Address: " << s.address << endl;
    file << "Phone Number: " << s.phoneNumber << endl;
    file << "-----------------------------" << endl;

    file.close();
    cout << "\nStudent added successfully!\n";
}

// display student
void displayStudents() {
    ifstream file("students.txt");
    if (!file) {
        cout << "\nNo students recorded yet.\n";
        return;
    }

    cout << "\n--- Current Students ---\n";
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

// delete student
void deleteStudent() {
    int listNum;
    cout << "\nEnter List Number to delete: ";
    cin >> listNum;

    ifstream file("students.txt");
    if (!file) {
        cout << "File not found.\n";
        return;
    }

    vector<string> keptLines;
    string line;
    bool skip = false;

    string target = "List Number: " + to_string(listNum);

    while (getline(file, line)) {
        if (line == target) {
            skip = true;
            continue;
        }
        if (skip && line == "-----------------------------") {
            skip = false;
            continue;
        }
        if (!skip) keptLines.push_back(line);
    }
    file.close();

    ofstream out("students.txt");
    for (string &l : keptLines) out << l << endl;
    out.close();

    cout << "\nStudent deleted successfully.\n";
}

// edit student
void editStudent() {
    ifstream file("students.txt");
    if (!file) {
        cout << "\nNo students to edit.\n";
        return;
    }

    int editNumber;
    cout << "\nEnter List Number to edit: ";
    cin >> editNumber;
    cin.ignore();

    ofstream temp("temp.txt");
    string line;
    bool found = false;

    while (getline(file, line)) {
        if (line.find("List Number: ") == 0) {
            int number;
            stringstream ss(line.substr(13));
            ss >> number;

            string name, studNo, section, status, address, phone;
            getline(file, name);
            getline(file, studNo);
            getline(file, section);
            getline(file, status);
            getline(file, address);
            getline(file, phone);
            getline(file, line); // separator

            if (number == editNumber) {
                found = true;
                char ch;
                string input;

                do {
                    cout << "\nEdit Menu\n";
                    cout << "a. Name\nb. Student Number\nc. Section\nd. Status\ne. Address\nf. Phone Number\ng. Done\n";
                    cout << "Choice: ";
                    cin >> ch;
                    cin.ignore();

                    if (ch == 'a') { cout << "New name: "; getline(cin, input); name = "Name: " + input; }
                    if (ch == 'b') { cout << "New student number: "; getline(cin, input); studNo = "Student Number: " + input; }
                    if (ch == 'c') { cout << "New section: "; getline(cin, input); section = "Section: " + input; }
                    if (ch == 'd') { cout << "New status: "; getline(cin, input); status = "Status: " + input; }
                    if (ch == 'e') { cout << "New address: "; getline(cin, input); address = "Address: " + input; }
                    if (ch == 'f') { cout << "New phone number: "; getline(cin, input); phone = "Phone Number: " + input; }

                } while (ch != 'g');
            }

            temp << "List Number: " << number << endl;
            temp << name << endl;
            temp << studNo << endl;
            temp << section << endl;
            temp << status << endl;
            temp << address << endl;
            temp << phone << endl;
            temp << "-----------------------------" << endl;
        }
    }

    file.close();
    temp.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        cout << "\nStudent updated successfully.\n";
    else
        cout << "\nStudent not found.\n";
}

// search student
void searchStudent() {
    ifstream file("students.txt");
    if (!file) {
        cout << "\nNo students recorded yet.\n";
        return;
    }

    int choice;
    cout << "\nSearch by:\n";
    cout << "1. Name\n";
    cout << "2. Student Number\n";
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    string keyword;
    cout << "Enter search value: ";
    getline(cin, keyword);

    string line;
    bool found = false;
    vector<string> block;
    bool collect = false;

    while (getline(file, line)) {

        if (choice == 1 && line.find("Name: ") == 0 && line.substr(6) == keyword)
            collect = found = true;

        if (choice == 2 && line.find("Student Number: ") == 0 && line.substr(16) == keyword)
            collect = found = true;

        if (collect) block.push_back(line);

        if (collect && line == "-----------------------------") {
            cout << "\n--- Student Found ---\n";
            for (string &l : block) cout << l << endl;
            block.clear();
            collect = false;
        }
    }

    if (!found) cout << "\nStudent not found.\n";
    file.close();
}

// main
int main() {
    int choice;

    do {
        displayStudents();

        cout << "\n--- Student Management System ---\n";
        cout << "1. Add Student\n";
        cout << "2. Delete Student\n";
        cout << "3. Edit Student\n";
        cout << "4. Search Student\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: editStudent(); break;
            case 4: searchStudent(); break;
            case 5: cout << "Exiting program...\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    return 0;
}
