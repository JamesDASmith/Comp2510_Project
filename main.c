#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SCHEDULE_DAYS   7
#define SCHEDULE_SHIFTS 3
#define INPUT_CHARS     50


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
int     read_from_patient_file();
int     read_from_schedule_file();
void    write_to_patient_file();
void    write_to_schedule_file();
int     isNameValidInput(const char *name);
int     isIntegerValidInput(const int num);
void    freeMemory();
void    generateReportsMenu();
void    generateAdmissionReport();
void    generateDischargeReport();
void    generateDoctorUtilizationReport();
void    generateRoomUsageReport();
void    saveReportToFile(const char *filename, const char *content);

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
int patient_added_current_session = 0;
int patient_discharged_current_session = 0;
int rooms[50] = {0}; // Assuming room numbers up to 50

const char patient_file_path[] = "patients.txt";
const char schedule_file_path[] = "schedule.txt";

const char admission_reports_file_path[] = "admission_report.txt";
const char discharge_report_file_path[] = "discharge_report.txt";
const char dr_report_file_path[] = "doctor_utilization.txt";
const char room_report_file_path[] = "room_utilization.txt";

char schedule[SCHEDULE_DAYS][SCHEDULE_SHIFTS][INPUT_CHARS];

//struct representing each patient
struct Patient
{
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
    if (patient_ptr == NULL)
    {
        printf("Memory allocation failed in main()\n");
        return 1;
    }
    if (read_from_patient_file(&patient_ptr, &totalPatients) == 0)
    {
        printf("Starting with empty patient database.\n");
    }
    read_from_schedule_file(&schedule);
    menu();
    write_to_patient_file(patient_ptr, totalPatients);
    free(patient_ptr);
    freeMemory();
    return 0;
}

// PRIMARY METHODS
/**
 * Represents the main menu from which a user can choose to use any of the programs functions.
 */
void menu()
{
    int choice;
    do {
        printf("\nHospital Management System\n");
        printf("1. Add Patient Record\n");
        printf("2. View All Patients\n");
        printf("3. Search Patient by ID\n");
        printf("4. Discharge Patient\n");
        printf("5. Manage Doctor Schedule\n");
        printf("6. Generate Reports\n");
        printf("7. Exit\n"
               "Enter your choice: \n");

        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice)
        {
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
                break;
            case 6:
                generateReportsMenu();
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 7);
    printf("Thanks for visiting!");
}

/**
 * Represents the menu from which hospital reports are handled.
 */
