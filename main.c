#include <stdio.h>
#include <string.h>

int idExists();
void menu();
void printPatient();
void addPatient();
void viewPatients();
int searchPatientByName();
void searchPatients();
void organizePatientList();
void dischargePatient();
void manageDrSchedule();

int totalPatients = 0;

const int DAYS_OF_WEEK = 7;
const int SHIFTS_PER_DAY = 3;
const int MONDAY = 1;
const int TUESDAY = 2;
const int WEDNESDAY = 3;
const int THURSDAY = 4;
const int FRIDAY = 5;
const int SATURDAY = 6;
const int SUNDAY = 7;
const int MAX_PATIENTS = 50;
const int DELETE = 666;

char * schedule[7][3];

int main(void)
{
    menu();
    return 0;
}


//struct representing each patient
struct Patient {
    int patient_id;
    char name[50];
    int age;
    char diagnosis[50];
    int room_number;
};

struct Patient patients[50];


int idExists(int id, struct Patient patients[], int size) {
    for (int i = 0; i < size; i++ ) {
        if (patients[i].patient_id == id) {
            return i;
        }
    }
        return -1;
}

void printPatient(int index) {
    printf("Patient ID:");
    printf("%d\n", patients[index].patient_id);
    printf("Name:");
    printf("%s\n", patients[index].name);
    printf("Age:");
    printf("%d\n", patients[index].age);
    printf("Diagnosis:");
    printf("%s\n", patients[index].diagnosis);
    printf("Room Number:");
    printf("%d\n", patients[index].room_number);
}

//enter patients based on user input
void addPatient() {
    int id;
    char name[50];
    int age;
    char diagnosis[50];
    int room_number;
    int status = 0;

    struct Patient patient = {};

    if (totalPatients >= MAX_PATIENTS) {
        printf("Patient Regsitry Full.");
        return;
    }

    do {
        printf("Enter Patient ID: \n");
        scanf("%d", &id);
        getchar();

        if (id <= 0 || idExists(id, patients, totalPatients) != -1) {
            printf("Invalid or duplicate ID\n");
            printf("Re-Enter\n");
        }else {
            patient.patient_id = id;
            status = 1;
        }
    }while (status == 0);
    status = 0;

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

    status = 0;

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

    patients[totalPatients] = patient;
    totalPatients++;
    printf("Patient Added Successfully\n");
}

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

int searchPatientByName(char name[], struct Patient patients[], int totalPatients) {
    if (strlen(name) == 0) {
        printf("name cannot be empty.\n");
    }else {
        for (int i = 0; i < totalPatients; i++) {
            if (strcmp(name, patients[i].name) == 0) {
                return i;
            }
        }
    }

    return -1;

}

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
            index = idExists(id, patients, totalPatients);
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
            index = searchPatientByName(name, patients, totalPatients);
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

void organizePatientList(int index, struct Patient patients[], int totalPatients) {
    struct Patient def = {};
    for (int i = index; i < totalPatients - 1; i++  ) {
        patients[i] = patients[i + 1];
    }
    patients[totalPatients - 1] = def;
}

void dischargePatient() {
    int choice;
    int id;
    char name[50];
    int index;
    struct Patient def = {};

    printf("Discharge Patient.\n");
    printf("1. Discharge By ID.\n");
    printf("2. Discharge By Name.\n");
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1:
            printf("Enter ID:\n");
            scanf("%d", &id);
            index = idExists(id, patients, totalPatients);
            if (index != -1) {
                patients[index] = def;
                organizePatientList(index, patients, totalPatients);
                totalPatients --;
                printf("Patient discharged successfully.\n");
            }else {
                printf("Patient with ID %d does not exist.\n", id);
            }
            break;
        case 2:
            printf("Enter Name: \n");
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = 0;
            index = searchPatientByName(name, patients, totalPatients);
            if (index != -1) {
                patients[index] = def;
                organizePatientList(index, patients, totalPatients);
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

void manageDrSchedule() {
    int choice;

    printf("What would you like to do?\n");
    printf("1. Add Doctor to Schedule\n");
    printf("2. View Schedule\n");

    scanf("%d", &choice);
    getchar();

    switch (choice) {
        case 1:
            char name[50];
            int shiftDay;
            int shiftTime;

            printf("Enter Doctor Name:\n");
            fgets(name, sizeof(name), stdin);
            name[strcspn(name, "\n")] = 0;

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

            schedule[shiftDay][shiftTime] = strdup(name);
            break;

        case 2:
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
                        printf("\t\t%s", schedule[i][j]);
                }

                printf("\n");
            }
        break;

        default:
            printf("Invalid Input:\n");
    }
}


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
               "Enter your choice: ");

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