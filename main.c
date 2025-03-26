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
int     searchPatientByName();
void    searchPatients();
void    organizePatientList();
void    dischargePatient();
void    manageDrSchedule();
void    toLowerString();
int     readFromFile();
void    writeToFile();

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

//struct representing each patient
struct Patient {
    int patient_id;
    char name[50];
    int age;
    char diagnosis[50];
    int room_number;
};

// struct Patient patients[50];//size: 5800 ; each patient: 116
struct Patient *patient_ptr;

// MAIN METHOD
/**
 * Main method. Beginning of the flow of control.
 *
 * @return int 0 if program has terminated successfully.
 */
int main(void)
{
    patient_ptr = (struct Patient*)malloc(max_patients * sizeof(struct Patient));
    if (patient_ptr == NULL) {
        printf("Memory allocation failed in main()\n");
        return 1;
    }
    if (readFromFile(&patient_ptr, &totalPatients) == 0) {
        printf("Starting with empty patient database.\n");
    }
    menu();
    writeToFile(patient_ptr, totalPatients);
    free(patient_ptr);
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
        printf("3. Search Patient by ID\n");
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
                viewPatients();
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

    // Checks room for more patients at the hospital
    if (totalPatients >= max_patients) {
        //expand memory for patient_ptr here
        max_patients += 10;
        struct Patient *tmp = realloc(patient_ptr, max_patients * sizeof(struct Patient));
        if (tmp == NULL) {
            printf("Memory allocation failed in addPatient()\n");
        }
        patient_ptr = tmp;
    }

    // Ensures patient ID is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Patient ID: \n");
        scanf("%d", &id);
        getchar();

        if (id <= 0 || idExists(id, patient_ptr, totalPatients) != -1) {
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

    //with pointer
    patient_ptr[totalPatients] = patient;

    totalPatients++;
    printf("Patient Added Successfully\n");
}

/**
 * Prints the list of patients and their information
 */
void viewPatients() {
    printf("Printing Patient Information...\n");
    for (int i = 0; i < totalPatients; i++) {
        printf("--------------------------\n");
        printPatient(i);
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
        index = idExists(id, patient_ptr, totalPatients);
        if (index != -1) {
            printf("--------------------------\n");
            printPatient(index);
            printf("--------------------------\n");
        }else {
            printf("No patients associated with ID %d.\n", id);
        }
        break;
        case 2:
            printf("Enter Patient Name:");
        fgets(name, 50, stdin);
        name[strcspn(name, "\n")] = 0;
        index = searchPatientByName(name, patient_ptr, totalPatients);
        if (index == -1) {
            printf("No patients with the name %s exists.\n", name);
        }else {
            printf("--------------------------\n");
            printPatient(index);
            printf("--------------------------\n");
        }
        break;
        default:
            printf("Invalid Input. Terminating...\n");
        break;
    }

}

/**
 * Allows the user to search by name if a patient is checked into the hospital. If the patient does exist, the function
 * returns the index they can be found in the array. Else -1 is returned.
 *
 * @param name          - array of chars representing a patient's name.
 * @param patient_ptr      - pointer to an array of patients.
 * @param totalPatients - int representing the number of patients currently checked into the hospital.
 *
 * @return              - int representing the integer representing the index a patient can be found.
 */
int searchPatientByName(char name[], struct Patient *patient_ptr, int totalPatients) {
    if (strlen(name) == 0) {
        printf("name cannot be empty.\n");
    }else {
        toLowerString(name);
        for (int i = 0; i < totalPatients; i++) {
            if (strcmp(name, patient_ptr[i].name) == 0) {
                return i;
            }
        }
    }

    return -1;

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
            index = idExists(id, patient_ptr, totalPatients);
            if (index != -1) {
                patient_ptr[index] = def;
                organizePatientList(index, patient_ptr, totalPatients);
                totalPatients --;
                printf("Patient discharged successfully.\n");
            }else {
                printf("Patient with ID %d does not exist.\n", id);
            }
            break;

        case 2: // Checks Patient Name, removing them if found.
            printf("Enter Name: \n");
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = 0;
            toLowerString(name);
            index = searchPatientByName(name, patient_ptr, totalPatients);
            if (index != -1) {
                patient_ptr[index] = def;
                organizePatientList(index, patient_ptr, totalPatients);
                totalPatients --;
                printf("Patient discharged successfully.\n");
            }else {
                printf("Patient with name %s does not exist.\n", name);
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
/**
 * Checks if a passed in patient ID exists in the system.
 * If it does the index of the patient is returned, else -1 is returned.
 *
 * @param id        - the ID you would like to check to see if it exists in the system.
 * @param patient_ptr  - pointer to the array of patients in the system.
 * @param size      - the size of the patient array the method will check through.
 * @return int      - representing the index of the patient whose ID was passed into the method. If no patient is found
 *                  then -1 is returned instead.
 */
int idExists(int id, struct Patient *patient_ptr, int size) {
    for (int i = 0; i < size; i++ ) {
        if (patient_ptr[i].patient_id == id) {
            return i;
        }
    }
    return -1;
}

/**
 * Organizes the list of patients in the hotel.
 *
 * @param index         - index of an incoming patient in the array
 * @param patient_ptr      - pointer to ab array of patients
 * @param totalPatients - int representing the total number of patients checked into the hotel
 */
void organizePatientList(int index, struct Patient *patient_ptr, int totalPatients) {
    struct Patient def = {};
    for (int i = index; i < totalPatients - 1; i++  ) {
        patient_ptr[i] = patient_ptr[i + 1];
    }
    patient_ptr[totalPatients - 1] = def;
}

/**
 * Prints the information of the patient found at the passed in index.
 * This information includes their: ID,
 *                                  Name,
 *                                  Age,
 *                                  Diagnosis, and
 *                                  Room Number.
 *
 * @param index int - representing the index of the patient's location in the struct array.
 */
void printPatient(int index) {
    // printf("Patient ID:");
    // printf("%d\n", patients[index].patient_id);
    // printf("Name:");
    // printf("%s\n", patients[index].name);
    // printf("Age:");
    // printf("%d\n", patients[index].age);
    // printf("Diagnosis:");
    // printf("%s\n", patients[index].diagnosis);
    // printf("Room Number:");
    // printf("%d\n", patients[index].room_number);
    printf("Patient ID:");
    printf("%d\n", patient_ptr[index].patient_id);
    printf("Name:");
    printf("%s\n", patient_ptr[index].name);
    printf("Age:");
    printf("%d\n", patient_ptr[index].age);
    printf("Diagnosis:");
    printf("%s\n", patient_ptr[index].diagnosis);
    printf("Room Number:");
    printf("%d\n", patient_ptr[index].room_number);
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
 * read from file
 * @param patient_ptr pointer to patient array
 * @return 1 if file is read successfully, 0 otherwise
 */
int readFromFile(struct Patient **patient_ptr, int *totalPatients) {
    *totalPatients = 0;
    FILE *file = fopen("patients.txt", "r");
    if (file == NULL) {
        printf("File not found.\n");
        return 0;
    }

    char buffer[200];

    while (fgets(buffer, sizeof(buffer), file)) {
        if (*totalPatients >= max_patients) {
            max_patients += 10;
            struct Patient *tmp = realloc(*patient_ptr, max_patients * sizeof(struct Patient));
            if (!tmp) {
                printf("Memory allocation error while reading file.\n");
                fclose(file);
                return 0;
            }
            *patient_ptr = tmp;
        }

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

        (*patient_ptr)[(*totalPatients)++] = p;
    }

    fclose(file);
    return 1;
}



/**
 * write patient data to file
 * @param patient_ptr array of patients
 * @param totalPatients total number of patients
 */
void writeToFile(struct Patient *patient_ptr, int totalPatients) {
    FILE *file = fopen("patients.txt", "w");
    if (file == NULL) {
        printf("error opening file");
        return ;
    }
    for (int i = 0; i < totalPatients; i++) {
        fprintf(file, "%d,%s,%d,%s,%d\n",
            patient_ptr[i].patient_id,
            patient_ptr[i].name,
            patient_ptr[i].age,
            patient_ptr[i].diagnosis,
            patient_ptr[i].room_number);
    }
    fclose(file);
}
