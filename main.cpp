#include "input.h"      //includes custom input handling functionality
#include <iostream>     //for standard input and output operations
#include <fstream>      //for file input and output operations
#include <vector>       //for using vector container
#include <cstdlib>      //for general utilities, including random number generation
#include <ctime>        //for time functions, used for seeding random generator
#include <limits>       //for getting numeric limits
#include <string>       //for string manipulation functions
#include <regex>        //for regular expression support, used for date validation

using namespace std;

//struct for division used in challenges 11 and 12
struct Division {
    char name[10];  //larger array size to fit full division names
    int quarter;
    double sales;
};

//struct for inventoryitem used in challenges 13 and 14
struct InventoryItem {
    char description[100];
    int quantity;
    double wholesaleCost;
    double retailCost;
    char dateAdded[11];  //date format: yyyy-mm-dd
};

//function declarations for all challenges
int mainMenuOption();
void challenge8();  //dynamic array and file operations
void enterSalesData();  //challenge 11 - input and save division sales
void Challenge12();  //challenge 12 - corporate sales data input
void addNewRecord(vector<InventoryItem>& inventory);  //challenge 13 - add inventory record
void displayRecords(const vector<InventoryItem>& inventory);  //challenge 13 - display records
void changeRecord(vector<InventoryItem>& inventory);  //challenge 13 - change record
void inventoryReport();  //challenge 14 - inventory screen report
void arrayToFile(const char* filename, const vector<int>& array);
void fileToArray(const char* filename, vector<int>& array);
void writeToFile(Division divisions[][4], int numDivisions, int numQuarters, const string& filename);
bool validateDescription(const string& description);  //validate no numbers in description
bool validateDate(const string& date);  //validate date in yyyy-mm-dd format
int inputIntegerValidated(const string& prompt);  //validate integer input
double inputDoubleValidated(const string& prompt);  //validate double input

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));  //explicit cast to unsigned int
    bool running = true;
    vector<InventoryItem> inventory;  //declare inventory outside the switch block

    //main form that displays the menu
    while (running) {
        int option = mainMenuOption();
        system("cls");  //clear the screen after each selection

        switch (option) {
        case 0:
            running = false;  //exit the program
            break;
        case 8:
            challenge8();  //call challenge 8 function
            break;
        case 11:
            enterSalesData();  //call challenge 11 function
            break;
        case 12:
            Challenge12();  //call challenge 12 function
            break;
        case 13:
            addNewRecord(inventory);  //call challenge 13 function
            break;
        case 14:
            inventoryReport();  //call challenge 14 function
            break;
        default:
            cout << "\tERROR - Invalid option. Please re-enter.\n";
            break;
        }
    }
}


//main menu displaying options for challenges
int mainMenuOption() {
    cout << "\n\tCMPR121 Chapter 12: Advanced File Operations by Francisco Bumanglag";
    cout << "\n\t" << string(60, char(205));
    cout << "\n\t 8. Challenge 8 - Dynamic Array and File Operations";
    cout << "\n\t11. Challenge 11 - Input and Save Division Sales";
    cout << "\n\t12. Challenge 12 - Corporate Sales Data Input";
    cout << "\n\t13. Challenge 13 - Manage Inventory";
    cout << "\n\t14. Challenge 14 - Inventory Screen Report";
    cout << "\n\t 0. Exit";
    cout << "\n\t" << string(60, char(205));

    return inputInteger("\n\tOption: ", 0, 14);  //get the user input
}

