#include <iostream>
#include <vector>
#include <sstream>
#include "Markup.h"

using namespace std;

struct User
{
    int userId=0;
    string name="",surname="", login="",password="";
};

int StringToIntegerConvers(string number)
{
    int numberInt;
    istringstream iss(number);
    iss >> numberInt;
    return numberInt;
}

int getUsersFromFile(vector<User> &theUsers)
{
    User user;
    int UserId=0, lastUserId=0;
    string strUserId="", strLogin="", strPassword="", strUserName="", strUserSurname="";
    CMarkup xml;
    bool fileExists = xml.Load( "users.xml" );
    xml.ResetPos();                             //NAGLOWERK DOKUMENTU
    xml.FindElem();                             //ZNAJDZ ELEMENT "Users"
    xml.IntoElem();                             //PRACUJ NA GALEZI: "Users"

    while (xml.FindElem("User"))                //SCZYTUJ W PETLI KOLEJNE PORCJE DANYCH "User"
    {
        xml.IntoElem();
        xml.FindElem("UserId");
        strUserId=xml.GetData();
        UserId=StringToIntegerConvers(strUserId);
        xml.FindElem("Login");
        strLogin=xml.GetData();
        xml.FindElem("Password");
        strPassword=xml.GetData();
        xml.FindElem("User_name");
        strUserName=xml.GetData();
        xml.FindElem("User_surname");
        strUserSurname=xml.GetData();
        xml.OutOfElem();

        user.userId=UserId;                     //ZAPISZ UZYSKANE DANE Z PLIKU DO WEKTORA
        user.login=strLogin;
        user.password=strPassword;
        user.name=strUserName;
        user.surname=strUserSurname;

        theUsers.push_back(user);
        lastUserId=UserId;
    }
    return lastUserId;                          //ZWROC OSTATNI ODCZYTANY NUMER ID
}

void showUserData(User user)
{
    cout<<endl<<"ID:           "<<user.userId<<endl;
    cout<<"LOGIN:              "<<user.login<<endl;
    cout<<"NAME:               "<<user.name<<endl;
    cout<<"SURNAME:            "<<user.surname<<endl;
    cout<<"PASSWORD:           "<<user.password<<endl;
}

void showAllUsers(vector<User> &theUsers)
{
    cout<<"Active users"<<endl;
    cout<<"-----------------------------------------------"<<endl;
    for (vector<User>:: iterator itr=theUsers.begin(); itr!=theUsers.end(); itr++)
    {
        showUserData(*itr);
    }
    cout<<"-----------------------------------------------"<<endl;
    cout<<endl;
    system("pause");
}

void saveUserInXML(User user)
{
    CMarkup xml;
    bool fileExists = xml.Load( "users.xml" );      //WCZYTAJ PLIK "users.xml"

    if (!fileExists)
    {
        xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
        xml.AddElem("Users");
    }
    xml.FindElem();                                 //ZNAJDZ ELEMENT "Users"
    xml.IntoElem();                                 //PRACUJ NA GALEZI: "Users"
    xml.AddElem("User");                            //DODAJ ELEMENT "User"
    xml.IntoElem();                                 //PRACUJ NA GALEZI: "User"
    xml.AddElem("UserId", user.userId);             //DODAJ ELEMENT "UserId"
    xml.AddElem("Login", user.login);               //DODAJ ELEMENT "Login"
    xml.AddElem("Password", user.password);         //DODAJ ELEMENT "Password"
    xml.AddElem("User_name", user.name);            //DODAJ ELEMENT "User_name"
    xml.AddElem("User_surname", user.surname);      //DODAJ ELEMENT "User_surname"
    xml.Save("users.xml");
}

int creatingUsers(vector<User> &theUsers, int theHighestId)
{
    User user;
    int userId=theHighestId+1;
    string name, surname, login, password;
    cout<<"Please, enter your name:"<<endl;
    cin>>name;
    cout<<"Enter your surname:"<<endl;
    cin>>surname;
    cout<<"Enter a login you want to create:"<<endl;
    cin>>login;
    cout<<"Enter a password:"<<endl;
    cin>>password;
    user.userId=userId;
    user.name=name;
    user.surname=surname;
    user.login=login;
    user.password=password;

    theUsers.push_back(user);                           //ZAPIS NOWEGO UZYTKOWNIKA DO WEKTORA
    saveUserInXML(user);                                //ZAPIS NOWEGO UZYTKOWNIKA DO PLIKU .XML
    return userId;
}

