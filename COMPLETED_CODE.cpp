#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>

using namespace std;

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

const string FILENAME = "students.txt";

/* ---------- REGEX VALIDATION ---------- */

bool validateName(const string& name) {
    return regex_match(name, regex("^[A-Za-z]+( [A-Za-z]+)*$"));
}

bool validateStudentNumber(const string& sn) {
    return regex_match(sn, regex("^\\d{4}-\\d{5}-[A-Z]{2}-\\d$"));
}

bool validateSection(const string& section) {
    return regex_match(section, regex("^[A-Z]{3,4} \\d-\\d$"));
}

bool validateStatus(const string& status) {
    return regex_match(status, regex("^(Regular|Irregular)$"));
}

bool validatePhone(const string& phone) {
    return regex_match(phone, regex("^(09\\d{9}|\\+63\\d{10})$"));
}

/* ---------- FILE UTILITIES ---------- */

int getNextListNumber() {
    ifstream file(FILENAME);
    int last = 0;
    string line;

    while (getline(file, line)) {
        if (line == "---") continue;
        last = stoi(line);
        for (int i = 0; i < 6; i++) getline(file, line);
    }
    return last + 1;
}

vector<Student> loadStudents() {
    vector<Student> students;
    ifstream file(FILENAME);
    Student s;

    while (file >> s.listNumber) {
        file.ignore();
        getline(file, s.name);
        getline(file, s.studentNumber);
        getline(file, s.section);
        getline(file, s.status);
        getline(file, s.address);
        getline(file, s.phoneNumber);
        file.ignore(3);
        students.push_back(s);
    }
    return students;
}

void saveStudents(const vector<Student>& students) {
    ofstream file(FILENAME);
    for (const auto& s : students) {
        file << s.listNumber << "\n"
             << s.name << "\n"
             << s.studentNumber << "\n"
             << s.section << "\n"
             << s.status << "\n"
             << s.address << "\n"
             << s.phoneNumber << "\n---\n";
    }
}

/* ---------- DISPLAY ---------- */

void displayStudents() {
    vector<Student> students = loadStudents();
    if (students.empty()) {
        cout << "No students found.\n";
        return;
    }

    cout << "\n--- CURRENT STUDENTS ---\n";
    for (const auto& s : students) {
        cout << s.listNumber << " | "
             << s.name << " | "
             << s.studentNumber << " | "
             << s.section << " | "
             << s.status << "\n";
    }
}

/* ---------- ADD ---------- */

void addStudent() {
    Student s;
    s.listNumber = getNextListNumber();

    do {
        cout << "Name: ";
        getline(cin, s.name);
    } while (!validateName(s.name));

    do {
        cout << "Student Number: ";
        getline(cin, s.studentNumber);
    } while (!validateStudentNumber(s.studentNumber));

    do {
        cout << "Section: ";
        getline(cin, s.section);
    } while (!validateSection(s.section));

    do {
        cout << "Status (Regular/Irregular): ";
        getline(cin, s.status);
    } while (!validateStatus(s.status));

    cout << "Address: ";
    getline(cin, s.address);

    do {
        cout << "Phone Number: ";
        getline(cin, s.phoneNumber);
    } while (!validatePhone(s.phoneNumber));

    ofstream file(FILENAME, ios::app);
    file << s.listNumber << "\n"
         << s.name << "\n"
         << s.studentNumber << "\n"
         << s.section << "\n"
         << s.status << "\n"
         << s.address << "\n"
         << s.phoneNumber << "\n---\n";

    cout << "Student added successfully.\n";
}

/* ---------- SEARCH ---------- */

void searchStudent() {
    vector<Student> students = loadStudents();
    string key;
    cout << "Enter name or student number: ";
    getline(cin, key);

    bool found = false;
    for (const auto& s : students) {
        if (s.name == key || s.studentNumber == key) {
            cout << "\nFOUND:\n";
            cout << s.listNumber << "\n"
                 << s.name << "\n"
                 << s.studentNumber << "\n"
                 << s.section << "\n"
                 << s.status << "\n"
                 << s.address << "\n"
                 << s.phoneNumber << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "Student not found.\n";
}

/* ---------- DELETE ---------- */

void deleteStudent() {
    vector<Student> students = loadStudents();
    string sn;
    cout << "Enter student number to delete: ";
    getline(cin, sn);

    vector<Student> updated;
    bool deleted = false;

    for (const auto& s : students) {
        if (s.studentNumber != sn)
            updated.push_back(s);
        else
            deleted = true;
    }

    saveStudents(updated);
    cout << (deleted ? "Student deleted.\n" : "Student not found.\n");
}

/* ---------- EDIT ---------- */

void editStudent() {
    vector<Student> students = loadStudents();
    string sn;
    cout << "Enter student number to edit: ";
    getline(cin, sn);

    bool found = false;
    for (auto& s : students) {
        if (s.studentNumber == sn) {
            found = true;

            do {
                cout << "New Name: ";
                getline(cin, s.name);
            } while (!validateName(s.name));

            do {
                cout << "New Section: ";
                getline(cin, s.section);
            } while (!validateSection(s.section));

            do {
                cout << "New Status: ";
                getline(cin, s.status);
            } while (!validateStatus(s.status));

            cout << "New Address: ";
            getline(cin, s.address);

            do {
                cout << "New Phone: ";
                getline(cin, s.phoneNumber);
            } while (!validatePhone(s.phoneNumber));
        }
    }

    saveStudents(students);
    cout << (found ? "Student updated.\n" : "Student not found.\n");
}

/* ---------- MAIN ---------- */

int main() {
    int choice;

    do {
        displayStudents();
        cout << "\n1. Add Student\n2. Delete Student\n3. Edit Student\n4. Search Student\n0. Exit\nChoice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: addStudent(); break;
        case 2: deleteStudent(); break;
        case 3: editStudent(); break;
        case 4: searchStudent(); break;
        case 0: cout << "Exiting...\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
