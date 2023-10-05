#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LEN 50
#define NUM_COURSES 10
#define NUM_CLASSES 10
#define NUM_LEVELS 7
#define PHONE_LEN 11
#define MIN_GRADE 55
#define MIN_AVERAGE 60
#define DB_INPUT_FILE "students.txt"
#define ERROR_OPENING_FILE "Error opening file"
#define INVALID_INPUT "Invalid input, try again\n"
#define END_OF_RECORD '\0'
#define ALLOC_ERROR "Memory allocation error.\n"
#define LINE_BREAK "========================================\n"
#define INSERT_NEW_STUDENT "Enter the Student's details:\n"
#define INSERT_PHONE "Enter the phone number of the student(11 digits): "
#define INSERT_LEVEL "Enter the level of the student(1-7): "
#define INSERT_CLASS_ID "Enter the class of the student(1-10): "
#define INSERT_GRADE "Enter the grade: "
#define INSERT_FNAME "Enter the first name of the student: "
#define INSERT_LNAME "Enter the last name of the student: "
#define INSERTED_SUCCESSFULLY "Student inserted successfully.\n"
#define STUDENT_NOT_FOUND "Student not found.\n"
#define STUDENT_UPDATED "Student updated successfully.\n"
#define STUDENT_DELETED "Student deleted successfully.\n"
#define UPDATE "Do you want to update "
#define TOP_STUDENTS_LIMIT 10
#define STUDENTS_PER_PAGE 250
#define SAVE_FILE_NAME "DB.txt"
#define STUDENT_FOUND "Student found:\n"
#define DELETE_CONFIRMATION "Are you sure you want to delete this student? (y/n): "
#define INSERT_GRADES "Enter the grades of the student: \n"
#define SUBJETCS {"Mathematics", "English", "Cpp","Python", "Java", "C", "C#", "PHP", "HTML", "CSS"}
#define YES 'y'
#define NO 'n'
#define PRESS_TO_CONTINUE "Press Enter To Continue..."
#define ERROR_READING "Error while reading input"
#define ERROR_WRITING "Error while writing to file"
#define AVERAGE_PRE_LEVEL_MSG "Average per course per level:\n";
#define EXTRACTED_SUCCESSFULLY "Extracted successfully to %s\n"
#define DISPLAY_CANDIDATES "<::: Display candidates :::>\n";
#define TOP_STUDENTS_SECTION "<::: Top 10 students in a subject :::>\n"
#define DELETE_SECTION "<::: Delete a student :::>\n"
#define UPDATE_SECTION "<::: Update a student :::>\n"
"<::: Delete a student :::>\n");


#define MAIN "<::: Welcome to the Student Management System :::>\n"\
        "0. Display all students\n"\
        "1. Insert a new student\n"\
        "2. Delete a student\n"\
        "3. Update a student\n"\
        "4. Search By first name and last name (display information)\n"\
        "5. Display top 10 students in a subject\n"\
        "6. Display students candidates\n"\
        "7. Claculating the average per course per level\n"\
        "8. Extract as txt file\n"\
        "9. Exit\n"\
        "Please enter your choice: 0 - 9\n"

#define HASH_TABLE_SIZE 46

typedef struct GradeNode {
    struct student* student;
    struct GradeNode* next;
} GradeNode;


typedef struct HashMap {
    GradeNode* buckets[HASH_TABLE_SIZE];
} HashMap;

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
    struct student* prev;
};

struct subjectName {
    char name[MAX_LEN];
};

struct school {
    struct student* DB[NUM_LEVELS][NUM_CLASSES];
    int num_students;
};

const char* SUBJECTS_NAMES[NUM_COURSES] = { "Mathematics", "English", "Cpp","Python", "Java", "C", "C#", "PHP", "HTML", "CSS" };

char* ALLOCATED_INPUT = NULL;
size_t ALlOC_SIZE = 0;

