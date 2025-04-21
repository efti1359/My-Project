#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    char type[20];
    int available;
    float rent_per_day;
} Vehicle;

typedef struct {
    int id;
    char name[50];
    char phone[15];
} Customer;

typedef struct {
    int booking_id;
    int vehicle_id;
    int customer_id;
    char start_date[11];
    int days;
} Booking;

void addVehicle() {
    FILE *fp = fopen("vehicles.dat", "ab");
    Vehicle v;

    printf("Enter vehicle ID: ");
    scanf("%d", &v.id);
    printf("Enter vehicle name: ");
    scanf(" %[^\n]", v.name);
    printf("Enter vehicle type: ");
    scanf(" %[^\n]", v.type);
    printf("Enter rent per day: ");
    scanf("%f", &v.rent_per_day);
    v.available = 1;

    fwrite(&v, sizeof(Vehicle), 1, fp);
    fclose(fp);
    printf("Vehicle added successfully.\n");
}

void viewVehicles() {
    FILE *fp = fopen("vehicles.dat", "rb");
    Vehicle v;

    printf("\nAvailable Vehicles:\n");
    printf("ID\tName\t\tType\t\tRent\tAvailable\n");
    printf("--------------------------------------------------\n");
    while (fread(&v, sizeof(Vehicle), 1, fp)) {
        printf("%d\t%s\t\t%s\t\t%.2f\t%s\n",
               v.id, v.name, v.type, v.rent_per_day, v.available ? "Yes" : "No");
    }
    fclose(fp);
}

void addCustomer() {
    FILE *fp = fopen("customers.dat", "ab");
    Customer c;

    printf("Enter customer ID: ");
    scanf("%d", &c.id);
    printf("Enter customer name: ");
    scanf(" %[^\n]", c.name);
    printf("Enter phone number: ");
    scanf(" %[^\n]", c.phone);

    fwrite(&c, sizeof(Customer), 1, fp);
    fclose(fp);
    printf("Customer added successfully.\n");
}

void viewCustomers() {
    FILE *fp = fopen("customers.dat", "rb");
    Customer c;

    printf("\nCustomer List:\n");
    printf("ID\tName\t\tPhone\n");
    printf("-------------------------------\n");
    while (fread(&c, sizeof(Customer), 1, fp)) {
        printf("%d\t%s\t\t%s\n", c.id, c.name, c.phone);
    }
    fclose(fp);
}

void bookVehicle() {
    FILE *vfp = fopen("vehicles.dat", "rb+");
    FILE *bfp = fopen("bookings.dat", "ab");
    Booking b;
    Vehicle v;
    int found = 0;

    printf("Enter booking ID: ");
    scanf("%d", &b.booking_id);
    printf("Enter vehicle ID: ");
    scanf("%d", &b.vehicle_id);

    while (fread(&v, sizeof(Vehicle), 1, vfp)) {
        if (v.id == b.vehicle_id && v.available) {
            fseek(vfp, -sizeof(Vehicle), SEEK_CUR);
            v.available = 0;
            fwrite(&v, sizeof(Vehicle), 1, vfp);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Vehicle not available.\n");
        fclose(vfp);
        fclose(bfp);
        return;
    }

    printf("Enter customer ID: ");
    scanf("%d", &b.customer_id);
    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%s", b.start_date);
    printf("Enter number of days: ");
    scanf("%d", &b.days);

    fwrite(&b, sizeof(Booking), 1, bfp);
    printf("Booking successful.\n");

    fclose(vfp);
    fclose(bfp);
}

void returnVehicle() {
    int booking_id;
    Booking b;
    FILE *bfp = fopen("bookings.dat", "rb");
    FILE *vfp = fopen("vehicles.dat", "rb+");

    printf("Enter booking ID to return: ");
    scanf("%d", &booking_id);

    while (fread(&b, sizeof(Booking), 1, bfp)) {
        if (b.booking_id == booking_id) {
            Vehicle v;
            rewind(vfp);
            while (fread(&v, sizeof(Vehicle), 1, vfp)) {
                if (v.id == b.vehicle_id) {
                    fseek(vfp, -sizeof(Vehicle), SEEK_CUR);
                    v.available = 1;
                    fwrite(&v, sizeof(Vehicle), 1, vfp);

                    int days_used;
                    printf("Enter number of days used: ");
                    scanf("%d", &days_used);

                    float total_cost = v.rent_per_day * days_used;
                    if (days_used > b.days) {
                        int extra_days = days_used - b.days;
                        float penalty = extra_days * 100;
                        total_cost += penalty;
                        printf("Late return penalty: %.2f\n", penalty);
                    }

                    printf("Total cost: %.2f\n", total_cost);
                    break;
                }
            }
        }
    }

    fclose(vfp);
    fclose(bfp);
}

int adminLogin() {
    char user[20], pass[20];
    printf("Admin Login\nUsername: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0)
        return 1;
    else
        return 0;
}

int main() {
    int choice;
    if (!adminLogin()) {
        printf("Login failed.\n");
        return 0;
    }

    do {
        printf("\n--- Vehicle Rental Management System ---\n");
        printf("1. Add Vehicle\n");
        printf("2. View Vehicles\n");
        printf("3. Add Customer\n");
        printf("4. View Customers\n");
        printf("5. Book Vehicle\n");
        printf("6. Return Vehicle\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addVehicle();
            break;
        case 2:
            viewVehicles();
            break;
        case 3:
            addCustomer();
            break;
        case 4:
            viewCustomers();
            break;
        case 5:
            bookVehicle();
            break;
        case 6:
            returnVehicle();
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}