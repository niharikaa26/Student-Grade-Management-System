#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>

// Function to calculate grade based on marks
std::string calculateGrade(double marks) {
    if (marks >= 90) return "A+";
    else if (marks >= 80) return "A";
    else if (marks >= 70) return "B";
    else if (marks >= 60) return "C";
    else if (marks >= 50) return "D";
    else return "F";
}

// Student class to store individual student information and grades
class Student {
private:
    std::string studentID;
    std::string name;
    std::unordered_map<std::string, double> marks; // Course -> Marks
    std::unordered_map<std::string, std::string> grades; // Course -> Grade

public:
    Student(std::string id, std::string name) : studentID(id), name(name) {}

    std::string getID() const { return studentID; }
    std::string getName() const { return name; }
    
    void addMarks(const std::string &course, double mark) {
        marks[course] = mark;
        grades[course] = calculateGrade(mark);
    }
    
    void modifyMarks(const std::string &course, double newMark) {
        if (marks.find(course) != marks.end()) {
            marks[course] = newMark;
            grades[course] = calculateGrade(newMark);
        } else {
            std::cerr << "Marks for course not found.\n";
        }
    }
    
    double getMarks(const std::string &course) const {
        if (marks.find(course) != marks.end()) {
            return marks.at(course);
        } else {
            std::cerr << "Marks for course not found.\n";
            return -1; // Indicator of error
        }
    }
    
    std::string getGrade(const std::string &course) const {
        if (grades.find(course) != grades.end()) {
            return grades.at(course);
        } else {
            std::cerr << "Grade for course not found.\n";
            return "N/A"; // Indicator of error
        }
    }
    
    std::unordered_map<std::string, double> getAllMarks() const {
        return marks;
    }
    
    std::unordered_map<std::string, std::string> getAllGrades() const {
        return grades;
    }
};

// GradeManager class to manage multiple students
class GradeManager {
private:
    std::vector<Student> students;

public:
    void addStudent(const Student &student) {
        students.push_back(student);
    }
    
    const Student* findStudentByID(const std::string &id) const {
        for (const auto &student : students) {
            if (student.getID() == id) {
                return &student;
            }
        }
        return nullptr; // Indicator of error
    }
    
    std::vector<Student> getStudents() const {
        return students;
    }
};

// Function to enter marks for a student
void marksEntry(GradeManager &gm) {
    std::string id, name, course;
    double mark;
    std::cout << "Enter student ID: ";
    std::cin >> id;
    std::cout << "Enter student name: ";
    std::cin.ignore(); // To handle the newline character
    std::getline(std::cin, name);
    Student student(id, name);
    
    std::cout << "Enter course name: ";
    std::cin >> course;
    std::cout << "Enter marks: ";
    std::cin >> mark;
    student.addMarks(course, mark);
    
    gm.addStudent(student);
}

// Function to modify marks for a student
void marksModification(GradeManager &gm) {
    std::string id, course;
    double newMark;
    std::cout << "Enter student ID: ";
    std::cin >> id;
    Student* student = const_cast<Student*>(gm.findStudentByID(id));
    
    if (student) {
        std::cout << "Enter course name: ";
        std::cin >> course;
        std::cout << "Enter new marks: ";
        std::cin >> newMark;
        student->modifyMarks(course, newMark);
    } else {
        std::cerr << "Student not found.\n";
    }
}

// Function to retrieve marks and grades for a student
void marksRetrieval(const GradeManager &gm) {
    std::string id;
    std::cout << "Enter student ID: ";
    std::cin >> id;
    const Student* student = gm.findStudentByID(id);
    
    if (student) {
        auto marks = student->getAllMarks();
        auto grades = student->getAllGrades();
        for (const auto &entry : marks) {
            std::cout << "Course: " << entry.first << " | Marks: " << entry.second << " | Grade: " << grades.at(entry.first) << '\n';
        }
    } else {
        std::cerr << "Student not found.\n";
    }
}

// Function to generate a report summary
void reportSummary(const GradeManager &gm) {
    for (const auto &student : gm.getStudents()) {
        std::cout << "Student ID: " << student.getID() << " | Name: " << student.getName() << '\n';
        auto marks = student.getAllMarks();
        auto grades = student.getAllGrades();
        for (const auto &entry : marks) {
            std::cout << "Course: " << entry.first << " | Marks: " << entry.second << " | Grade: " << grades.at(entry.first) << '\n';
        }
        std::cout << "--------------------------\n";
    }
}

// Authenticator class to manage user authentication
class Authenticator {
private:
    std::unordered_map<std::string, std::string> credentials; // Username -> Password

public:
    Authenticator() {
        // Default credentials
        credentials["admin"] = "password123";
    }

    bool login(const std::string &username, const std::string &password) {
        if (credentials.find(username) != credentials.end() && credentials[username] == password) {
            return true;
        }
        return false;
    }
};

// Main function to run the program
int main() {
    GradeManager gm;
    Authenticator auth;
    std::string username, password;

    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    if (auth.login(username, password)) {
        int choice;
        do {
            std::cout << "1. Enter Marks\n2. Modify Marks\n3. Retrieve Marks and Grades\n4. Report Summary\n5. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    marksEntry(gm);
                    break;
                case 2:
                    marksModification(gm);
                    break;
                case 3:
                    marksRetrieval(gm);
                    break;
                case 4:
                    reportSummary(gm);
                    break;
                case 5:
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cerr << "Invalid choice. Try again.\n";
            }
        } while (choice != 5);
    } else {
        std::cerr << "Authentication failed. Exiting...\n";
    }

    return 0;
}
