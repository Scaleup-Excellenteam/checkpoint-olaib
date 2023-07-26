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
#define INVALID_INPUT "Invalid input - level number must be between 1 and 10"
int subject_index; // Global variable to store the subject index

struct student {
    char fName[MAX_LEN];
    char lName[MAX_LEN];
    char phone[PHONE_LEN];
    char* courses[NUM_COURSES];
    int grades[NUM_COURSES];
    int level;
};

struct school {
    struct student DB[NUM_LEVELS][NUM_CLASSES];
    int num_students;
};

static struct school S;

void init_DB(struct school* sch) {
    FILE* file = fopen(DB_INPUT_FILE, "r");
    if (file == NULL) {
        printf(ERROR_OPENING_FILE);
        return;
    }

    int level, class_id;
    char fName[MAX_LEN], lName[MAX_LEN], phone[PHONE_LEN];
    int grades[NUM_CLASSES];

    while (fscanf(file, "%s %s %s %d %d", fName, lName, phone, &level, &class_id) == 5) {
        struct student new_student;
        strcpy(new_student.fName, fName);
        strcpy(new_student.lName, lName);
        strcpy(new_student.phone, phone);

        for (int i = 0; i < NUM_CLASSES; i++) {
            fscanf(file, "%d", &grades[i]);
            new_student.grades[i] = grades[i];
        }
        new_student.level = level;

        S.DB[level - 1][class_id - 1] = new_student;
        S.num_students++;
    }

    fclose(file);
}

void display_students(struct school* sch) {
    printf("List of Students:\n");
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = &sch->DB[level][class_id];
            if (current_student->courses[0] != NULL) {
                printf("Name: %s %s, Phone: %s, Level: %d, Class: %d\n",
                    current_student->fName, current_student->lName,
                    current_student->phone, level + 1, class_id + 1);
            }
        }
    }
}

void add_student(struct school* sch) {
    char fName[MAX_LEN], lName[MAX_LEN], phone[PHONE_LEN];
    int level, class_id;

    printf("Enter first name: ");
    scanf("%s", fName);

    printf("Enter last name: ");
    scanf("%s", lName);

    printf("Enter phone number: ");
    scanf("%s", phone);

    printf("Enter level (1-10): ");
    scanf("%d", &level);
    if (level < 1 || level > NUM_LEVELS) {
        printf(INVALID_INPUT);
        return;
    }

    printf("Enter class (1-20): ");
    scanf("%d", &class_id);
    if (class_id < 1 || class_id > NUM_CLASSES) {
        printf(INVALID_INPUT);
        return;
    }

    struct student new_student;
    strcpy(new_student.fName, fName);
    strcpy(new_student.lName, lName);
    strcpy(new_student.phone, phone);

    // Initializing courses array to NULL
    for (int i = 0; i < NUM_COURSES; i++) {
        new_student.courses[i] = NULL;
    }

    S.DB[level - 1][class_id - 1] = new_student;
    S.num_students++;
    printf("Student added successfully!\n");
}


int compare_grades(const void* a, const void* b) {
    const struct student* studentA = (const struct student*)a;
    const struct student* studentB = (const struct student*)b;
    return studentB->grades[subject_index] - studentA->grades[subject_index];
}

void get_top_students(int subject_idx) {
    subject_index = subject_idx;

    struct student all_students[NUM_LEVELS * NUM_CLASSES];
    int num_students = 0;

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int class_id = 0; class_id < NUM_CLASSES; class_id++) {
            struct student* current_student = &S.DB[level][class_id];
            if (current_student->courses[0] != NULL) {
                all_students[num_students++] = *current_student;
            }
        }
    }

    // Sort the students based on grades in the specific subject
    qsort(all_students, num_students, sizeof(struct student), compare_grades);

    int grade_counter[NUM_LEVELS] = { 0 };
    printf("Top Ten Students in Each Grade for Subject %d:\n", subject_index + 1);
    for (int i = 0; i < num_students; i++) {
        int level = all_students[i].level - 1;
        if (grade_counter[level] < 10) {
            printf("%s %s - Grade: %d\n", all_students[i].fName, all_students[i].lName,
                all_students[i].grades[subject_index]);
            grade_counter[level]++;
        }
    }
}
void get_candidates_for_departure() {
}

void calculate_average_grades() {
}

void encrypt_decrypt_data(bool encrypt) {
}

void display_menu() {
    printf("\nMain Menu\n");
    printf("1. Admission of a new student\n");
    printf("2. Query the top ten students in each grade for a particular subject\n");
    printf("3. Query students who are candidates for departure\n");
    printf("4. Calculation of average per course per layer\n");
    printf("5. Encrypt student information\n");
    printf("6. Decrypt student information\n");
    printf("7. Save data to file (optional)\n");
    printf("8. Exit the program\n");
}

int main() {
    struct school sch = { 0 };

    // Load data from the file to the DB
    init_DB(&sch);

    // Display all students
    display_students(&sch);

    int choice;
    do {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            add_student(&sch);
            break;
        case 2: {
            int subject_index;
            printf("Enter the subject index (0-%d)", NUM_COURSES - 1);
            scanf("%d", &subject_index);
            get_top_students(subject_index);
            break;
        }
        case 3:
            get_candidates_for_departure();
            break;
        case 4:
            calculate_average_grades();
            break;
        case 5:
            encrypt_decrypt_data(true); // Encrypt data
            break;
        case 6:
            encrypt_decrypt_data(false); // Decrypt data
            break;
        case 7:
            // Save data to the file
            break;
        case 8:
            printf("Exiting the program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}