static struct school S;
// For defining a function pointer
typedef bool (*MatchFunc)(struct student*, char*);

// ======================== Function Prototypes ========================

void init_db();
void clrscr();
FILE* open_file();
void freeMemory();
void displayMenu();
void extractFile();
void deleteStudent();
void updateStudent();
void insertStudent();
void getTopStudents();
void searchStudentBy();
void displayStudents();
void display_candidates();
bool is_user_confirmed();
void updateFirstName(struct student* student);
void updateLastName(struct student* student);
void updatePhoneNumber(struct student* student);
void updateLevel(struct student* student);
void updateGrades(struct student* student);
bool readInputWithConfirmation(char[] input, int max_len, char[] prompt, char[] success_msg, char[] error_msg);
void alloc_check(void* ptr);
void calculateLevelAverages();
void printError(const char* message);
double calculate_average(struct student* student);
bool fNameMatch(struct student* student, char fName[]);
bool lNameMatch(struct student* student, char lName[]);
void readFirstNameLastName(char fName[], char lName[]);
bool check_if_failed_in_course(struct student* student);
void displayInfo(struct student* student, bool full_info);
bool fullNameMatch(struct student* student, char fName[], char lName[]);
struct student* findByFullName(char fName[], char lName[], int* class_id);
bool readDigitInput(int var[], int min, int max, char msg[], char error_msg[]);
bool readInput(char var[], int max_len, int min_len, char msg[], char error_msg[]);

// ======================== Main Function ========================

int main() {
    init_db();
    displayMenu();
    atexit(freeMemory);
    return EXIT_SUCCESS;
}

// ======================== Function Implementations ========================
/*
    * This function is used to check if the user confirmed the action
    * @return true if the user confirmed the action, false otherwise
    */
