#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

class Course;

class Student {
public:
    string studentID, name, email;
    vector<string> coursesEnrolled;

    Student() {}
    Student(string id, string n, string e) : studentID(id), name(n), email(e) {}

    void enrollCourse(const string& courseCode);
    void dropCourse(const string& courseCode);
    void viewCourses();
    void save();
};

class Teacher {
public:
    string teacherID, name, email;
    vector<string> coursesTaught;

    Teacher() {}
    Teacher(string id, string n, string e) : teacherID(id), name(n), email(e) {}

    void assignCourse(const string& courseCode);
    void removeCourse(const string& courseCode);
    void viewCourses();
    void save();
};

class Course {
public:
    string courseCode, courseName;
    string teacherID;
    int capacity;
    vector<string> studentsEnrolled;

    Course() {}
    Course(string code, string name, string tID, int cap) :
        courseCode(code), courseName(name), teacherID(tID), capacity(cap) {}

    bool addStudent(const string& studentID);
    void removeStudent(const string& studentID);
    void viewStudents();
    void save();
};
map<string, Student> students;
map<string, Teacher> teachers;
map<string, Course> courses;
vector<string> split(const string& line, char delim = ',') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}

void loadStudents() {
    ifstream fin("students.txt");
    string line;
    while (getline(fin, line)) {
        auto tokens = split(line);
        Student s(tokens[0], tokens[1], tokens[2]);
        for (int i = 3; i < tokens.size(); i++) {
            s.coursesEnrolled.push_back(tokens[i]);
        }
        students[s.studentID] = s;
    }
    fin.close();
}

void loadTeachers() {
    ifstream fin("teachers.txt");
    string line;
    while (getline(fin, line)) {
        auto tokens = split(line);
        Teacher t(tokens[0], tokens[1], tokens[2]);
        for (int i = 3; i < tokens.size(); i++) {
            t.coursesTaught.push_back(tokens[i]);
        }
        teachers[t.teacherID] = t;
    }
    fin.close();
}

void loadCourses() {
    ifstream fin("courses.txt");
    string line;
    while (getline(fin, line)) {
        auto tokens = split(line);
        Course c(tokens[0], tokens[1], tokens[2], stoi(tokens[3]));
        for (int i = 4; i < tokens.size(); i++) {
            c.studentsEnrolled.push_back(tokens[i]);
        }
        courses[c.courseCode] = c;
    }
    fin.close();
}

void saveAll() {
    ofstream fout1("students.txt"), fout2("teachers.txt"), fout3("courses.txt");
    for (auto& [id, s] : students) {
        fout1 << s.studentID << "," << s.name << "," << s.email;
        for (auto& c : s.coursesEnrolled) fout1 << "," << c;
        fout1 << "\n";
    }
    for (auto& [id, t] : teachers) {
        fout2 << t.teacherID << "," << t.name << "," << t.email;
        for (auto& c : t.coursesTaught) fout2 << "," << c;
        fout2 << "\n";
    }
    for (auto& [code, c] : courses) {
        fout3 << c.courseCode << "," << c.courseName << "," << c.teacherID << "," << c.capacity;
        for (auto& s : c.studentsEnrolled) fout3 << "," << s;
        fout3 << "\n";
    }
    fout1.close(); fout2.close(); fout3.close();
}

void Student::enrollCourse(const string& courseCode) {
    if (find(coursesEnrolled.begin(), coursesEnrolled.end(), courseCode) != coursesEnrolled.end()) {
        cout << "Already enrolled in " << courseCode << endl;
        return;
    }
    if (courses[courseCode].addStudent(studentID)) {
        coursesEnrolled.push_back(courseCode);
        cout << "Enrollment successful.\n";
    } else {
        cout << "Enrollment failed. Course full.\n";
    }
}

void Student::dropCourse(const string& courseCode) {
    auto it = find(coursesEnrolled.begin(), coursesEnrolled.end(), courseCode);
    if (it != coursesEnrolled.end()) {
        coursesEnrolled.erase(it);
        courses[courseCode].removeStudent(studentID);
        cout << "Dropped course: " << courseCode << endl;
    } else {
        cout << "Not enrolled in course.\n";
    }
}

void Student::viewCourses() {
    cout << "Courses for " << name << ":\n";
    for (auto& c : coursesEnrolled) {
        cout << "- " << c << " (" << courses[c].courseName << ")\n";
    }
}

void Teacher::assignCourse(const string& courseCode) {
    if (find(coursesTaught.begin(), coursesTaught.end(), courseCode) == coursesTaught.end()) {
        coursesTaught.push_back(courseCode);
    }
}

void Teacher::removeCourse(const string& courseCode) {
    auto it = find(coursesTaught.begin(), coursesTaught.end(), courseCode);
    if (it != coursesTaught.end()) {
        coursesTaught.erase(it);
    }
}

void Teacher::viewCourses() {
    cout << "Courses taught by " << name << ":\n";
    for (auto& c : coursesTaught) {
        cout << "- " << c << " (" << courses[c].courseName << ")\n";
    }
}

bool Course::addStudent(const string& studentID) {
    if (studentsEnrolled.size() >= capacity) return false;
    if (find(studentsEnrolled.begin(), studentsEnrolled.end(), studentID) == studentsEnrolled.end()) {
        studentsEnrolled.push_back(studentID);
        return true;
    }
    return false;
}

void Course::removeStudent(const string& studentID) {
    auto it = find(studentsEnrolled.begin(), studentsEnrolled.end(), studentID);
    if (it != studentsEnrolled.end()) studentsEnrolled.erase(it);
}

void Course::viewStudents() {
    cout << "Students in " << courseCode << " (" << courseName << "):\n";
    for (auto& sid : studentsEnrolled) {
        cout << "- " << students[sid].name << endl;
    }
}

void menu() {
    int choice;
    while (true) {
        cout << "\n--- University Management System ---\n";
        cout << "1. Add Student\n2. Add Teacher\n3. Add Course\n4. Enroll Student\n5. Drop Course\n6. View Student Courses\n7. View Teacher Courses\n8. View Course Students\n9. Save and Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            string id, name, email;
            cout << "Enter ID, name, email: ";
            cin >> id >> name >> email;
            students[id] = Student(id, name, email);
        } else if (choice == 2) {
            string id, name, email;
            cout << "Enter ID, name, email: ";
            cin >> id >> name >> email;
            teachers[id] = Teacher(id, name, email);
        } else if (choice == 3) {
            string code, name, tid;
            int cap;
            cout << "Enter course code, name, teacher ID, capacity: ";
            cin >> code >> name >> tid >> cap;
            courses[code] = Course(code, name, tid, cap);
            teachers[tid].assignCourse(code);
        } else if (choice == 4) {
            string sid, code;
            cout << "Enter student ID and course code: ";
            cin >> sid >> code;
            students[sid].enrollCourse(code);
        } else if (choice == 5) {
            string sid, code;
            cout << "Enter student ID and course code: ";
            cin >> sid >> code;
            students[sid].dropCourse(code);
        } else if (choice == 6) {
            string sid;
            cout << "Enter student ID: ";
            cin >> sid;
            students[sid].viewCourses();
        } else if (choice == 7) {
            string tid;
            cout << "Enter teacher ID: ";
            cin >> tid;
            teachers[tid].viewCourses();
        } else if (choice == 8) {
            string code;
            cout << "Enter course code: ";
            cin >> code;
            courses[code].viewStudents();
        } else if (choice == 9) {
            saveAll();
            cout << "Data saved. Exiting.\n";
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}



int main() {
    loadStudents();
    loadTeachers();
    loadCourses();
    menu();
    return 0;
}