//challenge 8: dynamic array and file operations
void challenge8() {
    system("cls");  //clear the screen
    cout << "\n\tSpecify the size of the dynamic array: ";
    int sizeOfArray = inputInteger("", 1, 1000);  //prompt for array size
    vector<int> intArray(sizeOfArray);

    //fill array with random integers
    for (int& value : intArray) {
        value = rand() % 100;  //random numbers between 0 and 99
    }

    char subMenuChoice;
    bool runningSubMenu = true;
    while (runningSubMenu) {
        //display the sub-menu for challenge 8
        cout << "\n\tChapter 12 - Challenge 8: Array/File Functions";
        cout << "\n\t" << string(60, char(205));
        cout << "\n\tDynamic Array Size: " << sizeOfArray;
        cout << "\n\t" << string(60, char(205));
        cout << "\n\tA. Write Array to File";
        cout << "\n\tB. Read Array from File";
        cout << "\n\tR. Return to Main Menu";
        cout << "\n\t" << string(60, char(205));
        cout << "\n\tOption: ";
        cin >> subMenuChoice;

        string fileName;
        switch (toupper(subMenuChoice)) {
        case 'A':
            fileName = inputString("\n\tSpecify an output binary file name to write: ", false);
            arrayToFile(fileName.c_str(), intArray);
            cout << "\n\tBinary data has been written to file: " << fileName << ".\n";
            break;
        case 'B':
            fileName = inputString("\n\tSpecify an input binary file name to read: ", false);
            fileToArray(fileName.c_str(), intArray);
            cout << "\n\tData from the file: " << fileName << " has been read into the array.\n";
            for (int num : intArray) {
                cout << "\t" << num << "\n";
            }
            break;
        case 'R':
            runningSubMenu = false;  //exit challenge 8 form
            break;
        default:
            cout << "\n\tERROR: Invalid option. Please re-enter.\n";
            break;
        }

        cout << "\n";
        system("pause");
        system("cls");  //clear the screen after each operation
    }
}

//challenge 11: input and save division sales
void enterSalesData() {
    const int numDivisions = 4;
    const char* divisionNames[] = { "East", "West", "North", "South" };
    const int numQuarters = 4;
    Division divisions[numDivisions][numQuarters];
    string filename;

    system("cls");  //clear the screen to start fresh
    cout << "\n\tEnter Sales Data by Region\n\n";

    for (int i = 0; i < numDivisions; ++i) {
        for (int j = 0; j < numQuarters; ++j) {
            cout << "\tEnter sales for " << divisionNames[i] << ", Quarter " << (j + 1) << ": ";
            do {
                divisions[i][j].sales = inputDouble("Sales ($): ", true);
                if (divisions[i][j].sales < 0) {
                    cout << "\tERROR: Please enter a non-negative number for sales.\n";
                }
            } while (divisions[i][j].sales < 0);
            strncpy_s(divisions[i][j].name, divisionNames[i], sizeof(divisions[i][j].name) - 1);
            divisions[i][j].quarter = j + 1;
        }
        cout << "\n";  //add a line between each division's sales entries
    }

    //prompt for filename and save data
    filename = inputString("\n\tEnter filename to save data: ", false);
    writeToFile(divisions, numDivisions, numQuarters, filename);

    //return to the main menu after saving data
    char returnChoice;
    do {
        cout << "\n\tEnter 'R' to return to the main menu: ";
        cin >> returnChoice;
        returnChoice = toupper(returnChoice);
    } while (returnChoice != 'R');
    system("cls");  //clear the form and return to main menu
}

//challenge 12: corporate sales data input
void Challenge12() {
    const int numDivisions = 4;
    const int numQuarters = 4;
    Division divisions[numDivisions][numQuarters];
    string filename;

    system("cls");  //clear the screen to start fresh

    //prompt for filename
    filename = inputString("\n\tEnter the filename to read the sales data: ", false);
    ifstream infile(filename, ios::in | ios::binary);
    if (!infile) {
        cout << "\n\tERROR: File " << filename << " cannot be found.\n";
        return;
    }

    //read data from file
    for (int i = 0; i < numDivisions; ++i) {
        for (int j = 0; j < numQuarters; ++j) {
            infile.read(reinterpret_cast<char*>(&divisions[i][j]), sizeof(Division));
        }
    }
    infile.close();

    //variables to store totals
    double totalCorporateSales[numQuarters] = { 0 };
    double totalYearlySales[numDivisions] = { 0 };
    double totalCorporateYearlySales = 0;
    double averageQuarterlySales = 0;
    double highestSales = numeric_limits<double>::min();
    double lowestSales = numeric_limits<double>::max();
    int highestQuarter = 0, lowestQuarter = 0;

    //calculate totals and averages
    for (int i = 0; i < numDivisions; ++i) {
        for (int j = 0; j < numQuarters; ++j) {
            totalCorporateSales[j] += divisions[i][j].sales;
            totalYearlySales[i] += divisions[i][j].sales;
        }
        totalCorporateYearlySales += totalYearlySales[i];
    }

    //find highest and lowest quarter
    for (int j = 0; j < numQuarters; ++j) {
        if (totalCorporateSales[j] > highestSales) {
            highestSales = totalCorporateSales[j];
            highestQuarter = j + 1;
        }
        if (totalCorporateSales[j] < lowestSales) {
            lowestSales = totalCorporateSales[j];
            lowestQuarter = j + 1;
        }
        averageQuarterlySales += totalCorporateSales[j];
    }

    averageQuarterlySales /= numQuarters;

    //display results
    cout << "\n\tCorporate Sales Data Summary\n";
    for (int j = 0; j < numQuarters; ++j) {
        cout << "\n\tTotal corporate sales for Quarter " << (j + 1) << ": $" << totalCorporateSales[j];
    }
    cout << "\n";
    for (int i = 0; i < numDivisions; ++i) {
        cout << "\n\tTotal yearly sales for " << divisions[i][0].name << ": $" << totalYearlySales[i];
    }
    cout << "\n\n\tTotal yearly corporate sales: $" << totalCorporateYearlySales;
    cout << "\n\tAverage quarterly sales for the divisions: $" << averageQuarterlySales;
    cout << "\n\tThe highest quarter for the corporation: Quarter " << highestQuarter << " with sales $" << highestSales;
    cout << "\n\tThe lowest quarter for the corporation: Quarter " << lowestQuarter << " with sales $" << lowestSales;

    //return to the main menu
    char returnChoice;
    do {
        cout << "\n\n\tEnter 'R' to return to the main menu: ";
        cin >> returnChoice;
        returnChoice = toupper(returnChoice);
    } while (returnChoice != 'R');
    system("cls");  //clear the form and return to main menu
}