bool is_user_confirmed() {
    char c;
    do {
        c = (char)getc(stdin);
        getc(stdin);
    } while (c != YES && c != NO);
    printf("confirm: %c\n", c);
    return c == YES;
}
// ===========================================================================
// This function is used to display the menu and get the user's choice
void displayMenu() {
    char choice;
    while (1) {
        printf(MAIN);
        choice = (char)getc(stdin);
        getc(stdin);
        switch (choice) {
        case '0':
            displayStudents();
            break;
        case '1':
            insertStudent();
            break;
        case '2':
            deleteStudent();
            break;
        case '3':
            updateStudent();
            break;
        case '4':
            searchStudentBy();
            break;
        case '5':
            getTopStudents();
            break;
        case '6':
            display_candidates();
            break;
        case '7':
            calculateLevelAverages();
            break;
        case '8':
            extractFile();
            break;
        case '9':
            exit(EXIT_SUCCESS);
        default:
            printf(INVALID_INPUT);
            break;
        }
        printf(PRESS_TO_CONTINUE);
        getc(stdin);
        clrscr();
    }
}
// ===========================================================================
// This function is used to search for a student by first name and last name
void searchStudentBy() {
    printf("<::: Search for a student :::>\n");
    char fName[MAX_LEN], lName[MAX_LEN];
    int class_id = 0;
    readFirstNameLastName(fName, lName);
    struct student* student = findByFullName(fName, lName, &class_id);
    if (student != NULL) {
        printf("%s ,lname %s", student->fName, student->lName);
        printf(STUDENT_FOUND);
        displayInfo(student, true);
        printf(LINE_BREAK);
    }
}
// ===========================================================================
// This function is used to insert a new student
void insertStudent() {
    printf("<::: ========== Insert a new student ========= :::>\n");
    int class_id;
    printf(INSERT_NEW_STUDENT);
    struct student* new_student = (struct student*)malloc(sizeof(struct student));
    alloc_check(new_student);
    readFirstNameLastName(new_student->fName, new_student->lName);
    while (!readInput(new_student->phone, PHONE_LEN, PHONE_LEN, INSERT_PHONE, INVALID_INPUT));
    while (!readDigitInput(&new_student->level, 1, NUM_LEVELS, INSERT_LEVEL, INVALID_INPUT));
    while (!readDigitInput(&class_id, 1, NUM_CLASSES, INSERT_CLASS_ID, INVALID_INPUT));
    // insert grades
    printf(INSERT_GRADES);
    for (int i = 0; i < NUM_COURSES; i++) {
        printf("%s: ", SUBJECTS_NAMES[i]);
        while (!readDigitInput(&new_student->courses[i].grade, 0, 100, INSERT_GRADE, INVALID_INPUT));
    }
    // insert student into the DB
    new_student->next = S.DB[new_student->level - 1][class_id - 1];
    new_student->prev = NULL;
    if (new_student->next != NULL) {
        new_student->next->prev = new_student;
    }
    S.DB[new_student->level - 1][class_id - 1] = new_student;
    S.num_students++;
    printf(INSERTED_SUCCESSFULLY);
}
// ===========================================================================
// This function is used to check if the memory allocation was successful
void alloc_check(void* ptr) {
    if (ptr == NULL) {
        printError(ALLOC_ERROR);
        exit(EXIT_FAILURE);
    }
}
// ===========================================================================
// This function is used to initialize the database(DB 2D array) from the input file
void init_db() {
    FILE* file = open_file("r", DB_INPUT_FILE);

    S.num_students = 0;
    int level, class_id, grades[NUM_COURSES];
    char fName[MAX_LEN], lName[MAX_LEN], phone[PHONE_LEN];
    // Initialize the DB to NULL
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            S.DB[level][class_id] = NULL;
        }
    }
    // Read the input file and insert the students into the DB
    while (fscanf(file, "%s %s %s %d %d", fName, lName, phone, &level, &class_id) == 5) {
        struct student* new_student = malloc(sizeof(struct student));
        if (new_student == NULL) {
            printError(ALLOC_ERROR);
            exit(EXIT_FAILURE);
        }
        fName[strcspn(fName, "\n")] = '\0';
        lName[strcspn(lName, "\n")] = '\0';
        strncpy(new_student->fName, fName, MAX_LEN);
        strncpy(new_student->lName, lName, MAX_LEN);
        strncpy(new_student->phone, phone, PHONE_LEN);
        new_student->level = level;
        // Read the grades
        for (int i = 0; i < NUM_CLASSES; i++) {
            if (fscanf(file, "%d", &grades[i]) != 1) {
                printError(INVALID_INPUT);
                exit(EXIT_FAILURE);
            }
            new_student->courses[i].subject_id = i;
            new_student->courses[i].grade = grades[i];
        }
        // Insert the new student into the linked list for the corresponding level and class_id
        new_student->next = NULL;
        new_student->prev = NULL;
        // Insert the new student into the linked list for the corresponding level and class_id
        if (S.DB[level - 1][class_id - 1] == NULL) {
            S.DB[level - 1][class_id - 1] = new_student;
        }
        else {
            struct student* current_student = S.DB[level - 1][class_id - 1];
            while (current_student->next != NULL) {
                current_student = current_student->next;
            }
            current_student->next = new_student;
            new_student->prev = current_student;
        }
        S.num_students++;
    }
    printf("num of students: %d\n", S.num_students);
    fclose(file);
}
// ===========================================================================
/*
    * This function is used to display the students in pages
    * @param count pointer to the count of students displayed
    * @return true if the user wants to continue to the next page, false otherwise
*/
bool isNextPage(int* count) {
    bool isContinue = true;
    if (*count == STUDENTS_PER_PAGE) {
        *count = 0;
        printf(PRESS_TO_CONTINUE);
        isContinue = (getc(stdin) == '\n');
    }
    return isContinue;
}
// This function for opening file for reading/writing
FILE* open_file(const char* mode, const char* file_name) {
    FILE* file = fopen(file_name, mode);

    if (file == NULL) {
        printf(ERROR_OPENING_FILE);
        exit(EXIT_FAILURE);
    }
    return file;
}

