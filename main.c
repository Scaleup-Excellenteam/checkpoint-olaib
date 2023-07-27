#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LEN 50
#define NUM_COURSES 10
#define NUM_CLASSES 10
#define NUM_LEVELS 7
#define PHONE_LEN 11
#define DB_INPUT_FILE "students.txt"

#define ERROR_OPENING_FILE "Error opening file"

#define INVALID_INPUT "Invalid input, try again\n"

#define ALLOC_ERROR "Memory allocation error.\n"
#define LINE_BREAK "========================================\n"

#define INSERT_PHONE "Enter the phone number of the student: "
#define INSERT_LEVEL "Enter the level of the student: "
#define INSERT_CLASS_ID "Enter the class id of the student: "
#define INSERT_GRADE "Enter the grade of the student in subject "
#define INSERT_FNAME "Enter the first name of the student: "
#define INSERT_LNAME "Enter the last name of the student: "
#define INSERTED_SUCCESSFULLY "Student inserted successfully.\n"
#define STUDENT_NOT_FOUND "Student not found.\n"
#define STUDENT_UPDATED "Student updated successfully.\n"
#define STUDENT_DELETED "Student deleted successfully.\n"


const char* SUBJECTS_NAMES[NUM_COURSES] = {
    "Mathematics", "English", "Cpp","Python", "Java", "C", "C#", "PHP", "HTML", "CSS"
};


struct course {
    int subject_id;
    int grade;
};

struct student {
    char fName[MAX_LEN];
    char lName[MAX_LEN];
    char phone[PHONE_LEN];
    int level;
    struct course courses[NUM_COURSES];
    struct student* next;
};

struct subjectName {
    char name[MAX_LEN];
};

struct school {
    struct student* DB[NUM_LEVELS][NUM_CLASSES];
    int num_students;
    struct subjectName subjects[NUM_COURSES];
};

static struct school S;

// ======================= Function Prototypes =======================
void clrscr();
void updateStudent();
void deleteStudent();
void insertStudent();
void displayStudents();
void displayInfo(struct student* student, bool full_info);
void initDb();
void displayMenu();
void searchStudentByFnameByLName();
void freeMemory();
void readFirstNameLastName(char* fName, char* lName);
bool readInput(char* var, int max_len, int min_len, char* msg, char* error_msg);
bool readDigitInput(int* var, int min, int max, char* msg, char* error_msg);
bool fNameCompare(char* fName1, char* fName2);
bool lNameCompare(char* lName1, char* lName2);


int fNameMatch(const struct student* current, const char* pattern) {
    return strcasecmp(current->fName, pattern);
}

void searchBy(int (*check)(const struct student* current, const char* value)) {
    char pattern[MAX_LEN];
    scanf("%s", pattern);

    int find = 0;
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* temp = S.DB[level][class_id];
            while (temp != NULL) {
                if (check(temp, pattern) == 0) {
                    find = 1;
                    printf("<<Target student found! Details:>>\n");
                    displayInfo(temp, true);
                    break;
                }
                temp = temp->next;
            }
        }
    }

    if (find == 0)
        printf(STUDENT_NOT_FOUND);
}

void lNameMatch(const struct student* current, const char* pattern) {
    return strcasecmp(current->lName, pattern);
}

void fullNameMatch(const struct student* current, const char* pattern) {
    char fullName[MAX_LEN];
    strcpy(fullName, current->fName);
    strcat(fullName, " ");
    strcat(fullName, current->lName);
    printf("%s\n", fullName);
    return strcasecmp(fullName, pattern);
}

// ======================= Main Function =======================

int main() {
    initDb();
    // printf("select search by:\n");
    // printf("1. first name\n");
    // printf("2. last name\n");
    // printf("3. first and last name\n");



    displayMenu();
    return EXIT_SUCCESS;
}

