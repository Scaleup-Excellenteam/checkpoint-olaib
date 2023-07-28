void getTopStudentsForLevel(HashMap* hashMap, int level, struct student* top_students[]) {
    int index = hashLevel(level);

    // Initialize the top_students array
    for (int i = 0; i < TOP_STUDENTS_LIMIT; i++) {
        top_students[i] = NULL;
    }

    // Traverse the linked list at the corresponding index in the hash map
    GradeNode* current = hashMap->buckets[index];
    while (current != NULL) {
        struct student* student = current->student;

        // Check if the student's level matches the requested level
        if (student->level == level) {
            // Find the index to insert the student in the top_students array
            int insert_index = 0;
            while (insert_index < TOP_STUDENTS_LIMIT&& top_students[insert_index] != NULL &&
                top_students[insert_index]->courses[0].grade >= student->courses[0].grade) {
                insert_index++;
            }

            // Shift elements in the top_students array to make space for the new student
            for (int i = TOP_STUDENTS_LIMIT - 1; i > insert_index; i--) {
                top_students[i] = top_students[i - 1];
            }

            // Insert the student into the top_students array
            top_students[insert_index] = student;
        }

        current = current->next;
    }
}

// Function to print the top 10 students for a given level
void printTopStudentsForLevel(int level, struct student* top_students[]) {
    printf("Top 10 students for Level %d:\n", level);
    for (int i = 0; i < TOP_STUDENTS_LIMIT; i++) {
        if (top_students[i] != NULL) {
            printf("%d. ", i + 1);
            displayInfo(top_students[i], false);
        }
    }
    printf("\n");
}

// Function to print the top 10 students for all levels
void printTopStudentsByLevel(HashMap* hashMap) {
    for (int level = 1; level <= NUM_LEVELS; level++) {
        struct student* top_students[TOP_STUDENTS_LIMIT];
        getTopStudentsForLevel(hashMap, level, top_students);
        printTopStudentsForLevel(level, top_students);
    }
}