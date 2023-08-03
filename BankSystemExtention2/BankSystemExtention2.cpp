#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";
const string UserFileName = "Users.txt";


void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManagerUserMainMenue();
void LogIn();
void ShowAccessDeniedMessage();
void GoBackToMainMenue();

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6, eManageUsers = 7, eExit = 8 };

enum enManageUsers { eListUsers = 1, eAddNewUsers = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenue = 6 };

enum enMainPermissions {
    eAll = -1, pListClients = 1, pAddNewClients = 2, PDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTransaction = 32, pManageUsers = 64
};







struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};
struct sUsers {
    string UserName;
    string Password;
    int Permision;
    bool MarkForDeleteUser = false;
};



sUsers CurrentUser;

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}
string ReadUserName()
{
    string UserName = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> UserName;
    return UserName;

}

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

sUsers ConverLineToRecordForUsers(string Line, string Seperator = "#//#") {

    vector<string> vString = SplitString(Line, Seperator);
    sUsers User;

    User.UserName = vString[0];
    User.Password = vString[1];
    User.Permision = stoi(vString[2]);

    

    return User;


}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

string ConvertRecordToLineForUsers(sUsers User, string Seperator = "#//#") {

    string Line = "";

    Line += User.UserName + Seperator;
    Line += User.Password + Seperator;
    Line += to_string(User.Permision);

    return Line;
}


bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

int ReadPermissionToSet() {

    int Permission = 0;

    char Answer = 'n';


    cout << "\nDo you want to give Full Access? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {
        return -1;
    }

    cout << "\nDo you want to give Access to ?  ";

    cout << "\nShow Client List? y/n ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {
         
        Permission += enMainPermissions::pListClients;

    }


    cout << "\nShow Add Client ? y/n ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {

        Permission += enMainPermissions::pAddNewClients;

    }
    cout << "\nDelete Client ? y/n ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {

        Permission += enMainPermissions::PDeleteClient;

    }
    cout << "\nUpdate Client ? y/n ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {

        Permission += enMainPermissions::pUpdateClients;

    }

    cout << "\nFind Clients ? y/n ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {

        Permission += enMainPermissions::pFindClient;

    }

    cout << "\nTrasnsactions ? y/n ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {

        Permission += enMainPermissions::pTransaction;

    }


    cout << "\nManage Users ? y/n ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {

        Permission += enMainPermissions::pManageUsers;

    }


    return Permission;

}



bool CheckAccessPermission(enMainPermissions Permission) {

    if (CurrentUser.Permision == enMainPermissions::eAll)
        return true;

    if ((Permission & CurrentUser.Permision) == Permission)
        return true;
    else {
        return false;
    }




}


vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

vector <sUsers> LoadUserDataFromFile(string FileName) {


    fstream MyFile;
    sUsers User;
    vector<sUsers> vUser;
    MyFile.open(FileName, ios::in);

    string Line;

    if (MyFile.is_open()) {

        while (getline(MyFile, Line)) {

            User = ConverLineToRecordForUsers(Line);
            vUser.push_back(User);
        }

        MyFile.close();

    }

    

    return vUser;


}

bool IsUserNameExist(string UserName , vector<sUsers> vUsers , sUsers &User) {




    for (sUsers Usr : vUsers) {

        if (Usr.UserName == UserName) {
            User = Usr;
            return true;
        }

    }


    return false;



}



sUsers ReadNewUser() {


    sUsers User;
    vector <sUsers> vUser = LoadUserDataFromFile(UserFileName);

    cout << "Enter UserName: ";
    getline(cin >> ws, User.UserName);


    while (IsUserNameExist(User.UserName ,vUser , User )) {
        cout << "\nUser With[" << User.UserName << "] is Already Exist Please Add New User";
        getline(cin >> ws, User.UserName);

    }


    cout << "Enter Password: ";
    getline(cin, User.Password);


    User.Permision = ReadPermissionToSet();


    return User;



}


bool UserAndPassWordExist(string UserName, string Password, sUsers & user) {

    fstream MyFile;
    string line;
    sUsers User;
    vector<sUsers> vUser = LoadUserDataFromFile(UserFileName);


    for(sUsers U: vUser){
    
        if (U.UserName == UserName && U.Password == Password) {

            user = U;
            return true;
        
        
        }


    
    }


    return false;
  

}


void PrintClientRecordLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintUserRecord(sUsers User) {



    cout << "|" << setw(15) << left << User.UserName;
    cout << "|" << setw(12) << left << User.Password;
    cout << "|" << setw(21) << left << User.Permision;

}