void generateReportsMenu()
{
    int choice;
    do {
        printf("\nGenerate Reports\n");
        printf("1. Admission Report\n");
        printf("2. Discharge Report\n");
        printf("3. Doctor Utilization Report\n");
        printf("4. Room Usage Report\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                generateAdmissionReport();
            break;
            case 2:
                generateDischargeReport();
            break;
            case 3:
                generateDoctorUtilizationReport();
            break;
            case 4:
                generateRoomUsageReport();
            break;
            case 5:
                printf("Returning to main menu...\n");
            break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);
}

/**
 * Creates a patient, prompting a user for input.
 */
void addPatient()
{
    int id;
    char name[INPUT_CHARS];
    int age;
    char diagnosis[INPUT_CHARS];
    int room_number;
    int status = 0;

    struct Patient patient = {};

    // Checks room for more patients at the hospital
    if (totalPatients >= max_patients)
    {
        //expand memory for patient_ptr here
        max_patients += 10;
        struct Patient *tmp = realloc(patient_ptr, max_patients * sizeof(struct Patient));
        if (tmp == NULL)
        {
            printf("Memory allocation failed in addPatient()\n");
        }
        patient_ptr = tmp;
    }

    // Ensures patient ID is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Patient ID: \n");
        scanf("%d", &id);
        getchar();

        if (!isIntegerValidInput(id) ||
            idExists(id, patient_ptr, totalPatients) != -1)
        {
            printf("Invalid or duplicate ID\n");
            printf("Re-Enter\n");
        }
        else
        {
            patient.patient_id = id;
            status = 1;
        }
    } while (status == 0);
    status = 0;

    // Ensures patient Name is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Patient Name: \n");
        fgets(patient.name, 50, stdin);
        patient.name[strcspn(patient.name, "\n")] = 0;

        while (!isNameValidInput(patient.name))
        {
            printf("Invalid Patient Name. Reenter\n");
            fgets(patient.name, 50, stdin);
            patient.name[strcspn(patient.name, "\n")] = 0;
        }

        if (strlen(patient.name) == 0)
        {
            printf("Name cannot be empty.\n");
            printf("Re-Enter\n");
        }
        else
        {
            status = 1;
        }
    } while (status == 0);
    toLowerString(patient.name);
    status = 0;

    // Ensures patient Age is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Age: \n");
        scanf("%d", &age);
        if (!isIntegerValidInput(age))
        {
            printf("Invalid age!\n");
            printf("Re-Enter\n");
        }
        else
        {
            patient.age = age;
            status = 1;
        }
        getchar();
    } while (status == 0);
    status = 0;

    // Ensures patient Diagnosis is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Diagnosis: \n");
        fgets(patient.diagnosis, 50, stdin);
        patient.diagnosis[strcspn(patient.diagnosis, "\n")] = 0;
        if (strlen(patient.diagnosis) == 0)
        {
            printf("Diagnosis cannot be empty\n");
            printf("Re-Enter.\n");
        }
        else
        {
            status = 1;
        }
    } while (status == 0);
    status = 0;

    // Ensures patient Room Number is successfully input, continues to prompt in case of invalid input.
    do {
        printf("Enter Room Number: \n");
        scanf("%d", &room_number);
        if (!isIntegerValidInput(room_number))
        {
            printf("Invalid room number.\n");
            printf("Re-Enter.\n");
        }
        else
        {
            patient.room_number = room_number;
            status = 1;
        }
    }while (status == 0);

    rooms[room_number]++;

     patient_ptr[totalPatients] = patient;
     totalPatients++;
     patient_added_current_session++;


    printf("Patient Added Successfully\n");
}

/**
 * Prints the list of patients and their information
 */
