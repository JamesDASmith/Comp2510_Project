#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// INFORMATION
/**
 * The following program was designed to provide the fundamental groundwork for a hospital management system, allowing
 * users to provide basic services such as building a schedule for doctors, creating patients with unique information
 * and managing patients information, and discharging when needed.
 *
 * The program is controlled through a series of text based menus, using user inputs to navigate between the menu and
 * the program's various functions.
 *
 * @author James Smith
 * @author Leslie Zhang
 * @version 1.0
 */

// METHOD DECLARATIONS
int     idExists();
void    menu();
void    printPatient();
void    addPatient();
void    viewPatients();
void    searchPatients();
void    organizePatientList();
void    dischargePatient();
void    manageDrSchedule();
void    toLowerString();
int     read_from_patient_file_ll();
int     read_from_schedule_file();
void    write_to_patient_file_ll();
void    write_to_schedule_file();
void    create_node();
void    delete_node();
void    insert_at_beginning();
void    insert_at_end();
void    view_patient_ll();
struct Node*    search_patient_by_name_ll();
struct Node*    search_patient_by_id_ll();
void    print_patient_ll();


// VARIABLE DECLARATIONS
int totalPatients = 0;
int max_patients = 50;
const int DAYS_OF_WEEK = 7;
const int SHIFTS_PER_DAY = 3;
const int MONDAY = 1;
const int TUESDAY = 2;
const int WEDNESDAY = 3;
const int THURSDAY = 4;
const int FRIDAY = 5;
const int SATURDAY = 6;
const int SUNDAY = 7;
const int DELETE = 666;

char schedule[7][3][50];

const char patient_file_path[] = "patients.txt";
const char patientII_file_path[] = "patients_II.txt";
const char schedule_file_path[] = "schedule.txt";

//struct representing each patient
struct Patient {
    int patient_id;
    char name[50];
    int age;
    char diagnosis[50];
    int room_number;
};

//each node
struct Node {
    struct Patient patient;
    struct Node *next;
};

struct Node *head = NULL;

/**
 * create a new node
 * @param patient
 * @return
 */
struct Node* createNode(struct Patient patient) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->patient = patient;
    newNode->next = NULL;
    return newNode;
}

/**
 * insert node at beginning of linked list
 * @param head
 * @param patient
 */
void insert_at_beginning(struct Node** head, struct Patient patient, int *totalPatients) {
    struct Node* newNode = createNode(patient);
    newNode->next = *head;
    *head = newNode;
    (*totalPatients)++;
}

/**
 * Insert node at the end of linked list
 * @param head
 * @param patient
 */
void insert_at_end(struct Node** head, struct Patient patient, int *totalPatients) {
    struct Node* newNode = createNode(patient);

    if (*head == NULL) {
        *head = newNode;
        (*totalPatients)++;
        return;
    }

    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    (*totalPatients)++;
}

/**
 * delete node from linked list
 * @param head
 * @param target
 * @param totalPatients
 */