void PrintUsersRecords() {



    vector<sUsers> vUser = LoadUserDataFromFile(UserFileName);

    cout << "\t\t User List(" << vUser.size() << ") User(s).\n";
    cout << "---------------------------------------------------------------------------------\n";
    cout << "|" << setw(15) << left << "User Name";
    cout << "|" << setw(12) << left << "Password";
    cout << "|" << setw(21) << left << "Permsion";

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (sUsers User : vUser) {

        PrintUserRecord(User);
        cout << endl;

    }

    cout << "\n---------------------------------------------------------------------------------------";


}


void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}



void ShowAllClientsScreen()
{
    
    if (!CheckAccessPermission(enMainPermissions::pListClients)) {

        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }





    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddNewClients()
{


    if (!CheckAccessPermission(enMainPermissions::pAddNewClients)) {

        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }






    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}
////////////////////////////////////////////////

void PrintUserCard(sUsers User) {

    cout << "-----------------------------------------------------------\n";
    cout << "Username    :" << User.UserName << endl;
    cout << "Password    :" << User.Password << endl;
    cout << "Permission  :" << User.Permision << endl;

}

bool MarkUserForDelete(string UserName , vector<sUsers> &vUser) {


    for (sUsers &User : vUser) {

        if (User.UserName == UserName) {
            User.MarkForDeleteUser = true;
            return true;
        }

    }


    return false;

}

vector<sUsers> SaveDataLineUserToFile(string FileName, vector<sUsers> vUser) {


    fstream MyFile;
    MyFile.open(FileName, ios::out);//OverRight
    string Line;
    sUsers User;



    if (MyFile.is_open()) {


        for (sUsers User : vUser) {

            if (User.MarkForDeleteUser == false) {


                Line = ConvertRecordToLineForUsers(User);

                MyFile << Line << endl;


            }
        }
    
        MyFile.close();
    
    }





        return vUser;

 }



sUsers UbdateUser(string UserName) {

    sUsers User;


    User.UserName = UserName;


    cout << "Password: ";
    getline(cin >> ws, User.Password);

    
     User.Permision = ReadPermissionToSet();





    return User;


}



bool DeleteUserByUserName(string UserName, vector<sUsers> &vUsers) {

    sUsers User;
    char Check = 'n';


    if (UserName == "Admin") {
        cout << "\nYou Can't Delete The Admin";

        return false;
    }


    if (IsUserNameExist(UserName, vUsers, User)) {

        PrintUserCard(User);



        cout << "\n\nAre YouSure You Want To Delete this User? ";
        cin >> Check;

        if (Check == 'y' || Check == 'Y') {

            MarkUserForDelete(UserName, vUsers);

            SaveDataLineUserToFile(UserFileName, vUsers);



            vUsers = LoadUserDataFromFile(UserFileName);

            cout << "\n\nClient Deleted Successfully.";

            return true;
        }

    



    }
    else {


        cout << "\nUser Name [" << User.UserName << "] Is Not Exist ";

        return false;


    }

    

}



bool UbdateUserByUserName(string UserName, vector<sUsers>& vUsers) {

    sUsers User;
    char Check = 'n';

    if (IsUserNameExist(UserName, vUsers, User)) {

        PrintUserCard(User);



        cout << "\n\nAre YouSure You Want To Update this User? ";
        cin >> Check;

        if (Check == 'y' || Check == 'Y') {

            for (sUsers& U : vUsers) {
                if (U.UserName == UserName) {

                   U = UbdateUser(UserName);

                }


            }



            SaveDataLineUserToFile(UserFileName, vUsers);


            cout << "\n\nClient Ubdated Succesfully";
            return true;




        }
        



    }
    else {

        cout << "\nUser Name [" << User.UserName << "] Is Not Exist ";

        return false;

    }

}







//////////////////////////////////////////////////////
// User Finctions:

void ShowAllUserScreen() {
    PrintUsersRecords();



}



void AddUser() {

    sUsers User = ReadNewUser();
    string Line = ConvertRecordToLineForUsers(User);

    AddDataLineToFile(UserFileName, Line);


}


void AddNewUsers() {

    char AddMore = 'y';

    do {

        cout << "Adding new User: \n\n";

        AddUser();

        cout << "\nUser Added Sucessfuly, do you want to add More Users? Y/N? ";
        cin >> AddMore;



    } while (toupper(AddMore) == 'Y');






}



void ShowAccessDeniedMessage() {

    cout << "\n===============================\n";
    cout << "Access Denied, \nYou Dont have Permmision Please Try Again later\n";
    cout << "\n==================================\n";

}


void ShowAddUser() {

    cout << "=======================================\n";
    cout << "\t Add User ";
    cout << "=========================================\n";

    AddNewUsers();

}


void ShowDeleteUser() {

    cout << "========================================\n";
    cout << "\t  Delete User Screen \n";
    cout << "========================================\n";


    string ReadUser = ReadUserName();

    vector<sUsers> vUser = LoadUserDataFromFile(UserFileName);

    DeleteUserByUserName(ReadUser, vUser);



}



void ShowUbdateUser() {

    cout << "-------------------------------\n";
    cout << "\t  Ubdate User Screen \n";
    cout << "--------------------------------\n";
    string ReadUser = ReadUserName();

    vector<sUsers> vUser = LoadUserDataFromFile(UserFileName);


    UbdateUserByUserName(ReadUser, vUser);


}



void ShowFindUser() {
    cout << "=========================================\n";
    cout << "\t Find User Screen \n";
    cout << "============================================\n";


    vector<sUsers> vUser = LoadUserDataFromFile(UserFileName);

    sUsers User;

    if (IsUserNameExist(ReadUserName() , vUser , User)) {
        cout << "User Name IS Exist";

    }
    else {
        cout << "User Name Mot Found";
    }


}


////////////////////////////////////////////////////

void ShowDeleteClientScreen()
{

    if (!CheckAccessPermission(enMainPermissions::PDeleteClient)) {

        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{

    if (!CheckAccessPermission(enMainPermissions::pUpdateClients)) {

        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }



    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{

    if (!CheckAccessPermission(enMainPermissions::pFindClient)) {

        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }




    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalancesScreen()
{

    ShowTotalBalances();

}


void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}
void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}

void GoBackToManagerMainMenue() {
    
    
    

    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");

    ShowManagerUserMainMenue();

}




void PerformManagerMainMenueOption(enManageUsers ManagerUserOption) {


    switch (ManagerUserOption)
    {
    case enManageUsers::eListUsers:
        system("cls");
        ShowAllUserScreen();
       GoBackToManagerMainMenue();
        break;
    case enManageUsers::eAddNewUsers:
        system("cls");
        ShowAddUser();
        GoBackToManagerMainMenue();
        break;
    case enManageUsers::eDeleteUser:
        system("cls");
        ShowDeleteUser();
        GoBackToManagerMainMenue();
        break;
    case enManageUsers::eUpdateUser:
        system("cls");
        ShowUbdateUser();
        GoBackToManagerMainMenue();
        break;
    case enManageUsers::eFindUser:
        system("cls");
        ShowFindUser();
        GoBackToManagerMainMenue();
        break;
    case enManageUsers::eMainMenue:
        system("cls");
        ShowMainMenue();
        break;

    }


}

short ReadManagerMainMenue() {

    short Number;
    cout << "Choose What Do You Want To do? [1 to 6]";

    cin >> Number;

    return Number;

}

void ShowManagerUserMainMenue() {

    if (!CheckAccessPermission(enMainPermissions::pManageUsers)) {

        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }




    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerformManagerMainMenueOption((enManageUsers)ReadManagerMainMenue());
}

short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {

        ShowMainMenue();

    }
    }

}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enMainPermissions::pTransaction)) {

        ShowAccessDeniedMessage();
        GoBackToMainMenue();
    }




    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 7]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        ShowTransactionsMenue();
        break;


    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManagerUserMainMenue();
        break;



    case enMainMenueOptions::eExit:
        system("cls");
        LogIn();
        break;
    }

}



void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] ManageUsers.\n";
    cout << "\t[8] Log out.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}



void LogIn() {


    cout << "===========================================\n";
    cout << "\t  Log in Screen \n";
    cout << "============================================\n";
    string UserName;

    string Password;


    cout << "Username: ";
    cin >> UserName;

    cout << "Password: ";
    cin >> Password;



    while (!UserAndPassWordExist(UserName, Password, CurrentUser)) {
        cout << "Invalid Username / Password ";

        cout << "\nUsername: ";
        cin >> UserName;

        cout << "Password: ";
        cin >> Password;


    }



    ShowMainMenue();
}



int main(){


    LogIn();
    system("pause>0");



    return 0;
}