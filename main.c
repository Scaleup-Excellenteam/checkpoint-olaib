#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LEN 100
#define NUM_CLASSES 12
#define NUM_LEVELS 12
#define DB_INPUT_FILE "students.txt"


struct student {
    char fName[MAX_LEN];
    char lName[MAX_LEN];
    char phone[MAX_LEN];
    int classes[NUM_CLASSES];
};

struct school {
    struct student DB[NUM_LEVELS][NUM_CLASSES];
};

static struct school S;

void init_db() {
    FILE* file = fopen(DB_INPUT_FILE, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int classNum = 0; classNum < NUM_CLASSES; classNum++) {
            struct student* currentStudent = &S.DB[level][classNum];

            // Read data for each student from the file
            if (fscanf(file, "%s %s %s", currentStudent->fName, currentStudent->lName, currentStudent->phone) == 3) {
                for (int i = 0; i < NUM_CLASSES; i++) {
                    fscanf(file, "%d", &currentStudent->classes[i]);
                }
            }
        }
    }

    fclose(file);
}

void print_db() {
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int classNum = 0; classNum < NUM_CLASSES; classNum++) {
            struct student* currentStudent = &S.DB[level][classNum];
            if (strlen(currentStudent->fName) > 0) {
                printf("Student Info:\n");
                printf("First Name: %s\n", currentStudent->fName);
                printf("Last Name: %s\n", currentStudent->lName);
                printf("Phone: %s\n", currentStudent->phone);

                printf("Classes:\n");
                for (int i = 0; i < NUM_CLASSES; i++) {
                    printf("Level %d, Class %d: %d\n", i + 1, classNum + 1, currentStudent->classes[i]);
                }
                printf("\n");
            }
        }
    }
}

void pupulate_db(){
     FILE* file = fopen("database.txt", "w");
    if (file == NULL) {
        perror("Error creating database file");
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"students\": [\n");

    bool firstStudent = true;
    for (int level = 0; level < NUM_LEVELS; level++) {
        for (int classNum = 0; classNum < NUM_CLASSES; classNum++) {
            struct student* currentStudent = &S.DB[level][classNum];
            if (strlen(currentStudent->fName) > 0) {
                if (!firstStudent) {
                    fprintf(file, ",\n");
                }
                fprintf(file, "    {\n");
                fprintf(file, "      \"first_name\": \"%s\",\n", currentStudent->fName);
                fprintf(file, "      \"last_name\": \"%s\",\n", currentStudent->lName);
                fprintf(file, "      \"phone\": \"%s\",\n", currentStudent->phone);
                fprintf(file, "      \"classes\": [");
                for (int i = 0; i < NUM_CLASSES; i++) {
                    fprintf(file, "%d", currentStudent->classes[i]);
                    if (i != NUM_CLASSES - 1) {
                        fprintf(file, ", ");
                    }
                }
                fprintf(file, "]\n");
                fprintf(file, "    }");

                firstStudent = false;
            }
        }
    }

    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");

    fclose(file);
}

int main() {
    init_db();
    print_db();
    pupulate_db();


    return 0;
}