// This function for printing error message
void printError(const char* message) {
    fprintf(stderr, "%s", message);
}

void displayTotalStudents(int count) {
    printf("Total students: %d\n", count);
}
// This function for display all students info
void displayStudents() {
    int count = 0, total_students = 0;
    printf("=========== List of Students: ===========\n");
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                count++;
                printf("%d. ", total_students++);
                displayInfo(current_student, false);
                printf(LINE_BREAK);
                current_student = current_student->next;
                if (!isNextPage(&count))
                    return;
            }
        }
    }
    displayTotalStudents(total_students);
}
/***
 * This function for displaying the info of student
 * @param student the student to display
 * @param full_info boolean true-for shownig full info ,no-showing fname and lname
*/
void displayInfo(struct student* student, bool full_info) {
    printf("Name: %s %s\n", student->fName, student->lName);

    if (full_info) {
        printf("Phone: %s\n", student->phone);
        printf("Level: %d\n", student->level);
        printf("------- Subjects-------\n");
        for (int i = 0; i < NUM_COURSES; i++)
            printf("%d. %s: %d\n", i, SUBJECTS_NAMES[i], student->courses[i].grade);
    }
}

void clrscr() {
    system("@cls||clear");
}
// ===========================================================================

void freeMemory() {
    free(ALLOCATED_INPUT);
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
/*
    * This function for reading first name and last name
    * @param fName first name
    * @param lName last name
*/
void readFirstNameLastName(char fName[], char lName[]) {
    while (!readInput(fName, MAX_LEN, 1, INSERT_FNAME, INVALID_INPUT));
    while (!readInput(lName, MAX_LEN, 1, INSERT_LNAME, INVALID_INPUT));
}
// ===========================================================================
/*
    * This function for reading input from user
    * @param var the variable to store the input in
    * @param max_len the maximum length of the input
    * @param min_len the minimum length of the input
    * @param msg the message to display to the user when asking for input
    * @param error_msg the error message to display to the user when the input is invalid
    * @return true if the input is valid, false otherwise
*/
bool readInput(char var[], int max_len, int min_len, char msg[], char error_msg[]) {
    printf("%s", msg);

    char buffer[MAX_LEN];
    if (fgets(buffer, MAX_LEN, stdin) == NULL) {
        printError(ERROR_READING);
        exit(EXIT_FAILURE);
    }

    int new_line = strlen(buffer);
    if (new_line > 0 && buffer[new_line - 1] == '\n') {
        buffer[new_line - 1] = '\0';
        new_line--;
    }

    if (new_line < min_len || new_line > max_len) {
        printf("%s", error_msg);
        return false;
    }

    strncpy(var, buffer, new_line);
    var[new_line] = END_OF_RECORD;
    return true;
}
/*
    * This function for reading digit input from user
    * @param var the variable to store the input in
    * @param min the minimum value of the input
    * @param max the maximum value of the input
    * @param msg the message to display to the user when asking for input
    * @param error_msg the error message to display to the user when the input is invalid
    * @return true if the input is valid, false otherwise
*/
bool readDigitInput(int* var, int min, int max, char* msg, char* error_msg) {
    printf("%s", msg);
    if (scanf("%d", var) != 1 || *var < min || *var > max) {
        perror(error_msg);
        return false;
    }
    return true;
}

bool fullNameMatch(struct student* student, char fName[], char lName[]) {
    return fNameMatch(student, fName) && lNameMatch(student, lName);
}

bool fNameMatch(struct student* student, char fName[]) {
    return strcmp(student->fName, fName) == 0;
}

bool lNameMatch(struct student* student, char lName[]) {
    return strcmp(student->lName, lName) == 0;
}
// Ths function for extracting the DB to a file
void extractFile() {
    printf("Extracting...\n");
    FILE* file = open_file("w", SAVE_FILE_NAME);
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                if (fprintf(file, "%s %s %s %d %d ", current_student->fName, current_student->lName, current_student->phone, current_student->level, class_id + 1) < 0) {
                    printError(ERROR_WRITING);
                    exit(EXIT_FAILURE);
                }
                for (int i = 0; i < NUM_COURSES; i++) {
                    if (fprintf(file, "%d ", current_student->courses[i].grade) < 0) {
                        printError(ERROR_WRITING);
                        exit(EXIT_FAILURE);
                    }
                }
                if (fprintf(file, "\n") < 0) {
                    printError(ERROR_WRITING);
                    exit(EXIT_FAILURE);
                }
                current_student = current_student->next;
            }
        }
    }
    printf(EXTRACTED_SUCCESSFULLY, SAVE_FILE_NAME);
    fclose(file);
}
/*
    * This function for finding student by full name
    * @param fName first name
    * @param lName last name
    * @param class_id the class id of the student
    * @return the student if found, NULL otherwise
*/
struct student* findByFullName(char fName[], char lName[], int* class_id) {
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_index = 0; class_index < NUM_CLASSES; class_index++) {

            struct student* current_student = S.DB[level][class_index];
            while (current_student != NULL) {
                if (fullNameMatch(current_student, fName, lName)) {
                    *class_id = class_index;
                    return current_student;
                }
                current_student = current_student->next;
            }
        }
    }
    printf(STUDENT_NOT_FOUND);
    return NULL;
}
// This function for initializing the hashmap to NULL
void initHashMap(HashMap* hashMap) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashMap->buckets[i] = NULL;
    }
}