//function to write division sales data to file
void writeToFile(Division divisions[][4], int numDivisions, int numQuarters, const string& filename) {
    ofstream outfile(filename, ios::out | ios::binary);
    if (!outfile) {
        cout << "\n\tERROR: Unable to open file for writing.\n";
        return;
    }
    for (int i = 0; i < numDivisions; ++i) {
        for (int j = 0; j < numQuarters; ++j) {
            outfile.write(reinterpret_cast<char*>(&divisions[i][j]), sizeof(Division));
        }
    }
    outfile.close();
    cout << "\n\tData successfully saved to " << filename << ".\n";
}

//challenge 13: add inventory record
void addNewRecord(vector<InventoryItem>& inventory) {
    system("cls");  //clear the screen
    InventoryItem newItem;

    string description;
    do {
        cout << "\nEnter Item Description (no numbers allowed): ";
        cin.ignore();
        getline(cin, description);
        if (!validateDescription(description)) {
            cout << "\tERROR: Description cannot contain numbers or special characters!\n";
        }
    } while (!validateDescription(description));
    strncpy_s(newItem.description, sizeof(newItem.description), description.c_str(), description.size());

    newItem.quantity = inputIntegerValidated("Enter Quantity on Hand: ");
    newItem.wholesaleCost = inputDoubleValidated("Enter Wholesale Cost: ");
    newItem.retailCost = inputDoubleValidated("Enter Retail Cost: ");

    string date;
    do {
        cout << "Enter Date Added to Inventory (YYYY-MM-DD): ";
        cin >> date;
        if (!validateDate(date)) {
            cout << "\tERROR: Invalid date format! Please enter in YYYY-MM-DD format.\n";
        }
    } while (!validateDate(date));
    strncpy_s(newItem.dateAdded, sizeof(newItem.dateAdded), date.c_str(), date.size());

    inventory.push_back(newItem);

    ofstream outfile("inventory.dat", ios::binary | ios::trunc);
    for (const auto& item : inventory) {
        outfile.write(reinterpret_cast<const char*>(&item), sizeof(InventoryItem));
    }
    outfile.close();
    cout << "\n\tRecord has been added and saved.\n";
    system("pause");
}

//challenge 13: display inventory records
void displayRecords(const vector<InventoryItem>& inventory) {
    system("cls");
    if (inventory.empty()) {
        cout << "\n\tNo records to display.\n";
    }
    else {
        for (const auto& item : inventory) {
            cout << "\nDescription: " << item.description;
            cout << "\nQuantity: " << item.quantity;
            cout << "\nWholesale Cost: $" << item.wholesaleCost;
            cout << "\nRetail Cost: $" << item.retailCost;
            cout << "\nDate Added: " << item.dateAdded << "\n";
        }
    }
    system("pause");
}

