// Password Manager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Account{
    string accountName;
    string password;
};
string xorEncryptDecrypt(string& data, string& key) {
    string result = data;
    for (int i = 0; i < data.size(); ++i) {
        result[i] ^= key[i % key.size()];
    }
    return result;
}

int loadPasswordData(string& filename, Account accounts[], string& masterkey) {
    ifstream file(filename);
    if (!file) {
        cout << "No password database found. Starting fresh.\n";
        return 0; // error
    }

    string encryptedData;
    getline(file, encryptedData);
    string decryptedData = xorEncryptDecrypt(encryptedData, masterkey);

    
    int numAccounts = 0;
    size_t pos = 0;
    while ((pos = decryptedData.find(';')) != string::npos && numAccounts < 100) {
        string pair = decryptedData.substr(0, pos);
        size_t commaPos = pair.find(',');
        if (commaPos != string::npos) {
            accounts[numAccounts].accountName = pair.substr(0, commaPos);
            accounts[numAccounts].password = pair.substr(commaPos + 1);
            numAccounts++;
        }
        decryptedData.erase(0, pos + 1); // throw away the prev data
    }

    return numAccounts;
}

void savePasswordData(string& filename, Account accounts[], int numAccounts, string& masterkey) {
    string data;
    for (int i = 0; i < numAccounts; ++i) {
        data += accounts[i].accountName + "," + accounts[i].password + ";"; // Format: account,password;
    }
    string encryptedData = xorEncryptDecrypt(data, masterkey);

    ofstream file(filename);
    file << encryptedData;
    file.close();
}

void addPassword(Account accounts[], int& numAccounts) {
    if (numAccounts >= 100) {
        cout << "Maximum number of accounts reached!\n";
        return;
    }

    cout << "Enter account name: ";
    cin >> accounts[numAccounts].accountName;
    cout << "Enter password: ";
    cin >> accounts[numAccounts].password;
    numAccounts++;
    cout << "Password added successfully!\n";
}

void displayAccounts(Account accounts[], int numAccounts) {
    if (numAccounts == 0) {
        cout << "No accounts stored.\n";
        return;
    }

    cout << "Stored accounts:\n";
    for (int i = 0; i < numAccounts; ++i) {
        cout << "Account: " << accounts[i].accountName << '\n';
    }
}

void getPassword(Account accounts[], int numAccounts) {
    string account;
    cout << "Enter account name: ";
    cin >> account;

    for (int i = 0; i < numAccounts; ++i) {
        if (accounts[i].accountName == account) {
            cout << "Password for " << account << ": " << accounts[i].password << '\n';
            return;
        }
    }

    cout << "Account not found.\n";
}


int main()
{
    string filename = "./TextFile1";
    Account accounts[100];
    int numAccounts = 0;
    string masterkey;
    cout << "Enter a Master Password: ";
    cin >> masterkey;

    int choice;

    do {
        cout << "\n1. Add a new password\n2. Retrieve a password\n3. Display all accounts\n4. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        switch (choice) {
            case 1:
                addPassword(accounts, numAccounts);
                savePasswordData(filename, accounts, numAccounts, masterkey);
                break;
            case 2:
                getPassword(accounts, numAccounts);
                break;
            case 3:
                displayAccounts(accounts, numAccounts);
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }



    } while (choice != 4);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
