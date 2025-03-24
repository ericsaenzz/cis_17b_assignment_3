/*
Eric Chang
CIS 17B - Lesson 3 Assignment
Bank Account Manager
*/
using namespace std;
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <vector>
#include <limits>
#include <fstream>

class OverdrawException : public runtime_error {
public:  
    OverdrawException(): runtime_error("!! Error Insufficient Funds. You do not have enough money to make this withdrawl !!"){}
};

class NegativeDepositException : public runtime_error {
public:  
    NegativeDepositException(): runtime_error("!! Error. You cannot deposit NEGATIVE amounts !!"){}
};

class InvalidAccountException : public runtime_error{
    public:
        InvalidAccountException(): runtime_error("Account has been terminated."){}

};

// Bank account skeleton code from Assignment 1 used
// What's changed? New Selection (5) close account and smart pointer has been added. 
//Exception handling has been reformatted
class BankAccount{
    private: 
        string accountName;
        string accountNum;
        double balance;
        bool activeACC;
    
    public:
        BankAccount(string name, double intialBalance)
        :   accountName(name), accountNum( "Acc" + to_string(rand() %10000)), balance(intialBalance), activeACC(true){ //func for random acc number
            if(intialBalance <= 0){ //check to see if initial deposit is greater than 0
                throw NegativeDepositException();
            }
        }
        void deposit(double money){
            if(!activeACC){
                throw InvalidAccountException();
            }
            if (money <= 0){
                throw NegativeDepositException();
            }
            balance += money;
        }
        void withdraw(double money){
            if(!activeACC){
                throw InvalidAccountException();
            }
            if (money<= 0){
                throw NegativeDepositException();
            }
            if (money > balance){ //check balance to withdrawl ratio
                throw OverdrawException();
            }
            balance-= money;
        }
        double currentBalance()const{
            return balance;
        }
        string getAccountName()const{
            return accountName;
        }
        string getAccountNum()const{
            return accountNum;
        }
        void closeAccount(){
            activeACC = false;
        }
};

void clearDisplay() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}
int mainMenu(){
    int choice;

    cout<<"Welcome to ERIC's Complex Error handling Banking System"<<endl;
    cout<< "\n Please make a selection.\n" <<endl;
    cout<<"1. Create an Account"<<endl;
    cout<<"2. Sign In"<<endl;
    cout<<"3. Exit"<<endl;
    cin>> choice;
    clearDisplay();
    return choice;
}