void clrscr() {
    system("@cls||clear");
}
void initDb() {
    FILE* file = fopen(DB_INPUT_FILE, "r");
    if (file == NULL) {
        printf(ERROR_OPENING_FILE);
        return;
    }

    S.num_students = 0;
    int level, class_id;
    char fName[MAX_LEN], lName[MAX_LEN], phone[PHONE_LEN];
    int grades[NUM_COURSES];

    // Initialize the DB array to store linked lists for students of each level and class_id
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            S.DB[level][class_id] = NULL; // Initialize all pointers to NULL
        }
    }

    while (fscanf(file, "%s %s %s %d %d", fName, lName, phone, &level, &class_id) == 5) {
        struct student* new_student = malloc(sizeof(struct student));
        if (new_student == NULL) {
            printf("Memory allocation error.\n");
            return EXIT_FAILURE;
        }

        strncpy(new_student->fName, fName, MAX_LEN);
        strncpy(new_student->lName, lName, MAX_LEN);
        strncpy(new_student->phone, phone, PHONE_LEN);
        new_student->level = level;

        for (int i = 0; i < NUM_CLASSES; i++) {
            if (fscanf(file, "%d", &grades[i]) != 1) {
                printf("Invalid input in file.\n");
                free(new_student);
                return;
            }
            new_student->courses[i].subject_id = i;
            new_student->courses[i].grade = grades[i];
        }

        // Insert the new student into the linked list for the corresponding level and class_id
        new_student->next = S.DB[level - 1][class_id - 1];
        S.DB[level - 1][class_id - 1] = new_student;
        S.num_students++;
    }

    printf("num of students: %d\n", S.num_students);

    for (int i = 0; i < NUM_COURSES; i++) {
        strncpy(S.subjects[i].name, SUBJECTS_NAMES[i], MAX_LEN);
    }

    fclose(file);
}


void displayStudents() {
    int count = 0;
    printf("=========== List of Students: ===========\n");
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                printf("%d. ", ++count);
                displayInfo(current_student, false);
                current_student = current_student->next;
            }
        }
    }
    printf(LINE_BREAK);
}

bool readInput(char* var, int max_len, int min_len, char* msg, char* error_msg) {
    printf("%s", msg);
    if (scanf("%s", var) != 1 || strlen(var) < min_len || strlen(var) > max_len) {
        perror(error_msg);
        return false;
    }
    return true;
}

bool readDigitInput(int* var, int min, int max, char* msg, char* error_msg) {
    printf("%s", msg);
    if (scanf("%d", var) != 1 || *var < min || *var > max) {
        perror(error_msg);
        return false;
    }
    return true;
}

void insertStudent() {
    char fName[MAX_LEN], lName[MAX_LEN], phone[PHONE_LEN];
    int level, class_id;
    int grades[NUM_CLASSES];
    bool is_valid_input = false;
    while (!is_valid_input)
    {
        readFirstNameLastName(fName, lName);
        is_valid_input = true;

        if (!readInput(phone, PHONE_LEN, PHONE_LEN, INSERT_PHONE, "Invalid phone number.\n"))
        {
            is_valid_input = false;
            continue;
        }

        if (!readDigitInput(&level, 1, NUM_LEVELS, INSERT_LEVEL, "Invalid level.\n"))
        {
            is_valid_input = false;
            continue;
        }

        if (!readDigitInput(&class_id, 1, NUM_CLASSES, INSERT_CLASS_ID, "Invalid class id.\n"))
        {
            is_valid_input = false;
            continue;
        }
    }

    struct student* new_student = malloc(sizeof(struct student));
    if (new_student == NULL) {
        printf(ALLOC_ERROR);
        return;
    }

    strncpy(new_student->fName, fName, MAX_LEN);
    strncpy(new_student->lName, lName, MAX_LEN);
    strncpy(new_student->phone, phone, PHONE_LEN);
    new_student->level = level;

    for (int i = 0; i < NUM_CLASSES; i++) {
        readDigitInput(&grades[i], 0, 100, INSERT_GRADE, "Invalid grade.\n");
        new_student->courses[i].subject_id = i;
        new_student->courses[i].grade = grades[i];
    }

    new_student->next = S.DB[level - 1][class_id - 1];
    S.DB[level - 1][class_id - 1] = new_student;
    S.num_students++;
    printf(INSERTED_SUCCESSFULLY);
}

void deleteStudent() {
    char firstName[MAX_LEN], lastName[MAX_LEN];
    readFirstNameLastName(firstName, lastName);


    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            struct student* prev_student = NULL;
            while (current_student != NULL) {
                if (strcmp(current_student->fName, firstName) == 0 &&
                    strcmp(current_student->lName, lastName) == 0) {
                    if (prev_student != NULL)
                        prev_student->next = current_student->next;
                    else
                        S.DB[level][class_id] = current_student->next;

                    free(current_student);
                    S.num_students--;
                    printf(STUDENT_DELETED);
                    return;
                }
                prev_student = current_student;
                current_student = current_student->next;
            }
        }
    }
    printf(STUDENT_NOT_FOUND);
}