//challenge 13: change inventory record
void changeRecord(vector<InventoryItem>& inventory) {
    system("cls");
    if (inventory.empty()) {
        cout << "\n\tNo records to modify.\n";
    }
    else {
        int index;
        cout << "\nEnter index of record to change (0 to " << inventory.size() - 1 << "): ";
        cin >> index;
        if (index >= 0 && index < inventory.size()) {
            string description;
            do {
                cout << "Enter New Item Description (no numbers allowed): ";
                cin.ignore();
                getline(cin, description);
                if (!validateDescription(description)) {
                    cout << "\tERROR: Description cannot contain numbers or special characters!\n";
                }
            } while (!validateDescription(description));
            strncpy_s(inventory[index].description, sizeof(inventory[index].description), description.c_str(), description.size());

            inventory[index].quantity = inputIntegerValidated("Enter New Quantity on Hand: ");
            inventory[index].wholesaleCost = inputDoubleValidated("Enter New Wholesale Cost: ");
            inventory[index].retailCost = inputDoubleValidated("Enter New Retail Cost: ");

            string date;
            do {
                cout << "Enter New Date Added to Inventory (YYYY-MM-DD): ";
                cin >> date;
                if (!validateDate(date)) {
                    cout << "\tERROR: Invalid date format! Please enter in YYYY-MM-DD format.\n";
                }
            } while (!validateDate(date));
            strncpy_s(inventory[index].dateAdded, sizeof(inventory[index].dateAdded), date.c_str(), date.size());

            ofstream outfile("inventory.dat", ios::binary | ios::trunc);
            for (const auto& item : inventory) {
                outfile.write(reinterpret_cast<const char*>(&item), sizeof(InventoryItem));
            }
            outfile.close();
            cout << "\n\tRecord has been updated and saved.\n";
        }
        else {
            cout << "\n\tInvalid index!\n";
        }
    }
    system("pause");
}

//challenge 14: inventory screen report
void inventoryReport() {
    vector<InventoryItem> inventory;
    ifstream infile("inventory.dat", ios::binary);
    if (!infile) {
        cout << "Failed to open the inventory file.\n";
        return;
    }

    InventoryItem temp;
    while (infile.read(reinterpret_cast<char*>(&temp), sizeof(InventoryItem))) {
        inventory.push_back(temp);
    }
    infile.close();

    double totalWholesaleValue = 0.0;
    double totalRetailValue = 0.0;
    int totalQuantity = 0;

    for (const auto& item : inventory) {
        totalWholesaleValue += item.wholesaleCost * item.quantity;
        totalRetailValue += item.retailCost * item.quantity;
        totalQuantity += item.quantity;
    }

    cout << "\nTotal Wholesale Value of Inventory: $" << totalWholesaleValue;
    cout << "\nTotal Retail Value of Inventory: $" << totalRetailValue;
    cout << "\nTotal Quantity of All Items in Inventory: " << totalQuantity << "\n";
}

//function to validate no numbers in description
bool validateDescription(const string& description) {
    for (char ch : description) {
        if (isdigit(ch) || !isalpha(ch)) {
            return false;
        }
    }
    return true;
}

//function to validate date in yyyy-mm-dd format
bool validateDate(const string& date) {
    regex datePattern("^\\d{4}-\\d{2}-\\d{2}$");
    return regex_match(date, datePattern);
}

//function to validate integer input
int inputIntegerValidated(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\tERROR: Invalid input, please enter a valid integer.\n";
        }
        else {
            return value;
        }
    }
}

//function to validate double input
double inputDoubleValidated(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\tERROR: Invalid input, please enter a valid number.\n";
        }
        else {
            return value;
        }
    }
}

//functions to write and read array to/from file (used in challenge 8)
void arrayToFile(const char* filename, const vector<int>& array) {
    ofstream outFile(filename, ios::out | ios::binary);
    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return;
    }
    outFile.write(reinterpret_cast<const char*>(array.data()), array.size() * sizeof(int));
    outFile.close();
}

void fileToArray(const char* filename, vector<int>& array) {
    ifstream inFile(filename, ios::in | ios::binary);
    if (!inFile) {
        cout << "Error opening file for reading.\n";
        return;
    }
    inFile.read(reinterpret_cast<char*>(array.data()), array.size() * sizeof(int));
    inFile.close();
}