void deleteStudent() {
    printf(DELETE_SECTION);
    char fName[MAX_LEN], lName[MAX_LEN];
    int class_id = 0;
    readFirstNameLastName(fName, lName);
    struct student* student = findByFullName(fName, lName, &class_id);
    if (student == NULL)
        return;
    printf(STUDENT_FOUND);
    displayInfo(student, true);
    printf(LINE_BREAK);
    // ask user for confirmation
    printf(DELETE_CONFIRMATION);
    if (is_user_confirmed()) {
        if (student->prev == NULL) {
            S.DB[student->level - 1][class_id - 1] = student->next;
        }
        else {
            student->prev->next = student->next;
        }
        if (student->next != NULL) {
            student->next->prev = student->prev;
        }
        free(student);
        printf(STUDENT_DELETED);
    }
}

void updateStudent() {
    printf(UPDATE_SECTION);
    char fName[MAX_LEN], lName[MAX_LEN];
    int class_id = 0;
    readFirstNameLastName(fName, lName);
    struct student* student = findByFullName(fName, lName, &class_id);
    if (student == NULL)
        return;
    printf(STUDENT_FOUND);
    displayInfo(student, true);
    printf(LINE_BREAK);
    // ask user for confirmation and update the student
    printf("first name? (y/n): ");
    updateFirstName(student);
    updateLastName(student);
    updatePhoneNumber(student);
    updateLevel(student);
    updateGrades(student);

    printf(STUDENT_UPDATED);
}

void updateFirstName(struct student* student) {
    printf(UPDATE);
    printf("first name? (y/n): ");
    if (is_user_confirmed()) {
        while (!readInput(student->fName, MAX_LEN, 2, INSERT_LNAME, INVALID_INPUT));
    }
}

void updateLastName(struct student* student) {
    printf(UPDATE);
    printf("last name? (y/n): ");
    if (is_user_confirmed()) {
        while (!readInput(student->lName, MAX_LEN, 2, INSERT_LNAME, INVALID_INPUT));
    }
}

void updatePhoneNumber(struct student* student) {
    printf(UPDATE);
    printf("phone number? (y/n): ");
    if (is_user_confirmed()) {
        while (!readInput(student->phone, PHONE_LEN, PHONE_LEN, INSERT_PHONE, INVALID_INPUT));
    }
}

