#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Screen clear function */
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/* System Header Banner */
void printHeader() {
    printf("====================================================\n");
    printf("     WELCOME TO SMART WATER USAGE MONITORING SYSTEM  \n");
    printf("====================================================\n");
}

/* Pause function until Enter key is pressed */
void pauseConsole() {
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

struct User
{
    char username[30];
    char password[30];
    char name[50];
    char flatNo[20];
};

/* Feature 1: User Registration */
void registration()
{
    clearScreen();
    printHeader();
    struct User u;
    FILE *fp;

    fp = fopen("credentials.txt", "a");

    if(fp == NULL)
    {
        printf("\nFile Open Error!\n");
        pauseConsole();
        return;
    }

    printf("\n----------------------------------------------------\n");
    printf("                 USER REGISTRATION                  \n");
    printf("----------------------------------------------------\n");

    printf("Enter Username (No spaces, e.g. afsana): ");
    scanf("%s", u.username);

    printf("Enter Password: ");
    scanf("%s", u.password);

    printf("Enter Name (Use '_' for spaces, e.g. Afsana_Akter): ");
    scanf("%s", u.name);

    printf("Enter Flat Number (e.g. 02): ");
    scanf("%s", u.flatNo);

    fprintf(fp, "%s %s %s %s\n",
            u.username,
            u.password,
            u.name,
            u.flatNo);

    fclose(fp);

    printf("\n[SUCCESS] Registration Successful!\n");
    pauseConsole();
}

/* Feature 2: User Login */
int login(char loggedUser[])
{
    clearScreen();
    printHeader();
    struct User u;
    char username[30];
    char password[30];

    FILE *fp;

    fp = fopen("credentials.txt", "r");

    if(fp == NULL)
    {
        printf("\nNo registered user found! Please register first.\n");
        pauseConsole();
        return 0;
    }

    printf("\n----------------------------------------------------\n");
    printf("                     USER LOGIN                     \n");
    printf("----------------------------------------------------\n");

    printf("Enter Username: ");
    scanf("%s", username);

    printf("Enter Password: ");
    scanf("%s", password);

    while(fscanf(fp, "%s %s %s %s",
                 u.username,
                 u.password,
                 u.name,
                 u.flatNo) != EOF)
    {
        if(strcmp(username, u.username) == 0 &&
           strcmp(password, u.password) == 0)
        {
            strcpy(loggedUser, username);
            fclose(fp);

            printf("\n[SUCCESS] Login Successful!\n");
            printf("Welcome to Smart Water Usage Monitoring System, %s!\n", u.name);
            pauseConsole();
            return 1;
        }
    }

    fclose(fp);

    printf("\n[ERROR] Invalid Username or Password!\n");
    pauseConsole();
    return 0;
}

/* Feature 3: Profile Management */
void profileManagement(char loggedUser[])
{
    clearScreen();
    printHeader();
    struct User u;
    FILE *fp, *temp;
    char newName[50], newFlat[20];
    int found = 0;

    fp = fopen("credentials.txt", "r");
    temp = fopen("temp.txt", "w");

    if(fp == NULL || temp == NULL)
    {
        printf("\nFile Error!\n");
        pauseConsole();
        return;
    }

    printf("\n----------------------------------------------------\n");
    printf("                 PROFILE MANAGEMENT                 \n");
    printf("----------------------------------------------------\n");

    while(fscanf(fp, "%s %s %s %s", u.username, u.password, u.name, u.flatNo) != EOF)
    {
        if(strcmp(loggedUser, u.username) == 0)
        {
            found = 1;
            printf("Current Username : %s\n", u.username);
            printf("Current Name     : %s\n", u.name);
            printf("Current Flat No  : %s\n\n", u.flatNo);

            printf("Enter New Name (Use '_' for spaces): ");
            scanf("%s", newName);

            printf("Enter New Flat Number: ");
            scanf("%s", newFlat);

            strcpy(u.name, newName);
            strcpy(u.flatNo, newFlat);
        }

        fprintf(temp, "%s %s %s %s\n", u.username, u.password, u.name, u.flatNo);
    }

    fclose(fp);
    fclose(temp);

    remove("credentials.txt");
    rename("temp.txt", "credentials.txt");

    if(found)
        printf("\n[SUCCESS] Profile Updated Successfully!\n");
    else
        printf("\n[ERROR] User Not Found!\n");

    pauseConsole();
}

/* Feature 4: Add Daily Water Usage */
void addWaterUsage(char loggedUser[])
{
    clearScreen();
    printHeader();
    FILE *fp;
    char date[20];
    float liters;

    fp = fopen("usage_records.txt", "a");

    if(fp == NULL)
    {
        printf("\nFile Open Error!\n");
        pauseConsole();
        return;
    }

    printf("\n----------------------------------------------------\n");
    printf("               ADD DAILY WATER USAGE                \n");
    printf("----------------------------------------------------\n");

    printf("Enter Date (DD-MM-YYYY): ");
    scanf("%s", date);

    printf("Enter Water Usage (Liters): ");
    scanf("%f", &liters);

    fprintf(fp, "%s %s %.2f\n", loggedUser, date, liters);
    fclose(fp);

    printf("\n[SUCCESS] Water Usage Saved Successfully!\n");
    pauseConsole();
}

/* Feature 5: View Usage History */
void viewUsageHistory(char loggedUser[])
{
    clearScreen();
    printHeader();
    FILE *fp = fopen("usage_records.txt", "r");
    if(fp == NULL) {
        printf("\nNo usage history found!\n");
        pauseConsole();
        return;
    }

    char user[30], date[20];
    float liters;
    int count = 0;

    printf("\n----------------------------------------------------\n");
    printf("                 VIEW USAGE HISTORY                 \n");
    printf("----------------------------------------------------\n");
    printf("DATE\t\t\tCONSUMPTION\n");
    printf("----------------------------------------------------\n");

    while(fscanf(fp, "%s %s %f", user, date, &liters) != EOF) {
        if(strcmp(loggedUser, user) == 0) {
            printf("%s\t\t%.2f Liters\n", date, liters);
            count++;
        }
    }
    fclose(fp);

    if(count == 0) {
        printf("No records found for %s.\n", loggedUser);
    }
    printf("----------------------------------------------------\n");
    pauseConsole();
}

/* Feature 6: Edit/Delete Records */
void editDeleteRecords(char loggedUser[])
{
    clearScreen();
    printHeader();
    FILE *fp = fopen("usage_records.txt", "r");
    FILE *temp = fopen("temp_usage.txt", "w");
    if(fp == NULL || temp == NULL) {
        printf("\nFile Error!\n");
        pauseConsole();
        return;
    }

    char user[30], date[20], targetDate[20];
    float liters, newLiters;
    int option, found = 0;

    printf("\n----------------------------------------------------\n");
    printf("               EDIT / DELETE RECORDS                \n");
    printf("----------------------------------------------------\n");
    printf("1. Modify Record\n2. Delete Record\nEnter Choice: ");
    scanf("%d", &option);
    printf("Enter Target Date (DD-MM-YYYY): ");
    scanf("%s", targetDate);

    while(fscanf(fp, "%s %s %f", user, date, &liters) != EOF) {
        if(strcmp(loggedUser, user) == 0 && strcmp(targetDate, date) == 0) {
            found = 1;
            if(option == 1) {
                printf("Enter New Water Usage (Liters): ");
                scanf("%f", &newLiters);
                fprintf(temp, "%s %s %.2f\n", user, date, newLiters);
                printf("\n[SUCCESS] Record Updated Successfully!\n");
            } else if(option == 2) {
                printf("\n[SUCCESS] Record Deleted Successfully!\n");
            }
        } else {
            fprintf(temp, "%s %s %.2f\n", user, date, liters);
        }
    }
    fclose(fp);
    fclose(temp);

    remove("usage_records.txt");
    rename("temp_usage.txt", "usage_records.txt");

    if(!found) printf("\nNo record found on %s!\n", targetDate);
    pauseConsole();
}

/* Feature 7: Search Water Usage Records */
void searchUsageRecords(char loggedUser[])
{
    clearScreen();
    printHeader();
    FILE *fp = fopen("usage_records.txt", "r");
    if(fp == NULL) {
        printf("\nNo records found!\n");
        pauseConsole();
        return;
    }

    char user[30], date[20], searchParam[20];
    float liters;
    int found = 0;

    printf("\n----------------------------------------------------\n");
    printf("             SEARCH WATER USAGE RECORDS             \n");
    printf("----------------------------------------------------\n");
    printf("Enter Date to Search (DD-MM-YYYY): ");
    scanf("%s", searchParam);

    while(fscanf(fp, "%s %s %f", user, date, &liters) != EOF) {
        if(strcmp(loggedUser, user) == 0 && strcmp(searchParam, date) == 0) {
            printf("\n[MATCH FOUND] Date: %s | Usage: %.2f Liters\n", date, liters);
            found = 1;
        }
    }
    fclose(fp);

    if(!found) printf("\nNo matching record found.\n");
    pauseConsole();
}

/* Feature 8: Daily Report */
void dailyReport(char loggedUser[])
{
    clearScreen();
    printHeader();
    FILE *fp = fopen("usage_records.txt", "r");
    if(fp == NULL) {
        printf("\nNo records found!\n");
        pauseConsole();
        return;
    }

    char user[30], date[20], targetDate[20];
    float liters, dayTotal = 0.0;

    printf("\n----------------------------------------------------\n");
    printf("                    DAILY REPORT                    \n");
    printf("----------------------------------------------------\n");
    printf("Enter Date for Daily Summary (DD-MM-YYYY): ");
    scanf("%s", targetDate);

    while(fscanf(fp, "%s %s %f", user, date, &liters) != EOF) {
        if(strcmp(loggedUser, user) == 0 && strcmp(targetDate, date) == 0) {
            dayTotal += liters;
        }
    }
    fclose(fp);

    printf("\n--- Summary for %s ---\n", targetDate);
    printf("Total Daily Consumption: %.2f Liters\n", dayTotal);
    pauseConsole();
}

/* Feature 9: Monthly Report */
void monthlyReport(char loggedUser[])
{
    clearScreen();
    printHeader();
    FILE *fp = fopen("usage_records.txt", "r");
    if(fp == NULL) {
        printf("\nNo records found!\n");
        pauseConsole();
        return;
    }

    char user[30], date[20], monthYear[10];
    float liters, monthTotal = 0.0;

    printf("\n----------------------------------------------------\n");
    printf("                   MONTHLY REPORT                   \n");
    printf("----------------------------------------------------\n");
    printf("Enter Month & Year (MM-YYYY): ");
    scanf("%s", monthYear);

    while(fscanf(fp, "%s %s %f", user, date, &liters) != EOF) {
        if(strcmp(loggedUser, user) == 0 && strstr(date, monthYear) != NULL) {
            monthTotal += liters;
        }
    }
    fclose(fp);

    printf("\n--- Summary for %s ---\n", monthYear);
    printf("Total Monthly Usage: %.2f Liters\n", monthTotal);
    pauseConsole();
}

/* Feature 10: Water Bill Estimation */
void waterBillEstimation(char loggedUser[])
{
    clearScreen();
    printHeader();
    FILE *fp = fopen("usage_records.txt", "r");
    if(fp == NULL) {
        printf("\nNo usage records found!\n");
        pauseConsole();
        return;
    }

    char user[30], date[20], monthYear[10];
    float liters, totalLiters = 0.0, bill = 0.0;

    printf("\n----------------------------------------------------\n");
    printf("               WATER BILL ESTIMATION                \n");
    printf("----------------------------------------------------\n");
    printf("Enter Target Month & Year (MM-YYYY): ");
    scanf("%s", monthYear);

    while(fscanf(fp, "%s %s %f", user, date, &liters) != EOF) {
        if(strcmp(loggedUser, user) == 0 && strstr(date, monthYear) != NULL) {
            totalLiters += liters;
        }
    }
    fclose(fp);

    if(totalLiters <= 5000) {
        bill = (totalLiters / 1000.0) * 12.0;
    } else if(totalLiters <= 15000) {
        bill = (5000 / 1000.0 * 12.0) + ((totalLiters - 5000) / 1000.0 * 18.0);
    } else {
        bill = (5000 / 1000.0 * 12.0) + (10000 / 1000.0 * 18.0) + ((totalLiters - 15000) / 1000.0 * 25.0);
    }

    printf("\nTotal Consumption: %.2f Liters\n", totalLiters);
    printf("Estimated Bill: %.2f BDT\n", bill);
    pauseConsole();
}

/* Feature 11: High Water Usage Alert */
void highUsageAlert(char loggedUser[])
{
    clearScreen();
    printHeader();
    FILE *fp = fopen("usage_records.txt", "r");
    if(fp == NULL) {
        printf("\nNo usage records found!\n");
        pauseConsole();
        return;
    }

    char user[30], date[20];
    float liters, threshold = 500.0;
    int alertFound = 0;

    printf("\n----------------------------------------------------\n");
    printf("              HIGH WATER USAGE ALERT                \n");
    printf("----------------------------------------------------\n");
    printf("Environmental Threshold: %.2f Liters/day\n\n", threshold);

    while(fscanf(fp, "%s %s %f", user, date, &liters) != EOF) {
        if(strcmp(loggedUser, user) == 0 && liters > threshold) {
            printf("[WARNING] Exceeded on %s! Used: %.2f Liters\n", date, liters);
            alertFound = 1;
        }
    }
    fclose(fp);

    if(!alertFound) printf("All usage records are within safe limits.\n");
    pauseConsole();
}

/* Feature 12: Water Saving Tips */
void waterSavingTips()
{
    clearScreen();
    printHeader();
    printf("\n----------------------------------------------------\n");
    printf("             DYNAMIC WATER SAVING TIPS              \n");
    printf("----------------------------------------------------\n");
    printf("1. Turn off taps while brushing teeth to save up to 6 L/min.\n");
    printf("2. Fix leaky pipes immediately to prevent thousands of liters in waste.\n");
    printf("3. Take shorter showers instead of full bucket baths.\n");
    printf("4. Reuse laundry water for floor cleaning or flushing.\n");
    printf("5. Monitor daily reports regularly to spot abnormal consumption spikes.\n");
    pauseConsole();
}

/* Main Function */
int main()
{
    int choice;
    int loggedIn = 0;
    char loggedUser[30] = "";

    do
    {
        clearScreen();
        printHeader();

        if(loggedIn) {
            printf(" Active Session: [%s]\n", loggedUser);
            printf("----------------------------------------------------\n");
        }

        printf("1. Registration\n");
        printf("2. Login\n");
        printf("3. Profile Management\n");
        printf("4. Add Daily Water Usage\n");
        printf("5. View Usage History\n");
        printf("6. Edit/Delete Records\n");
        printf("7. Search Water Usage Records\n");
        printf("8. Daily Report\n");
        printf("9. Monthly Report\n");
        printf("10. Water Bill Estimation\n");
        printf("11. High Water Usage Alert\n");
        printf("12. Water Saving Tips\n");
        printf("13. Exit\n");

        printf("\nEnter Choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                registration();
                break;
            case 2:
                loggedIn = login(loggedUser);
                break;
            case 3:
                if(loggedIn) profileManagement(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 4:
                if(loggedIn) addWaterUsage(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 5:
                if(loggedIn) viewUsageHistory(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 6:
                if(loggedIn) editDeleteRecords(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 7:
                if(loggedIn) searchUsageRecords(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 8:
                if(loggedIn) dailyReport(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 9:
                if(loggedIn) monthlyReport(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 10:
                if(loggedIn) waterBillEstimation(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 11:
                if(loggedIn) highUsageAlert(loggedUser);
                else { printf("\nPlease Login First!\n"); pauseConsole(); }
                break;
            case 12:
                waterSavingTips();
                break;
            case 13:
                printf("\nThank You for using Smart Water Usage Monitoring System!\n");
                break;
            default:
                printf("\nInvalid Choice!\n");
                pauseConsole();
        }

    } while(choice != 13);

    return 0;
}