void delete_node(struct Node** head, struct Node* target, int* totalPatients) {
    if (head == NULL || *head == NULL || target == NULL) {
        printf("Invalid operation. Nothing to delete.\n");
        return;
    }

    struct Node* current = *head;
    struct Node* prev = NULL;

    // Traverse to find the target node
    while (current != NULL && current != target) {
        prev = current;
        current = current->next;
    }

    // If target not found
    if (current == NULL) {
        printf("Patient not found\n");
        return;
    }

    // If node to delete is the head
    if (prev == NULL) {
        *head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    (*totalPatients)--;

    printf("Patient discharged successfully.\n");
}

// MAIN METHOD
/**
 * Main method. Beginning of the flow of control.
 *
 * @return int 0 if program has terminated successfully.
 */
int main(void)
{
    if (read_from_patient_file_ll(&head, &totalPatients) == 0) {
        printf("Starting with empty patient database.\n");
    }
    read_from_schedule_file(&schedule);
    menu();
    return 0;
}

// PRIMARY METHODS
/**
 * Represents the main menu from which a user can choose to use any of the programs functions.
 */
void menu() {
    int choice;
    do {
        printf("\nHospital Management System\n");
        printf("1. Add Patient Record\n");
        printf("2. View All Patients\n");
        printf("3. Search Patient\n");
        printf("4. Discharge Patient\n");
        printf("5. Manage Doctor Schedule\n");
        printf("6. Exit\n"
               "Enter your choice: \n");

        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1:
                addPatient();
            break;
            case 2:
                view_patient_ll();              //view from linked list
            break;
            case 3:
                searchPatients();
            break;
            case 4:
                dischargePatient();
            break;
            case 5:
                manageDrSchedule();
            case 6:
                printf("Exiting...\n");
                write_to_patient_file_ll(head);
            break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);
    printf("Thanks for visiting!");
}


/**
 * Creates a patient, prompting a user for input.
 */
void addPatient() {
    int id;
    char name[50];
    int age;
    char diagnosis[50];
    int room_number;
    int status = 0;

    struct Patient patient = {};

    // Ensures patient ID is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Patient ID: \n");
        scanf("%d", &id);
        getchar();

        if (id <= 0 || idExists(id) == 1) {
            printf("Invalid or duplicate ID\n");
            printf("Re-Enter\n");
        }else {
            patient.patient_id = id;
            status = 1;
        }
    }while (status == 0);
    status = 0;

    // Ensures patient Name is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Patient Name: \n");
        fgets(patient.name, 50, stdin);
        patient.name[strcspn(patient.name, "\n")] = 0;
        if (strlen(patient.name) == 0) {
            printf("Name cannot be empty.\n");
            printf("Re-Enter\n");
        }else {
            status = 1;
        }
    }while (status == 0);
    toLowerString(patient.name);
    status = 0;

    // Ensures patient Age is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Age: \n");
        scanf("%d", &age);
        if (age <= 0) {
            printf("Invalid age!\n");
            printf("Re-Enter\n");
        }else {
            patient.age = age;
            status = 1;
        }
        getchar();
    }while (status == 0);
    status = 0;

    // Ensures patient Diagnosis is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Diagnosis: \n");
        fgets(patient.diagnosis, 50, stdin);
        patient.diagnosis[strcspn(patient.diagnosis, "\n")] = 0;
        if (strlen(patient.diagnosis) == 0) {
            printf("Diagnosis cannot be empty\n");
            printf("Re-Enter.\n");
        }else {
            status = 1;
        }
    }while (status == 0);
    status = 0;

    // Ensures patient Room Number is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Room Number: \n");
        scanf("%d", &room_number);
        if (room_number < 0) {
            printf("Invalid room number.\n");
            printf("Re-Enter.\n");
        }else {
            patient.room_number = room_number;
            status = 1;
        }
    }while (status == 0);

    //with linked list
    insert_at_end(&head, patient, &totalPatients);

    printf("Patient Added Successfully\n");
}

/**
 * print patient list with linkedlist
 */
void view_patient_ll() {
    printf("Printing Patient Information...\n");
    struct Node* current = head;
    int i = 0;

    while (current != NULL) {
        printf("--------------------------\n");
        printf("Patient #%d\n", i + 1);
        printf("Patient ID: %d\n", current->patient.patient_id);
        printf("Name: %s\n", current->patient.name);
        printf("Age: %d\n", current->patient.age);
        printf("Diagnosis: %s\n", current->patient.diagnosis);
        printf("Room Number: %d\n", current->patient.room_number);
        current = current->next;
        i++;
    }

    if (i == 0) {
        printf("No patients in the system.\n");
    }

    printf("--------------------------\n");
    printf("End of List\n");
    printf("--------------------------\n");
}
/**
 * Allows user to search for a patient by either their ID or by Name, printing their information if found.
 */
void searchPatients() {
    int selection;
    int id;
    int index;
    char name[50];

    printf("1. Search Patient By ID\n");
    printf("2. Search Patient By Name\n");
    scanf("%d", &selection);
    getchar();

    switch (selection) {
        case 1:
            printf("Enter Patient ID:");
            scanf("%d", &id);
            //with linked list
            struct Node* patient = search_patient_by_id_ll(id);
            if (patient == NULL) {
                printf("Patient not found\n");
            }else {
                printf("--------------------------\n");
                print_patient_ll(patient);
                printf("--------------------------\n");
            }
            break;
        case 2:
            printf("Enter Patient Name:");
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = 0;
            //with linked list
            struct Node* p = search_patient_by_name_ll(name);
            if (p == NULL) {
                printf("Patient not found\n");
            }else {
                printf("--------------------------\n");
                print_patient_ll(p);
                printf("--------------------------\n");
            }
            break;
        default:
            printf("Invalid Input. Terminating...\n");
        break;
    }

}

