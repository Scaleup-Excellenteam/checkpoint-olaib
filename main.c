#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LEN 50
#define NUM_COURSES 20
#define NUM_CLASSES 10
#define NUM_LEVELS 7
#define PHONE_LEN 11
#define DB_INPUT_FILE "students.txt"
#define ERROR_OPENING_FILE "Error opening file"
#define INVALID_INPUT "Invalid input, try again\n"
#define ALLOC_ERROR "Memory allocation error.\n"
#define MENU "Welcome to the Student Management System 2023\n"\
             "1. Display all students\n"\
             "2. Insert a new student\n"\
             "3. Delete a student\n"\
             "4. Update a student\n"\
             "5. Search By first name and last name (display information)\n"\
             "6. Exit\n"\
             "Please enter your choice: 1 - 6\n"
void clrscr();
void updateStudent();
void deleteStudent();
void insertStudent();
void displayStudents();
void initDb();
void displayMenu();
void searchStudentByFnameByLName();
void freeMemory();


struct course {
    int subject_id; // The subject ID is used as an index in the subject_map
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

struct subject_map {
    char name[MAX_LEN];
};

struct school {
    struct student* DB[NUM_LEVELS][NUM_CLASSES];
    int num_students;
    struct subject_map subjects[NUM_COURSES];
};

static struct school S;

int main() {
    initDb();
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

    int level, class_id;
    char fName[MAX_LEN], lName[MAX_LEN], phone[PHONE_LEN];
    int grades[NUM_CLASSES];

    while (fscanf(file, "%s %s %s %d %d", fName, lName, phone, &level, &class_id) == 5) {
        struct student* new_student = malloc(sizeof(struct student));
        if (new_student == NULL) {
            printf("Memory allocation error.\n");
            return;
        }

        strcpy(new_student->fName, fName);
        strcpy(new_student->lName, lName);
        strcpy(new_student->phone, phone);
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

        new_student->next = S.DB[level - 1][class_id - 1];
        S.DB[level - 1][class_id - 1] = new_student;
        S.num_students++;
    }

    // Initializing the subject_map 
    const char* subjectNames[NUM_COURSES] = {
        "Mathematics", "English", "Science", "History", "Internet Programming",
        "Physics", "Chemistry", "Biology", "Geography", "Literature",
        "Physical Education", "Art", "Music","Economics", "Python",
        "Foreign Language", "Health", "Cpp", "Psychology", "Sociology"
    };


    struct subject_map* current_subject = S.subjects;

    fclose(file);
}

void displaySubjects(struct student* student) {
    printf("Subjects:\n");
    for (int i = 0; i < NUM_COURSES; i++) {
        int subject_id = student->courses[i].subject_id;
        printf("%s: %d\n", S.subjects[subject_id].name, student->courses[i].grade);
    }
}

void displayStudents() {
    printf("=========== List of Students: ===========\n");
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                printf("First Name: %s, Last Name: %s\n",
                    current_student->fName, current_student->lName);
                printf("Phone: %s\n", current_student->phone);
                printf("Level: %d\n", current_student->level);
                displaySubjects(current_student);
                printf("\n");
                current_student = current_student->next;
            }
        }
    }
    printf("=========================================\n");
}

void insertStudent() {
    char fName[MAX_LEN], lName[MAX_LEN], phone[PHONE_LEN];
    int level, class_id;
    int grades[NUM_CLASSES];

    printf("Enter the first name of the student: ");
    scanf("%s", fName);
    printf("Enter the last name of the student: ");
    scanf("%s", lName);
    printf("Enter the phone number of the student: ");
    scanf("%s", phone);
    printf("Enter the level of the student: ");
    scanf("%d", &level);
    printf("Enter the class id of the student: ");
    scanf("%d", &class_id);

    struct student* new_student = malloc(sizeof(struct student));
    if (new_student == NULL) {
        printf(ALLOC_ERROR);
        return;
    }

    strcpy(new_student->fName, fName);
    strcpy(new_student->lName, lName);
    strcpy(new_student->phone, phone);
    new_student->level = level;

    for (int i = 0; i < NUM_CLASSES; i++) {
        printf("Enter the grade of the student in subject %d: ", i + 1);
        scanf("%d", &grades[i]);
        new_student->courses[i].subject_id = i;
        new_student->courses[i].grade = grades[i];
    }

    new_student->next = S.DB[level - 1][class_id - 1];
    S.DB[level - 1][class_id - 1] = new_student;
    S.num_students++;
}

void deleteStudent() {
    char fName[MAX_LEN], lName[MAX_LEN];
    printf("Enter the first name of the student: ");
    scanf("%s", fName);
    printf("Enter the last name of the student: ");
    scanf("%s", lName);

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            struct student* prev_student = NULL;
            while (current_student != NULL) {
                if (strcmp(current_student->fName, fName) == 0 &&
                    strcmp(current_student->lName, lName) == 0) {
                    if (prev_student != NULL)
                        prev_student->next = current_student->next;
                    else
                        S.DB[level][class_id] = current_student->next;

                    free(current_student);
                    S.num_students--;
                    return;
                }
                prev_student = current_student;
                current_student = current_student->next;
            }
        }
    }
    printf("Student not found.\n");
}

void updateStudent() {
    char fName[MAX_LEN], lName[MAX_LEN];
    printf("Enter the first name of the student: ");
    scanf("%s", fName);
    printf("Enter the last name of the student: ");
    scanf("%s", lName);

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                if (strcmp(current_student->fName, fName) == 0 &&
                    strcmp(current_student->lName, lName) == 0) {
                    printf("Enter the new first name of the student: ");
                    scanf("%s", current_student->fName);
                    printf("Enter the new last name of the student: ");
                    scanf("%s", current_student->lName);
                    printf("Enter the new phone number of the student: ");
                    scanf("%s", current_student->phone);
                    printf("Enter the new level of the student: ");
                    scanf("%d", &current_student->level);

                    for (int i = 0; i < NUM_CLASSES; i++) {
                        printf("Enter the new grade of the student in subject %d: ", i + 1);
                        scanf("%d", &current_student->courses[i].grade);
                    }
                    return;
                }
                current_student = current_student->next;
            }
        }
    }
    printf("Student not found.\n");
}

void searchStudentByFnameByLName() {
    char fName[MAX_LEN], lName[MAX_LEN];
    printf("Enter the first name of the student: ");
    scanf("%s", fName);
    printf("Enter the last name of the student: ");
    scanf("%s", lName);

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                if (strcmp(current_student->fName, fName) == 0 &&
                    strcmp(current_student->lName, lName) == 0) {
                    printf("First Name: %s, Last Name: %s\n",
                        current_student->fName, current_student->lName);
                    printf("Phone: %s\n", current_student->phone);
                    printf("Level: %d\n", current_student->level);
                    displaySubjects(current_student);
                    printf("\n");
                    return;
                }
                current_student = current_student->next;
            }
        }
    }
    printf("Student not found.\n");
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
    while (true) {
        //clrscr();
        printf(MENU);
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
            searchStudentByFnameByLName();
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