void updateLevel(struct student* student) {
    printf(UPDATE);
    printf("level? (y/n): ");
    if (is_user_confirmed()) {
        while (!readDigitInput(&student->level, 1, NUM_LEVELS, INSERT_LEVEL, INVALID_INPUT));
    }
}

void updateGrades(struct student* student) {
    printf(UPDATE);
    printf("grades? (y/n): ");
    if (is_user_confirmed()) {
        printf(INSERT_GRADES);
        for (int i = 0; i < NUM_COURSES; i++) {
            printf("%s: ", SUBJECTS_NAMES[i]);
            while (!readDigitInput(&student->courses[i].grade, 0, 100, INSERT_GRADE, INVALID_INPUT));
        }
    }
}
/* This function is reusable for reading input with confirmation
    * @param input the input to read
    * @param max_len the maximum length of the input
    * @param prompt the prompt to display to the user when asking for input
    * @param success_msg the message to display to the user when the input is valid
    * @param error_msg the error message to display to the user when the input is invalid
    * @return true if the input is valid, false otherwise
*/
bool readInputWithConfirmation(char[] input, int max_len, char[] prompt, char[] success_msg, char[] error_msg){
    printf("%s (y/n): ", prompt);
    if (is_user_confirmed()) {
        while (!readInput(input, max_len, 2, prompt, error_msg));
        printf("%s", success_msg);
        return true;
    }
    return false;
}


/*  This function for calculating the average per course per level
    * @return void
*/
double calculate_average(struct student* student) {
    double average = 0;
    for (int i = 0; i < NUM_COURSES; i++) {
        average += student->courses[i].grade;
    }
    return average / NUM_COURSES;
}
/*
    * This function for checking if the student has failed in any course
    * @param student the student to check
    * @return true if the student has failed in any course, false otherwise
*/
bool check_if_failed_in_course(struct student* student) {
    for (int i = 0; i < NUM_COURSES; i++)
        if (student->courses[i].grade < MIN_GRADE)
            return true;

    return false;
}

// Function to find candidates with average < 60 or grades < 55 
void display_candidates() {
    printf(DISPLAY_CANDIDATES);
    int count = 0;
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                double average = calculate_average(current_student);
                bool has_failed_course = check_if_failed_in_course(current_student);
                if (average < MIN_AVERAGE || has_failed_course) {
                    count++;
                    displayInfo(current_student, false);
                    printf("Average: %.2lf\n", average);
                    printf("Has failed course: %s\n", has_failed_course ? "Yes" : "No");
                    printf(LINE_BREAK);
                }
                current_student = current_student->next;
                if (!isNextPage(&count))
                    break;
            }
        }
    }
    printf(count == 0 ? "No candidates found.\n" : "Total candidates: %d\n", count);
}
// ===========================================================================
 /*
    * This function for creating a new node in the linked list of the same grade
    * @param student the student to insert
    * @return the new node
*/
GradeNode* createGradeNode(struct student* student) {
    GradeNode* newNode = (GradeNode*)malloc(sizeof(GradeNode));
    newNode->student = student;
    newNode->next = NULL;
    return newNode;
}

/* Function to insert a student into the linked list of the same grade
    * @param head the head of the linked list
    * @param student the student to insert
    * @return the new head of the linked list
*/
GradeNode* insertGradeNode(GradeNode* head, struct student* student) {
    GradeNode* newNode = createGradeNode(student);
    newNode->next = head;
    return newNode;
}

/* Function to retrieve the top 10 students
    * @param head the head of the linked list
    * @return void
*/
void getTopStudentsFromGradeList(GradeNode* head) {
    int count = 0;
    while (head != NULL && count < TOP_STUDENTS_LIMIT) {
        printf("%s %s - {Lvl: %d Grade: %d}\n", head->student->fName, head->student->lName, head->student->level, head->student->courses[0].grade);
        head = head->next;
        count++;
    }
}