/**
 * search patient by name with a linked list
 * @param name
 * @param head
 * @param totalPatients
 * @return
 */
struct Node* search_patient_by_name_ll(char name[]) {
    if (strlen(name) == 0) {
        printf("Name cannot be empty.\n");
        return NULL;
    }

    toLowerString(name);
    struct Node* current = head;
    int index = 0;

    while (current != NULL) {
        char patientNameLower[50];
        strcpy(patientNameLower, current->patient.name);
        toLowerString(patientNameLower);

        if (strcmp(name, patientNameLower) == 0) {
            return current;
        }

        current = current->next;
        index++;
    }

    return NULL; // not found
}


/**
 * return address of the node, of which the patient id matches
 * @param id
 * @param head
 * @return
 */
struct Node* search_patient_by_id_ll(int id) {
    struct Node* current = head;
    while (current != NULL) {
        if (current->patient.patient_id == id) {
            return current;
        }
        current = current->next;
    }

    return NULL; // Not found
}

/**
 * Prompts the user for information about a patient (either name or ID) and removes them from the patient list.
 */
void dischargePatient() {
    int choice;
    int id;
    char name[50];
    int index;
    struct Patient def = {};

    // Menu
    printf("Discharge Patient.\n");
    printf("1. Discharge By ID.\n");
    printf("2. Discharge By Name.\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: // Checks Patient ID, removing them if found.
            printf("Enter ID:\n");
            scanf("%d", &id);
            struct Node* patient = search_patient_by_id_ll(id);
            if (patient == NULL) {
                printf("Patient not found\n");
            }else {
                printf("--------------------------\n");
                delete_node(&head, patient, &totalPatients);
                printf("--------------------------\n");
            }
            break;

        case 2: // Checks Patient Name, removing them if found.
            printf("Enter Name: \n");
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = 0;
            toLowerString(name);
            struct Node* p = search_patient_by_name_ll(name);
            if (p == NULL) {
                printf("Patient not found\n");
            }else {
                printf("--------------------------\n");
                delete_node(&head, p, &totalPatients);
                printf("--------------------------\n");
            }
            break;
        default:
            printf("Invalid Input.");
            break;
    }

}

/**
 * Prompts users to various options to either add a doctor to the schedule, or view schedule
 */
void manageDrSchedule() {
    int choice;

    // Menu
    printf("What would you like to do?\n");
    printf("1. Add Doctor to Schedule\n");
    printf("2. View Schedule\n");

    scanf("%d", &choice);
    getchar(); // Remove newline

    switch (choice) {
        case 1: // User chooses to add a doctor to the schedule.
            char name[50];
            int shiftDay;
            int shiftTime;
            int status = 0;

            // User inputs Doctor Name
            do {
                printf("Enter Doctor Name:\n");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                if (strlen(name) == 0) {
                    printf("Name cannot be empty.\n");
                    printf("Re-Enter\n");
                } else {
                    status = 1;
                }
            } while (status != 1);

            // User chooses day of said doctor's shift
            printf("Enter Shift Day:\n");
            printf("1. Monday\n");
            printf("2. Tuesday\n");
            printf("3. Wednesday\n");
            printf("4. Thursday\n");
            printf("5. Friday\n");
            printf("6. Saturday\n");
            printf("7. Sunday\n");
            scanf("%d", &shiftDay);
            getchar();

            if(shiftDay > 7 || shiftDay < 1) {
                printf("Invalid Input. Terminating...\n");
                break;
            }

            // User chooses time of said doctor's shift
            printf("Enter Shift Time:\n");
            printf("1. Morning\n");
            printf("2. Evening\n");
            printf("3. Night\n");
            scanf("%d", &shiftTime);
            getchar();

            if(shiftTime > 3 || shiftTime < 1) {
                printf("Invalid Input. Terminating...\n");
                break;
        }
            strcpy(schedule[shiftDay - 1][shiftTime - 1], name);
            write_to_schedule_file(schedule);
            break;

        case 2: // User chooses to view doctor schedule.
            // Prints the Header for the Calendar
            printf("Shift Calendar\n");
            printf("\t\t\tMorning:\tEvening:\tNight:\n");

            // Prints the Day of the Week before each row
            for (int i = 1; i <= DAYS_OF_WEEK; i++) {

                if (i == 1) {
                    printf("Monday:\t");
                }else if (i == 2) {
                    printf("Tuesday:");
                }else if (i == 3) {
                    printf("Wednesday:");
                }else if (i == 4) {
                    printf("Thursday:");
                }else if (i == 5) {
                    printf("Friday:\t");
                }else if (i == 6) {
                    printf("Saturday:");
                }else if (i == 7) {
                    printf("Sunday:\t");
                }else {
                    printf("Day of Week Error.");
                }

                // Prints the value stored in the current array unit where doctor is store.
                for (int j = 1; j <= SHIFTS_PER_DAY; j++) {
                        printf("\t\t%s", schedule[i - 1][j - 1]);
                }

                printf("\n");
            }
        break;

        default:
            printf("Invalid Input:\n");
    }
}


