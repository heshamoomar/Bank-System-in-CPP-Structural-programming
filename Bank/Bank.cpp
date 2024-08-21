#include <string>
#include <iostream>
#include <vector>
#include<iomanip>
#include<fstream>

using namespace std;

struct stClient
{
    string Account_Number = "";
    string PIN_CODE = "";
    string Full_Name = "";
    string Phone_Number = "";
    double Account_Balance = 0.0;
};

vector< stClient> vClients;
const string FILENAME = "clients.txt";

vector<string> SplitString(string S1, string stringDelim) {
    vector<string> vString;
    size_t pos = 0;
    string sWord;
    // Use find() function to get the position of the delimiters
    while ((pos = S1.find(stringDelim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);                                  // store the word
        if (!sWord.empty())
        {                                                           // check if the word is not empty
            vString.push_back(sWord);
        }
        S1.erase(0, pos + stringDelim.length());                    // erase() until position and move to the next word.
    }
    if (!S1.empty())
    {
        vString.push_back(S1);                                      // it adds the last word of the string.
    }
    return vString;
}

stClient convert_line_to_client_record(string line, string separator = "//") {
    stClient client;
    vector<string> vClientRecord;
    vClientRecord = SplitString(line, separator);

    client.Account_Number = vClientRecord[0];
    client.PIN_CODE = vClientRecord[1];
    client.Full_Name = vClientRecord[2];
    client.Phone_Number = vClientRecord[3];
    client.Account_Balance = stod(vClientRecord[4]);

    return client;
}

string convert_client_record_to_line(const stClient& client, string separator = "//") {
    string cRecord = "";
    cRecord += client.Account_Number;
    cRecord += separator;
    cRecord += client.PIN_CODE;
    cRecord += separator;
    cRecord += client.Full_Name;
    cRecord += separator;
    cRecord += client.Phone_Number;
    cRecord += separator;
    cRecord += to_string(client.Account_Balance);
    return cRecord;
}

void save_record_to_file(stClient& client) {

    //// Check if the file exists, if not, create it
    //if (!fileExists(FILENAME)) {
    //    std::ofstream outFile = create_file(FILENAME, "txt");
    //}

    // Open in append mode
    std::ofstream outFile(FILENAME, std::ios::app);
    outFile << convert_client_record_to_line(client) << '\n';

}

void sync_all_records_to_file() {
    std::ofstream outFile(FILENAME, std::ios::out);
    for (auto client : vClients) {
        outFile << convert_client_record_to_line(client) << '\n';
    }
}

void load_records_from_file(string FileName) {
    fstream myFile;
    myFile.open(FileName, ios::in); //  read mode

    if (myFile.is_open()) {
        string line;
        stClient client;

        while (getline(myFile, line))
        {
            client = convert_line_to_client_record(line);
            vClients.push_back(client);
        }
        myFile.close();
    }

}

bool client_exists(string Account_Number) {

    for (auto client : vClients) {
        if (client.Account_Number == Account_Number) {
            return true;
        }
    }
    return false;
}

void print_client_card(const stClient& client) {
    cout << left << setw(17) << "Account Number: ";
    cout << client.Account_Number << '\n';

    cout << left << setw(17) << "Pin Code: ";
    cout << client.PIN_CODE << '\n';

    cout << left << setw(17) << "Full Name: ";
    cout << client.Full_Name << '\n';

    cout << left << setw(17) << "Phone Number: ";
    cout << client.Phone_Number << '\n';

    cout << left << setw(17) << "Account Balance: ";
    cout << client.Account_Balance << endl;

}

void update_client() {
    cout << "-----------------------------------\n";
    cout << "\tUpdate Client Screen\n";
    cout << "-----------------------------------\n";

    string Account_Number;
    cout << "Enter account number: ";
    cin >> Account_Number;

    for (auto& client : vClients)
    {
        if (client.Account_Number == Account_Number)
        {
            cout << "-----------------------------------\n";
            print_client_card(client);
            cout << "-----------------------------------\n";

            cout << "Updating Client [" << Account_Number << "]: \n\n";

            cout << left << setw(23) << "Enter Pin Code: ";
            getline(cin >> ws, client.PIN_CODE);

            cout << left << setw(23) << "Enter Full Name: ";
            getline(cin, client.Full_Name);

            cout << left << setw(23) << "Enter Phone Number: ";
            getline(cin, client.Phone_Number);

            cout << left << setw(23) << "Enter Account Balance: ";
            cin >> client.Account_Balance;

            cout << "\nClient Updated." << endl;

            sync_all_records_to_file();
            return;
        }
    }
    cout << "Client with account number [" << Account_Number << "] Doesn't Exist.\n\n";

}

void delete_client() {
    char choice;
    cout << "-----------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "-----------------------------------\n";

    string Account_Number;
    cout << "Enter account number: ";
    cin >> Account_Number;

    for (int i = 0; i < vClients.size(); i++)
    {
        if (vClients[i].Account_Number == Account_Number) {
            cout << "-----------------------------------\n";
            print_client_card(vClients[i]);
            cout << "-----------------------------------\n";

            cout << "Are you sure, do you want to delete this client record? [y/n]: \n";
            cin >> choice;
            if (toupper(choice) == 'Y') {

                vClients.erase(vClients.begin() + i);
                sync_all_records_to_file();
                cout << "\nClient Deleted.\n";
                return;
            }
            else
            {
                return;
            }
        }
    }

    cout << "Client with account number [" << Account_Number << "] Doesn't Exist.\n\n";
}

void search_for_client() {
    cout << "-----------------------------------\n";
    cout << "\tSearch For Client Screen\n";
    cout << "-----------------------------------\n";

    string Account_Number;
    cout << "Enter account number: ";
    cin >> Account_Number;

    for (auto client : vClients)
    {
        if (client.Account_Number == Account_Number)
        {
            cout << "\nClient Found.\n";
            cout << "The following are the client details: \n";
            cout << "-----------------------------------\n";
            print_client_card(client);
            cout << "-----------------------------------\n";
            cout << endl;
            return;
        }
    }
    cout << "Client with account number [" << Account_Number << "] Not Found.\n\n";
}

stClient enter_client_data() {
    stClient client;

    cout << "Adding New Client: \n\n";

    cout << left << setw(23) << "Enter Account Number: ";
    getline(cin >> ws, client.Account_Number); // consume whitespace

    while (client_exists(client.Account_Number))
    {
        cout << "\nClient [" << client.Account_Number << "] already exists, Enter another Account Number.\n";

        cout << left << setw(23) << "Enter Account Number: ";
        getline(cin >> ws, client.Account_Number); // consume whitespace
    }

    cout << left << setw(23) << "Enter Pin Code: ";
    getline(cin, client.PIN_CODE);

    cout << left << setw(23) << "Enter Full Name: ";
    getline(cin, client.Full_Name);

    cout << left << setw(23) << "Enter Phone Number: ";
    getline(cin, client.Phone_Number);

    cout << left << setw(23) << "Enter Account Balance: ";
    cin >> client.Account_Balance;

    return client;
}

void add_clients() {
    char choice;
    cout << "-----------------------------------\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "-----------------------------------\n";

    do
    {
        stClient client = enter_client_data();
        vClients.push_back(client);

        save_record_to_file(client);
        cout << "\nClient Added Successfully, do you want to add more clients? [y/n]: \n";
        cin >> choice;
    } while (toupper(choice) == 'Y');

}

void print_client_record(const stClient& client) {
    cout << "| " << setw(15) << left << client.Account_Number;
    cout << "| " << setw(10) << left << client.PIN_CODE;
    cout << "| " << setw(40) << left << client.Full_Name;
    cout << "| " << setw(12) << left << client.Phone_Number;
    cout << "| " << setw(12) << left << client.Account_Balance;
}

void print_clients() {
    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;


    for (auto client : vClients) {
        print_client_record(client);
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

int readNumber(std::string message) {
    int num;
    std::cout << message;
    std::cin >> num;
    while (std::cin.fail())
    {
        //	user didn't input a number
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Please enter a number: ";
        std::cin >> num;
    }
    return num;
}

short main_menu_screen() {
    cout << "-----------------------------------\n";
    cout << "\tMain Menu Screen\n";
    cout << "-----------------------------------\n";
    cout << "[1] Show Client List.\n";
    cout << "[2] Add New Client.\n";
    cout << "[3] Update Client.\n";
    cout << "[4] Delete Client.\n";
    cout << "[5] Find Client.\n";
    cout << "[6] Transactions.\n";
    cout << "[7] Exit.\n";
    cout << "-----------------------------------\n";
    cout << "Choose what do you want to do, [1 to 7]?\n";

    return readNumber("");
}

bool fileExists(const std::string& filename) {	//	Check if File Exists
    std::ifstream file(filename);
    return file.good();
}

short transactions_menu_screen() {
    cout << "-----------------------------------\n";
    cout << "\tTransactions Menu Screen\n";
    cout << "-----------------------------------\n";
    cout << "[1] Deposit.\n";
    cout << "[2] Withdraw.\n";
    cout << "[3] Show Balances.\n";
    cout << "[4] Return to Main Menu.\n";
    cout << "-----------------------------------\n";
    cout << "Choose what do you want to do, [1 to 4]?\n";

    return readNumber("");
}

void deposit() {
    float amount = 0.0;
    char choice = 'n';
    cout << "-----------------------------------\n";
    cout << "\tDeposit Screen\n";
    cout << "-----------------------------------\n";

    string Account_Number;
    cout << "Enter account number: ";
    cin >> Account_Number;

    for (auto& client : vClients)
    {
        if (client.Account_Number == Account_Number)
        {
            cout << "\nClient Found.\n";
            cout << "The following are the client details: \n";
            cout << "-----------------------------------\n";
            print_client_card(client);
            cout << "-----------------------------------\n";
            cout << endl;

            cout << "Please enter deposit amount: ";
            cin >> amount;

            if (amount < 0.0) {
                while (amount < 0.0) {
                    cout << "Enter a positive amount.\n";
                    cin >> amount;
                }
            }
            cout << "Are you sure, do you want to perform this transaction? [y/n]: ";
            cin >> choice;
            if (toupper(choice) != 'Y') {
                return;
            }

            client.Account_Balance += amount;
            sync_all_records_to_file();

            cout << "Operation done successfully. New balance is ";
            cout << client.Account_Balance;
            cout << endl;
            return;
        }
    }
    cout << "Client [" << Account_Number << "] Doesn't Exist.\n\n";

}

void withdraw() {
    float amount = 0.0;
    char choice = 'n';
    cout << "-----------------------------------\n";
    cout << "\Withdraw Screen\n";
    cout << "-----------------------------------\n";

    string Account_Number;
    cout << "Enter account number: ";
    cin >> Account_Number;

    for (auto& client : vClients)
    {
        if (client.Account_Number == Account_Number)
        {
            cout << "\nClient Found.\n";
            cout << "The following are the client details: \n";
            cout << "-----------------------------------\n";
            print_client_card(client);
            cout << "-----------------------------------\n";
            cout << endl;

            cout << "Please enter withdraw amount: ";
            cin >> amount;

            if (amount < 0.0) {
                while (amount < 0.0) {
                    cout << "Enter a positive amount.\n";
                    cin >> amount;
                }
            }
            if (amount > client.Account_Balance) {
                while (amount > client.Account_Balance) {
                    cout << "\nAmount Exeeds client balance, you can withdraw up to: ";
                    cout << client.Account_Balance;
                    cout << endl;
                    cout << "Please enter another amount: ";
                    cin >> amount;
                }
            }
            cout << "Are you sure, do you want to perform this transaction? [y/n]: ";
            cin >> choice;
            if (toupper(choice) != 'Y') {
                return;
            }

            client.Account_Balance -= amount;
            sync_all_records_to_file();

            cout << "Operation done successfully. New balance is ";
            cout << client.Account_Balance;
            cout << endl;
            return;
        }
    }
    cout << "Client [" << Account_Number << "] Doesn't Exist.\n\n";


}

void balances_list() {
    double total_balances = 0.0;

    cout << "\n\t\t\t\t\tBalancees List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;


    for (auto client : vClients) {
        total_balances += client.Account_Balance;
        cout << "| " << setw(15) << left << client.Account_Number;
        cout << "| " << setw(40) << left << client.Full_Name;
        cout << "| " << setw(12) << left << client.Account_Balance;
        cout << endl;
    }
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "Total Balances = " << total_balances << endl << endl;

}

void transactions() {
    bool exit = 0;
    while (1)
    {
        system("cls");

        switch (transactions_menu_screen())
        {
        case 1:
            system("cls");
            deposit();
            system("pause");
            break;

        case 2:
            system("cls");
            withdraw();
            system("pause");
            break;

        case 3:
            system("cls");
            balances_list();
            system("pause");
            break;

        case 4:
            exit = true;
            break;

        default:
            system("cls");
            cout << "Invalid Operation.\n\n";
            system("pause");
            break;
        }

        if (exit)break;
    }
}

int main() {
    bool exit = 0;

    //  if file exists load its contents
    if (fileExists(FILENAME)) {
        load_records_from_file(FILENAME);
    }

    while (1)
    {

        system("cls");

        switch (main_menu_screen())
        {
        case 1:
            system("cls");
            print_clients();
            system("pause");
            break;

        case 2:
            system("cls");
            add_clients();
            system("pause");
            break;

        case 3:
            system("cls");
            update_client();
            system("pause");
            break;

        case 4:
            system("cls");
            delete_client();
            system("pause");
            break;

        case 5:
            system("cls");
            search_for_client();
            system("pause");
            break;

        case 6:
            system("cls");
            transactions();
            break;

        case 7:
            system("cls");
            cout << "Program Ended.\n";
            exit = true;
            break;

        default:
            system("cls");
            cout << "Invalid Operation.\n\n";
            system("pause");
            break;
        }

        if (exit)break;
    }

    return 0;
}