void readFirstNameLastName(char* fName, char* lName) {
    char names[2][MAX_LEN];
    bool is_valid_input = false;
    while (!is_valid_input) {
        if (!readInput(names[0], MAX_LEN, 1, INSERT_FNAME, INVALID_INPUT))
            continue;
        if (!readInput(names[1], MAX_LEN, 1, INSERT_LNAME, INVALID_INPUT))
            continue;
        is_valid_input = true;
    }
    strncpy(fName, names[0], MAX_LEN);
    strncpy(lName, names[1], MAX_LEN);
}

bool getUserAnswer() {
    char answer;
    bool is_valid_input = false;

    while (!is_valid_input) {
        printf("Do you want to update the grades of the student? (y/n)\n");
        scanf(" %c", &answer);
        if (answer == 'y' || answer == 'Y')
            return true;
        else if (answer == 'n' || answer == 'N')
            return false;
        else
            printf(INVALID_INPUT);
    }

}
void updateStudent() {
    char fName[MAX_LEN], lName[MAX_LEN], phone[PHONE_LEN];
    int level, class_id;
    readFirstNameLastName(fName, lName);
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                if (fNameCompare(current_student->fName, fName) && lNameCompare(current_student->lName, lName)) {
                    if (!readInput(phone, PHONE_LEN, PHONE_LEN, INSERT_PHONE, "Invalid phone number.\n"))
                        continue;
                    strncpy(current_student->phone, phone, PHONE_LEN);
                    if (!readDigitInput(&level, 1, NUM_LEVELS, INSERT_LEVEL, "Invalid level.\n"))
                        continue;

                    current_student->level = level;
                    if (!readDigitInput(&class_id, 1, NUM_CLASSES, INSERT_CLASS_ID, "Invalid class id.\n"))
                        continue;
                    if (getUserAnswer())
                    {
                        printf("Enter the grades of the student in subjects:\n");
                        for (int i = 0; i < NUM_CLASSES; i++) {
                            printf("%s: ", S.subjects[i].name);
                            readDigitInput(&current_student->courses[i].grade, 0, 100, INSERT_GRADE, "Invalid grade.\n");
                        }
                    }
                    return;
                }
                current_student = current_student->next;
            }
        }
    }
    printf(STUDENT_NOT_FOUND);
}

bool fNameCompare(char* fName1, char* fName2) {
    return strcmp(fName1, fName2) == 0;
}

bool lNameCompare(char* lName1, char* lName2) {
    return strcmp(lName1, lName2) == 0;
}

void searchStudentByFnameByLName() {
    char fName[MAX_LEN], lName[MAX_LEN];
    readFirstNameLastName(fName, lName);

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                if (fNameCompare(current_student->fName, fName) && lNameCompare(current_student->lName, lName)) {
                    displayInfo(current_student, true);
                    return;
                }
                current_student = current_student->next;
            }
        }
    }

    printf(STUDENT_NOT_FOUND);
}

void freeMemory() {
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                struct student* temp = current_student;
                current_student = current_student->next;
                free(temp);
            }
        }
    }
}

void displayMenu() {
    char choice;
    while (1) {
        //clrscr();
        printf("Welcome to the Student Management System 2023\n");
        printf("1. Display all students\n");
        printf("2. Insert a new student\n");
        printf("3. Delete a student\n");
        printf("4. Update a student\n");
        printf("5. Search By first name and last name (display information)\n");
        printf("6. Exit\n");
        printf("Please enter your choice: 1 - 6\n");
        scanf(" %c", &choice);
        switch (choice) {
        case '1':
            displayStudents();
            break;
        case '2':
            insertStudent();
            break;
        case '3':
            deleteStudent();
            break;
        case '4':
            updateStudent();
            break;
        case '5':
            printf("Enter the first name and last name of the student to search for:\n");
            searchBy(fullNameMatch);
            break;
        case '6':
            freeMemory();
            exit(EXIT_SUCCESS);
        default:
            printf(INVALID_INPUT);
            break;
        }
    }
}

void displayInfo(struct student* student, bool full_info) {
    printf("Name: %s %s\n", student->fName, student->lName);

    if (full_info) {
        printf("Phone: %s\n", student->phone);
        printf("Level: %d\n", student->level);
        printf("Subjects:\n");
        for (int i = 0; i < NUM_COURSES; i++) {
            printf("%s: %d\n", S.subjects[i].name, student->courses[i].grade);
        }
    }
    printf(LINE_BREAK);
}