// SUPPORT METHODS
int idExists(int id) {
    struct Node* current = head;
    while (current != NULL) {
        if (current->patient.patient_id == id) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}
/**
 * print patient from linked list
 * @param patient
 */
void print_patient_ll(struct Node *patient) {
    printf("Patient ID: %d\n", patient->patient.patient_id);
    printf("Name: %s\n", patient->patient.name);
    printf("Age: %d\n", patient->patient.age);
    printf("Diagnosis: %s\n", patient->patient.diagnosis);
    printf("Room Number: %d\n", patient->patient.room_number);
}

/**
 * utility function to turn all strings to lowercase
 * @param str pointer to string
 */
void toLowerString(char *str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

/**
 * read from patient file
 * @param head
 * @param totalPatients
 * @return
 */
int read_from_patient_file_ll(struct Node **head, int *totalPatients) {
    *totalPatients = 0;
    FILE *file = fopen(patientII_file_path, "r");
    if (file == NULL) {
        printf("File not found.\n");
        return 0;
    }

    char buffer[200];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;

        struct Patient p = {};
        char *token;

        token = strtok(buffer, ",");
        if (token) p.patient_id = atoi(token);

        token = strtok(NULL, ",");
        if (token) strncpy(p.name, token, sizeof(p.name) - 1);

        token = strtok(NULL, ",");
        if (token) p.age = atoi(token);

        token = strtok(NULL, ",");
        if (token) strncpy(p.diagnosis, token, sizeof(p.diagnosis) - 1);

        token = strtok(NULL, ",");
        if (token) p.room_number = atoi(token);

        insert_at_end(head, p, totalPatients);
    }

    fclose(file);
    return 1;
}

/**
 * write patient data to file
 * @param patient_ptr array of patients
 * @param totalPatients total number of patients
 */
void write_to_patient_file_ll(struct Node* head) {
    FILE *file = fopen(patientII_file_path, "w");
    if (file == NULL) {
        printf("error opening file");
        return ;
    }
    struct Node* current = head;

    while (current != NULL) {
        fprintf(file, "%d,%s,%d,%s,%d\n",
            current->patient.patient_id,
            current->patient.name,
            current->patient.age,
            current->patient.diagnosis,
            current->patient.room_number);

        current = current->next;
    }
    fclose(file);
}

/**
 * read from doctor schedule file
 * @param schedule
 * @return
 */
int read_from_schedule_file(char schedule[7][3][50]) {
    FILE *file = fopen(schedule_file_path, "r");
    if (file == NULL) {
        printf("File not found.\n");
    }
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        char *token;
        int row;
        int col;
        char name[50];

        token = strtok(buffer, ",");
        row = atoi(token);
        token = strtok(NULL, ",");
        col = atoi(token);
        token = strtok(NULL, ",");
        if (token) {
            strncpy(name, token, sizeof(name) - 1);
            strcpy(schedule[row][col], name);
        }
    }
    fclose(file);
    return 1;
}

/**
 * wrtie doctor schedule to file
 * @param schedule  array o fdoctor schedule
 */
void write_to_schedule_file(char schedule[7][3][50]) {
    FILE *file = fopen(schedule_file_path, "w");
    if (file == NULL) {
        printf("error opening file");
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 3; j++) {
            fprintf(file, "%d,%d,%s\n",i, j, schedule[i][j]);
        }
    }
    fclose(file);
}
