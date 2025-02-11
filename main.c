#include <stdio.h>

void menu();
void addPatient();
void viewPatients();
void searchPatients();
void dischargePatient();
void manageDrSchedule();

int main(void)
{
    menu();
    return 0;
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