void editPasswordInFile(int editUserId, string newPassword)
{
    string strUserId="";
    int UserId=0;
    CMarkup xml;
    bool fileExists = xml.Load( "users.xml" );
    xml.ResetPos();                                     //NAGLOWEK DOKUMENTU
    xml.FindElem();                                     //ZNAJDZ ELEMENT "USERS"
    xml.IntoElem();                                     //PRACUJ NA TEJ GALEZI
    while (xml.FindElem("User"))
    {
        xml.IntoElem();
        xml.FindElem("UserId");
        strUserId=xml.GetData();
        UserId=StringToIntegerConvers(strUserId);
        if(UserId==editUserId)
        {
            xml.FindElem("Password");
            xml.RemoveElem();
            xml.AddElem("Password", newPassword);
            break;
        }
        xml.OutOfElem();
    }
    xml.Save("users.xml");
}

void upadeXMLFile(vector<User> &theUsers)
{
    //CZYSZCZENIE STARYCH DANYCH W PLIKU XML
    CMarkup xml;
    bool fileExists = xml.Load( "users.xml" );
    xml.ResetMainPos();
    xml.FindElem();                                     //ZNAJDZ ELEMENT "USERS"
    xml.IntoElem();                                     //PRACUJ NA TEJ GALEZI
    while (xml.FindElem("User"))
    {
        xml.RemoveElem();
    }
    xml.Save("users.xml");
    //WYPELNIANIE PLIKU XML AKTUALNYM WEKTOREM
    User user;
    xml.ResetPos();
    xml.FindElem();                                     //ZNAJDZ ELEMENT "Users"
    xml.IntoElem();                                     //PRACUJ NA TEJ GALEZI
    for (vector<User>:: iterator itr=theUsers.begin(); itr!=theUsers.end(); itr++)
    {
        user=*itr;
        xml.AddElem("User");                            //DODAJ ELEMENT "User"
        xml.IntoElem();                                 //PRACUJ NA GALEZI: "User"
        xml.AddElem("UserId", user.userId);             //DODAJ ELEMENT "UserId"
        xml.AddElem("Login", user.login);               //DODAJ ELEMENT "Login"
        xml.AddElem("Password", user.password);         //DODAJ ELEMENT "Password"
        xml.AddElem("User_name", user.name);            //DODAJ ELEMENT "User_name"
        xml.AddElem("User_surname", user.surname);      //DODAJ ELEMENT "User_surname"
        xml.OutOfElem();                                //WYJDZ Z ELEMENTU "User"
    }
    xml.Save("users.xml");
}

void changeUserPassword(vector<User> &theUsers)
{
    int logedUserId=0;
    string newPassword = "";
    cout<<"Enter user ID: ";
    cin>>logedUserId;
    cout<<endl;
    cout<<"Enter new password: ";
    cin>>newPassword;
    cout<<endl;
    for (vector <User>::iterator itr = theUsers.begin(); itr != theUsers.end(); itr++)
    {
        if(itr->userId==logedUserId)
        {
            itr ->password=newPassword;
            cout << "Password has been changed." << endl << endl;
            system("pause");
        }
    }
    editPasswordInFile(logedUserId, newPassword);
}

char choiceOptionFromMenu()
{
    char choice;
    system("cls");
    cout << "    >>>MAIN MENU<<<" << endl;
    cout << "---------------------------" << endl;
    cout << "1. Add new user" << endl;
    cout << "2. Show all users" << endl;
    cout << "3. Change password" << endl;
    cout << "9. Exit" << endl;
    cout << "---------------------------" << endl;
    cout << "Your choice: ";
    cin>>choice;

    return choice;
}

int main()
{
    vector<User> theUsers;
    int lastUserId=0;
    char choice;
    lastUserId=getUsersFromFile(theUsers);
    while(true)
    {
        choice=choiceOptionFromMenu();

        switch (choice)
            {
            case '1':
                lastUserId=creatingUsers(theUsers, lastUserId);
                break;
            case '2':
                showAllUsers(theUsers);
                break;
            case '3':
                changeUserPassword(theUsers);
                break;
            case '9':
                exit(0);
                break;
            default:
                cout << endl << "...No option..." << endl << endl;
                system("pause");
                break;
            }
    }
    return 0;
}