void viewPatients()
{
    printf("Printing Patient Information...\n");
    for (int i = 0; i < totalPatients; i++)
    {
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
void searchPatients()
{
    int selection;
    int id;
    int index;
    char name[50];

    printf("1. Search Patient By ID\n");
    printf("2. Search Patient By Name\n");
    scanf("%d", &selection);
    getchar();

    switch (selection)
    {
        case 1:
            printf("Enter Patient ID:");
            scanf("%d", &id);

            if (!isIntegerValidInput(id))
            {
                printf("Invalid ID!\n");
                return;
            }
            //with array
            index = idExists(id, patient_ptr, totalPatients);
            if (index != -1)
            {
                printf("--------------------------\n");
                printPatient(index);
                printf("--------------------------\n");
            }
            else
            {
                printf("No patients associated with ID %d.\n", id);
            }
            break;
        case 2:
            printf("Enter Patient Name:");
            fgets(name, INPUT_CHARS, stdin);
            name[strcspn(name, "\n")] = 0;

            if (!isNameValidInput(name))
            {
                printf("Invalid Patient Name. Exiting...\n");
                return;
            }

            index = searchPatientByName(name, patient_ptr, totalPatients);
            if (index == -1)
            {
                printf("No patients with the name %s exists.\n", name);
            }
            else
            {
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
int searchPatientByName(char name[], struct Patient *patient_ptr, int totalPatients)
{
    if (strlen(name) == 0)
    {
        printf("name cannot be empty.\n");
    }
    else
    {
        toLowerString(name);
        for (int i = 0; i < totalPatients; i++)
        {
            if (strcmp(name, patient_ptr[i].name) == 0)
            {
                return i;
            }
        }
    }

    return -1;

}

/**
 * Prompts the user for information about a patient (either name or ID) and removes them from the patient list.
 */
void dischargePatient()
{
    int choice;
    int id;
    char name[INPUT_CHARS];
    int index;
    struct Patient def = {};

    // Menu
    printf("Discharge Patient.\n");
    printf("1. Discharge By ID.\n");
    printf("2. Discharge By Name.\n");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
        case 1: // Checks Patient ID, removing them if found.
            printf("Enter ID:\n");
            scanf("%d", &id);

            if (!isIntegerValidInput(id))
            {
                printf("Invalid ID!\n");
                return;
            }

            index = idExists(id, patient_ptr, totalPatients);
            if (index != -1)
            {
                patient_ptr[index] = def;
                organizePatientList(index, patient_ptr, totalPatients);
                totalPatients --;
                printf("Patient discharged successfully.\n");
            }
            else
            {
                printf("Patient with ID %d does not exist.\n", id);
            }
            break;

        case 2: // Checks Patient Name, removing them if found.
            printf("Enter Name: \n");
            fgets(name, INPUT_CHARS, stdin);
            name[strcspn(name, "\n")] = 0;

            if (!isNameValidInput(name))
            {
                printf("Invalid Patient Name. Terminating...\n");
                return;
            }

            toLowerString(name);
            index = searchPatientByName(name, patient_ptr, totalPatients);
            if (index != -1)
            {
                patient_ptr[index] = def;

                rooms[patient_ptr[index].room_number]--;
                patient_discharged_current_session++;

                organizePatientList(index, patient_ptr, totalPatients);
                totalPatients --;
                printf("Patient discharged successfully.\n");
            }
            else
            {
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
void manageDrSchedule()
{
    int choice;

    // Menu
    printf("What would you like to do?\n");
    printf("1. Add Doctor to Schedule\n");
    printf("2. View Schedule\n");

    scanf("%d", &choice);
    getchar(); // Remove newline

    switch (choice)
    {
        case 1: // User chooses to add a doctor to the schedule.
            char name[INPUT_CHARS];
            int shiftDay;
            int shiftTime;
            int status = 0;

            // User inputs Doctor Name
            do {
                printf("Enter Doctor Name:\n");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                if (!isNameValidInput(name) || strlen(name) == 0)
                {
                    printf("Name cannot be empty.\n");
                    printf("Re-Enter\n");
                }
                else
                {
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

            if(shiftDay > 7 || shiftDay < 1 && isIntegerValidInput(shiftDay))
            {
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

            if(shiftTime > 3 || shiftTime < 1 && isIntegerValidInput(shiftTime))
            {
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
            for (int i = 1; i <= DAYS_OF_WEEK; i++)
            {

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
int idExists(int id,
                struct Patient *patient_ptr,
                int size)
{
    for (int i = 0; i < size; i++ )
    {
        if (patient_ptr[i].patient_id == id)
        {
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
void organizePatientList(int index,
                            struct Patient *patient_ptr,
                            int totalPatients)
{
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
int read_from_patient_file(struct Patient **patient_ptr,
                            int *totalPatients)
{
    *totalPatients = 0;
    FILE *file = fopen(patient_file_path, "r");
    if (file == NULL)
    {
        printf("File not found.\n");
        return 0;
    }

    char buffer[200];

    while (fgets(buffer, sizeof(buffer), file))
        {
        if (*totalPatients >= max_patients)
            {
            max_patients += 10;
            struct Patient *tmp = realloc(*patient_ptr, max_patients * sizeof(struct Patient));
            if (!tmp)
            {
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
void write_to_patient_file(struct Patient *patient_ptr,
                            int totalPatients)
{
    FILE *file = fopen(patient_file_path, "w");
    if (file == NULL)
    {
        printf("error opening file");
        return ;
    }
    for (int i = 0; i < totalPatients; i++)
    {
        fprintf(file, "%d,%s,%d,%s,%d\n",
            patient_ptr[i].patient_id,
            patient_ptr[i].name,
            patient_ptr[i].age,
            patient_ptr[i].diagnosis,
            patient_ptr[i].room_number);
    }
    fclose(file);
}

/**
 *
 * @param schedule the weekly doctor schedule
 * @return represents a successful read from file.
 */
int read_from_schedule_file(char schedule[7][3][50])
{
    FILE *file = fopen(schedule_file_path, "r");
    if (file == NULL)
    {
        printf("File not found.\n");
    }
    char buffer[200];
    while (fgets(buffer, sizeof(buffer), file))
        {
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
        if (token)
        {
            strncpy(name, token, sizeof(name) - 1);
            strcpy(schedule[row][col], name);
        }
    }
    fclose(file);
}

/**
 * wrtie doctor schedule to file
 * @param schedule  array o fdoctor schedule
 */
void write_to_schedule_file(char schedule[7][3][50])
{
    FILE *file = fopen(schedule_file_path, "w");
    if (file == NULL)
    {
        printf("error opening file");
    }
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            fprintf(file, "%d,%d,%s\n",i, j, schedule[i][j]);
        }
    }
    fclose(file);
}

/**
 * generates a report how many patients were admitted over a given time period.
 */
void generateAdmissionReport()
{
    char timePeriod[20];
    int count;
    char report[1000] = "=== PATIENT ADMISSION REPORT ===\n\n";

    printf("\nSelect time period:\n");
    printf("1. Today\n");
    printf("2. This Week\n");
    printf("3. This Month\n");
    printf("Enter choice: ");
    int periodChoice;
    scanf("%d", &periodChoice);
    getchar();

    switch(periodChoice)
    {
        case 1:
            strcpy(timePeriod, "Today");
            count = patient_added_current_session;
            break;
        case 2:
            strcpy(timePeriod, "This Week");
            count = patient_added_current_session * 7; // Simplified - would normally track daily
            break;
        case 3:
            strcpy(timePeriod, "This Month");
            count = patient_added_current_session * 30; // Simplified - would normally track daily
            break;
        default:
            printf("Invalid choice. Using today's data.\n");
            strcpy(timePeriod, "Today");
            count = patient_added_current_session;
    }

    char temp[200];
    sprintf(temp, "Period: %s\nTotal Admissions: %d\n\n", timePeriod, count);
    strcat(report, temp);

    printf("\n%s", report);

    printf("Save to file? (y/n): ");
    char saveChoice;
    scanf("%c", &saveChoice);
    getchar();

    if (tolower(saveChoice) == 'y')
    {
        saveReportToFile(admission_reports_file_path,
                            report);
    }
}

/**
 * Generates a report on how many patients have been discharged on a given day.
 */
void generateDischargeReport()
{
    char date[20];
    char report[1000] = "=== PATIENT DISCHARGE REPORT ===\n\n";

    printf("Enter date for discharge report (DD-MM-YYYY): ");
    fgets(date, 20, stdin);
    date[strcspn(date, "\n")] = 0;

    // In a real system, we'd check discharge dates, but for now we'll use our counter
    char temp[200];
    sprintf(temp, "Date: %s\nPatients Discharged: %d\n\n", date, patient_discharged_current_session);
    strcat(report, temp);

    // List discharged patients (in a real system, we'd have discharge dates)
    strcat(report, "Discharged Patients:\n");
    for (int i = 0; i < totalPatients; i++)
    {
        if (patient_ptr[i].patient_id == 0)
        { // Our placeholder for discharged patients
            sprintf(temp, "ID: %d, Name: %s\n",
                   patient_ptr[i].patient_id, patient_ptr[i].name);
            strcat(report, temp);
        }
    }

    printf("\n%s", report);

    printf("Save to file? (y/n): ");
    char saveChoice;
    scanf("%c", &saveChoice);
    getchar();

    if (tolower(saveChoice) == 'y')
    {
        saveReportToFile(discharge_report_file_path,
                            report);
    }
}

/**
 * Generates a report for how often each doctor is scheduled for a shift.
 */
void generateDoctorUtilizationReport()
{
    char report[2000] = "=== DOCTOR UTILIZATION REPORT ===\n\n";
    char temp[200];

    // Count shifts per doctor
    typedef struct
    {
        char name[INPUT_CHARS];
        int shifts;
    } DoctorCount;

    DoctorCount doctors[INPUT_CHARS] = {0};
    int numDoctors = 0;

    // Count doctor shifts
    for (int i = 0; i < SCHEDULE_DAYS; i++)
    {
        for (int j = 0; j < SCHEDULE_SHIFTS; j++)
        {
            if (strlen(schedule[i][j]) > 0)
            {
                int found = 0;
                for (int k = 0; k < numDoctors; k++)
                {
                    if (strcmp(doctors[k].name, schedule[i][j]) == 0)
                    {
                        doctors[k].shifts++;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                {
                    strcpy(doctors[numDoctors].name, schedule[i][j]);
                    doctors[numDoctors].shifts = 1;
                    numDoctors++;
                }
            }
        }
    }

    // Generate report
    strcat(report, "Doctor\t\tShifts This Week\n");
    strcat(report, "--------------------------------\n");

    for (int i = 0; i < numDoctors; i++)
    {
        sprintf(temp, "%s\t\t%d\n", doctors[i].name, doctors[i].shifts);
        strcat(report, temp);
    }

    printf("\n%s", report);

    printf("Save to file? (y/n): ");
    char saveChoice;
    scanf("%c", &saveChoice);
    getchar();

    if (tolower(saveChoice) == 'y')
    {
        saveReportToFile(dr_report_file_path,
                        report);
    }
}

/**
 * Generates a report for how much each of the rooms is used during a given time span.
 */
void generateRoomUsageReport()
{
    char report[2000] = "=== ROOM UTILIZATION REPORT ===\n\n";
    char temp[200];

    strcat(report, "Room\tPatients\tStatus\n");
    strcat(report, "--------------------------------\n");

    for (int i = 0; i < 50; i++)
    {
        if (rooms[i] > 0)
        {
            sprintf(temp, "%d\t%d\t\t%s\n", i, rooms[i],
                   rooms[i] > 1 ? "OVERUTILIZED" : "NORMAL");
            strcat(report, temp);
        }
        else
        {
            sprintf(temp, "%d\t0\t\tUNDERUTILIZED\n", i);
            strcat(report, temp);
        }
    }

    printf("\n%s", report);

    printf("Save to file? (y/n): ");
    char saveChoice;
    scanf("%c", &saveChoice);
    getchar();

    if (tolower(saveChoice) == 'y')
    {
        saveReportToFile(room_report_file_path,
                            report);
    }
}

/**
 * Asks user if they would like to save the report passed in to a file ofr future use.
 *
 * @param filename name of the file to be saved
 * @param content  formatted content to be saved in the report
 */
void saveReportToFile(const char *filename,
                        const char *content)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error saving report to file!\n");
        return;
    }

    fprintf(file, "%s", content);
    fclose(file);
    printf("Report saved to %s\n", filename);
}

/**
 *
 * @param name incoming name to check if valid
 * @return 1 if valid input, else 0
 */
int isNameValidInput(const char *name)
{
    if (strlen(name) <= 0 ||
        strpbrk(name, "0123456789!@#$%^&*()") == NULL)
    {
        return 1; // valid input
    }
    return 0; // invalid input
}

/**
 *
 * @param num incoming number to check validity
 * @return 1 if valid, else 0
 */
int isIntegerValidInput(const int num)
{
    if (num >= 0)
    {
        return 1;
    }
    return 0;
}

/**
 * Clears us memory on system close.
 */
void freeMemory()
{
    free(patient_file_path);
    free(schedule_file_path);
    free(patient_ptr);

    for (int h = 0; h < SCHEDULE_DAYS; h++)
    {
        for (int i=0; i < SCHEDULE_SHIFTS; i++)
        {
            for (int j = 0 ; j < INPUT_CHARS; j++)
                {
                free(schedule[h][i][j]);
            }
        }
    }
    free(schedule);
}