int signedInMenu(){
    int choice;
    cout<< "\n Please make a selection.\n" <<endl;
    cout<<"1. Deposit Money"<<endl;
    cout<<"2. Withdraw Money"<<endl;
    cout<<"3. Check Account Balance"<<endl;
    cout<<"4. Exit and Sign Out"<<endl;
    cout<<"5. Close Account"<<endl;
    cin>> choice;
    clearDisplay();
    return choice;
}
string getStringVerif(const string& prompt){
    string value;
    while(true){
        cout<< prompt;
    
        getline(cin,value);
        if(!value.empty()){
            return value;
        }else{
            cout<<"No input detected.."<<endl;
        }

    }
}
double getNumVal(const string & prompt){
    double value;
    while (true){
        cout<<prompt;
        if(cin >>value){
            clearDisplay();
            return value;
        }
        else {
            cout<<"Invalid Input. Please Enter a NUMBER"<<endl;
            clearDisplay();
        }
    }
}
int main() {
    srand(time(0)); 
    vector<unique_ptr<BankAccount>> accounts; 
    unique_ptr<BankAccount> account = nullptr; // Current account
    bool running = true;

    while (running) {
        int startMenu = mainMenu();

        switch (startMenu) {
            case 1: { //check to see if its valid num
                string name;
                cout << "\nEnter the account Holder's name or create a username: ";
                getline(cin, name);


                double initialBalance = getNumVal("Enter initial deposit amount: ");
                try{
                    auto account = make_unique<BankAccount>(name, initialBalance);
                    cout << "Account Successfully created! Your account number is: " << account->getAccountNum() << "\n" << endl;
                    accounts.push_back(move(account)); 
                } catch(const NegativeDepositException& e){
                    cout<< "!! Error, "<< e.what()<<"\n"<<endl;
                }
                break;
            }

            case 2: {
                if (accounts.empty()) { 
                    cout << "\n Login not found. Account for this device does not exist. Please create an account.\n" << endl;
                    break;
                }
                string inputACCnum = getStringVerif("\nPlease Enter the Account Number: ");
                bool accVerified = false;

                for (auto it = accounts.begin(); it != accounts.end(); ++it) {
                    if ((*it)->getAccountNum() == inputACCnum) {
                        account = move(*it); 
                        accounts.erase(it);
                        accVerified = true;
                        break;
                    }
                }

                if (accVerified) {
                    cout << "\n" << "Signed in as " << account->getAccountName() << "\n" << "Account Number: " << account->getAccountNum() << endl;
                    bool signedIn = true;
                    while (signedIn) {
                        int signedInChoice = signedInMenu();

                        switch (signedInChoice) {
                            case 1: { 
                                double depositMoney = getNumVal("Enter Deposit Amount: ");
                                try {
                                    account->deposit(depositMoney);
                                    cout << "Deposit Complete! New Balance: $" << account->currentBalance() << endl;
                                } catch (const NegativeDepositException& e) {
                                    cout << "!! Error, "<<e.what() << endl;
                                }catch (const InvalidAccountException& e){
                                    cout<<"Error, Invalid Input."<<e.what() << endl;
                                }
                                break;
                            }

                            case 2: { //check balance to withdrawl ratio
                                double withdrawalMoney = getNumVal("Enter Withdrawal amount: ");
                                try {
                                    account->withdraw(withdrawalMoney);
                                    cout << "Withdrawal Complete! Your Current Balance: $" << account->currentBalance() << endl;
                                } catch (const NegativeDepositException& e) {
                                    cout << "\n Error, Invalid Choice. Please Select a Number from the Menu" << endl;
                                } catch (const OverdrawException& e) {
                                    cout << "Error, Insufficient Funds, please select a different amount." << endl;
                                }catch (const InvalidAccountException& e) {
                                    cout << "Error, "<<e.what()<< endl;
                                }
                                break;
                            }

                            case 3: { 
                                cout << "Current Account Balance: $" << account->currentBalance() << endl;
                                break;
                            }

                            case 4: { 
                                signedIn = false;
                                cout << "You have Successfully signed out of ERIC'S SIMPLE BANKING!" << endl;
                                account = nullptr;
                                break;
                            }
                            case 5: {
                                string confirm;
                                bool validInput = false;

                                while(!validInput){
                                    cout << "Are you sure? Terminated accounts can no longer be recovered.(yes/no)"<<endl;
                                    getline(cin, confirm);

                                    string lowerConfirm;
                                    for(char c: confirm){
                                        lowerConfirm += tolower(c);
                                    }
                                    if(lowerConfirm == "yes"){
                                        account -> closeAccount();
                                        cout<< "\n\n>> Your account has been successfully CLOSED.. No further transactions will be allowed.\n"<< endl;
                                        signedIn = false;
                                        account = nullptr;
                                        validInput = true;
                                    }else if(lowerConfirm == "no"){
                                        cout<< "\nAccount Termination has been cancelled."<<endl;
                                        validInput = true;
                                    }else{
                                        cout<< "!! Invalid input. Please enter either 'yes' or 'no' to continue!!"<<endl;
                                    }
                                }

                                break;
                            }

                            default: { 
                                cout << "Error, Input does not match our database, please make another selection" << endl;
                                break;
                            }
                        }
                    }
                } else {
                    cout << "\n\nLogin not found." << endl;
                }
                break;
            }

            case 3: { 
                running = false;
                cout<< "Thank you for using ERIC's COMPLEX Error Handling BANKING SYSTEM."<<endl;
                break;
            }

            default: { //check to see if its valid num
                cout << "Invalid input. Please make a numeric selection from the menu." << endl;
                break;
            }
        }
    }

    return 0;
}