/* Function to hash the grade to an index in the hashmap from 0 to 45 (46 indices)
 * The grade is hashed as follows:
 * 0 -> grade 55: in table index 0 represents grade 55
 * 45 -> grade 100: in table index 45 represents grade 100
 * @param grade the grade to hash
 * @return the index in the hashmap
*/
int hashGrade(int grade) {
    return grade - MIN_GRADE;
}

/*  Function to insert a student into the hashmap
    * @param hashMap the hashmap
    * @param student the student to insert
    * @param subject_id the subject id
    * @return void
*/
void insertStudentToHashMap(HashMap* hashMap, struct student* student, int subject_id) {
    int grade = student->courses[subject_id].grade;
    int index = hashGrade(grade);
    // Insert the student to the linked list at the corresponding index
    hashMap->buckets[index] = insertGradeNode(hashMap->buckets[index], student);
}

/* Function to get the top 10 students for a given level from the hashmap
    * @param hashMap the hashmap
    * @param level the level
    * @return void
*/
void getTopStudentsForLevel(HashMap* hashMap, int level) {
    int count = 0;
    for (int i = HASH_TABLE_SIZE - 1; i >= 0 && count < TOP_STUDENTS_LIMIT; i--) {
        GradeNode* gradeList = hashMap->buckets[i];

        while (gradeList != NULL) {
            if (gradeList->student->level == level) {
                printf("%s %s - {Lvl: %d Grade: %d}\n", gradeList->student->fName, gradeList->student->lName, gradeList->student->level, gradeList->student->courses[0].grade);
                count++;
                break;
            }
            gradeList = gradeList->next;
        }
    }
}

/* Function to get the top 10 students for a given subject for each level
    * 1. Create a hashmap of size 46 (0 - 45)
    * 2. Iterate over the DB and insert each student into the hashmap
    * 3. Iterate over the hashmap from the end and retrieve the top 10 students for each level
*/
void getTopStudents() {
    printf(TOP_STUDENTS_SECTION);
    int subject_id;
    printf("Subjects:\n");
    for (int i = 0; i < NUM_COURSES; i++) {
        printf("%d. %s\n", i, SUBJECTS_NAMES[i]);
    }
    while (!readDigitInput(&subject_id, 0, NUM_COURSES, "Enter the subject id: ", INVALID_INPUT));
    getc(stdin);
    printf("Top 10 students which didn't fail in any course(grade >= %d) in %s:\n", MIN_GRADE, SUBJECTS_NAMES[subject_id]);
    HashMap hashMap;
    initHashMap(&hashMap);

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL && current_student->courses[subject_id].grade >= MIN_GRADE) {
                insertStudentToHashMap(&hashMap, current_student, subject_id);
                current_student = current_student->next;
            }
        }
    }
    printf("Top 10 students in %s:\n", SUBJECTS_NAMES[subject_id]);
    for (int level = 1; level <= NUM_LEVELS; level++) {
        printf("Level %d:\n", level);
        getTopStudentsForLevel(&hashMap, level);
    }
    // Free the memory allocated for the hashmap
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        GradeNode* current = hashMap.buckets[i];
        while (current != NULL) {
            GradeNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
}
/**
 * Function to calculate the average per course per level
*/
void calculateLevelAverages() {
    int total_grades[NUM_LEVELS] = { 0 };
    int total_courses[NUM_LEVELS] = { 0 };

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = S.DB[level][class_id];
            while (current_student != NULL) {
                for (int i = 0; i < NUM_COURSES; i++) {
                    total_grades[level] += current_student->courses[i].grade;
                    total_courses[level]++;
                }
                current_student = current_student->next;
            }
        }
    }
    printf(AVERAGE_PRE_LEVEL_MSG);
    for (int level = 0; level < NUM_LEVELS; level++) {
        double average = (double)total_grades[level] / total_courses[level];
        printf("Level %d: %.2lf\n", level + 1, average);
    }
}