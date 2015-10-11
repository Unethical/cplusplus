/*
CIS 22C HW1:
    Revised by: Jose Sepulveda (10/1/2015)
                Linux Mint x64
                Code::Blocks 13.12 GNU GCC 4.9.2

    Original code by: Ahmed Shalan


Ahmed Shalan
CIS 22B
Winter 2015
Assignment F
Problem F1
Description of problem : This program is a modified version of Problem E3 which
						 uses a linked list to contain the cars from the file.



*/

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

enum Kind { business, maintenance, other, box, tank, flat, otherFreight, chair, sleeper, otherPassenger };
const string KIND_ARRAY[] = { "business", "maintenance", "other", "box", "tank", "flat", "otherFreight", "chair", "sleeper", "otherPassenger" };

class Car
{
protected:
    string reportingMark;
    int carNumber;
    Kind kind;
    bool loaded;
public:
    string destination;

    void setUp(string rMark, int carNum, string carKind, bool isLoaded, string dest);
    virtual void setKind(const string & tempKind);
    void output();
    Car& operator=(const Car & carB);
    friend bool operator==(const Car &carA, const Car &carB);

    Car()
    {
        setUp("", 0, "other", false, "NONE");
    }
    Car(const Car &oldCar)
    {
        setUp(oldCar.reportingMark, oldCar.carNumber, KIND_ARRAY[oldCar.kind], oldCar.loaded, oldCar.destination);
        kind = oldCar.kind;
    }
    Car(const string &reportingMark, const int &carNumber, const string &kind, const bool &loaded, const string &destination)
    {
        setUp(reportingMark, carNumber, kind, loaded, destination);
    }
    int getCarNum(){
        return carNumber;
    }
    string getRepMark(){
        return reportingMark;
    }

    virtual ~Car() {};

};

class FreightCar : public Car
{
public:
    FreightCar()
    {
        setUp("", 0, "other", false, "NONE");
    }
    FreightCar(const FreightCar &oldFreight)
    {
        setUp(oldFreight.reportingMark, oldFreight.carNumber, KIND_ARRAY[oldFreight.kind], oldFreight.loaded, oldFreight.destination);
    }
    FreightCar(const string &reportingMark, const int &carNumber, const string &kind, const bool &loaded, const string &destination)
    {
        setUp(reportingMark, carNumber, kind, loaded, destination);
    }
    void setKind(const string & stringKind);
};


class PassengerCar : public Car
{
public:
    PassengerCar()
    {
        setUp("", 0, "other", false, "NONE");
    }
    PassengerCar(const PassengerCar &oldPassenger)
    {
        setUp(oldPassenger.reportingMark, oldPassenger.carNumber, KIND_ARRAY[oldPassenger.kind], oldPassenger.loaded, oldPassenger.destination);
    }
    PassengerCar(const string &reportingMark, const int &carNumber, const string &kind, const bool &loaded, const string &destination)
    {
        setUp(reportingMark, carNumber, kind, loaded, destination);
    }
    void setKind(const string & stringKind);

};

//StringOfCars declaration for the Node class
class StringOfCars;

class Node
{
private:
    Node * next;
    Car * data;
    Node()
    {
        next = nullptr;
        data = nullptr;
    }
public:
    friend class StringOfCars;



};

class StringOfCars
{
private:
    Node * head;
    Node * tail;

public:
    StringOfCars()
    {
        head = nullptr;
        tail = nullptr;
    }

    StringOfCars(const StringOfCars & oldStringOfCars)
    {
        Node * currentNodePtr = oldStringOfCars.head;
        head = nullptr;
        tail = nullptr;

        if (oldStringOfCars.head != nullptr)
        {
            while (currentNodePtr != nullptr)
            {
                push(*currentNodePtr->data);
                currentNodePtr = (*currentNodePtr).next;
            }
        }
    }

    ~StringOfCars() {}

    void push(const Car & tempCar);
    Car* search(int carNum);
    void addCar();
    void output();
};



//Function Prototypes
void input(StringOfCars & carArray);
void buildCar(string rMark, int carNum, string carKind, bool isLoaded, string dest, StringOfCars & carArray);
void buildFreightCar(string rMark, int carNum, string carKind, bool isLoaded, string dest, StringOfCars & carArray);
void buildPassengerCar(string rMark, int carNum, string carKind, bool isLoaded, string dest, StringOfCars & carArray);

int main()
{
    StringOfCars stringOfCars1;
    input(stringOfCars1);

    StringOfCars stringOfCars2(stringOfCars1);
    stringOfCars2.output();

    cout << "Enter the car number you'd like to search: ";
    int carNumChoice;
    cin >> carNumChoice;
    cout << "\nSearch results:\n\n";

    if(stringOfCars2.search(carNumChoice) != nullptr){
        (stringOfCars2.search(carNumChoice))->output();
    }else{
        cout << "No car found.\n";
    }


    int closeWindow;
    cout << "\nEnter an integer to close the output window: ";
    cin >> closeWindow;
}

// **********************************************************
// Car Member Functions
// **********************************************************

/*************************** Car::output *****************************
Outputs the member data in a neat format
Output is a member function of the Car class
*/
void Car::output()
{
    cout << "Reporting Mark: " << reportingMark << endl;
    cout << "Car Number    : " << carNumber << endl;
    cout << "Kind          : " << KIND_ARRAY[kind] << endl;

    //Converts the loaded boolean into a string for output
    string temp;
    if (loaded == true)
        temp = "true";
    else
        temp = "false";

    cout << "Loaded        : " << temp << endl;
    cout << "Destination   : " << destination << endl;
    cout << endl;

}

/*************************** Car::setUp *****************************
Assigns the data to the member data in an object from the Car class
setUp is a member function of the Car class
*/
void Car::setUp(string rMark, int carNum, string carKind, bool isLoaded, string dest)
{
    reportingMark = rMark;
    carNumber = carNum;
    setKind(carKind);
    loaded = isLoaded;
    destination = dest;
}

/*********************** Kind Car::setKind ***************************
Sets the kind variable for the setUp function
*/
void Car::setKind(const string & stringKind)
{
    if (stringKind == "business")
        kind = business;
    else if (stringKind == "maintenance")
        kind = maintenance;
    else kind = other;
}

/************************* Car::operator= *****************************
Returns the left hand operator by refrence
operator= is an overloaded member function from the Car class
*/
Car & Car::operator=(const Car & carB)
{
    setUp(carB.reportingMark, carB.carNumber, KIND_ARRAY[carB.kind], carB.loaded, carB.destination);

    return *this;
}

/***************************** operator== **************************************
Compares two Car objects to check equivalence (same reportingMark and carNumber
operator== is a overloaded friend function from the Car class
*/
bool operator==(const Car &carA, const Car &carB)
{
    bool temp;
    if (carA.reportingMark == carB.reportingMark && carA.carNumber == carB.carNumber)
        temp = true;
    else
        temp = false;
    return temp;
}

// **********************************************************
// FreightCar Member Functions
// **********************************************************
void FreightCar::setKind(const string & stringKind)
{
    if (stringKind == "box")
        kind = box;
    else if (stringKind == "tank")
        kind = tank;
    else if (stringKind == "flat")
        kind = flat;
    else kind = otherFreight;
}
// **********************************************************
// PassengerCar Member Functions
// **********************************************************
void PassengerCar::setKind(const string & stringKind)
{
    if (stringKind == "chair")
        kind = chair;
    else if (stringKind == "sleeper")
        kind = sleeper;
    else kind = otherPassenger;
}

// **********************************************************
// StringOfCars Member Functions
// **********************************************************

/*********************** StringOfCars::addCar ***************************
This was added by Jose Sepulveda on 9/27/2015
This function adds a car from the list
*/

void StringOfCars::addCar(){
    string type, order, rMark;
    cout << "Would you like to add a:\n 1)Car\n 2)PassengerCar\n 3)FrieghtCar\n 4)Exit\n";
    int choice;
    cin >> choice;
    while(choice > 4){
        cout << "Sorry, please enter a valid input.\n";
        cin >> choice;
    }
    if(choice == 1){
        type = "Car";
    } else if(choice == 2){
        type = "PassengerCar";
    } else {
        type = "FreightCar";
    }
    order = "car11";

    cout << "Please enter the rMark: ";
    cin >> rMark;

    cout << "Please enter the car number: ";
    int carNum;
    cin >> carNum;

    cout << "In one word, what kind of car is it?: ";
    string carKind;
    cin >> carKind;

    bool isLoaded;
    isLoaded = true;

    cout << "In one word, where is it headed?: ";
    string dest;
    cin >> dest;

    Car temp(rMark, carNum, carKind, isLoaded, dest);
    push(temp);



}

/*********************** StringOfCars::search ***************************
This function was added by Jose Sepulveda(10/1/2015)
Searchs for a car using the carNum, returns ptr to found car,
or nullptr if no car is found.
*/
Car* StringOfCars::search(int carNum){
    Node * pCurr;
    if (head == nullptr){
        cout << "No cars\n";
    }else{
        pCurr = head;
        while(pCurr != nullptr){
            if(carNum == (*pCurr->data).getCarNum()){
                return &(*pCurr->data);
            }
            pCurr = (*pCurr).next;
        }
    }
    Car* temp = nullptr;
    return temp;


}



/*********************** StringOfCars::output ***************************
Outputs the data from the linked list of pointers to car objects
Prints the data saved in each car object
*/
void StringOfCars::output()
{
    Node * currentNodePtr;
    if (head == nullptr)
    {
        cout << "No cars\n";
    }
    else
    {
        currentNodePtr = head;
        int currentCarNumber = 0;
        while (currentNodePtr != nullptr)
        {
            cout << "Car Number " << ++currentCarNumber << endl;
            (*currentNodePtr->data).output();
            currentNodePtr = (*currentNodePtr).next;
        }
    }
}

/*********************** StringOfCars::push ***************************
Adds a car pointer to the list of pointers to cars
Car object accessed through constant refrence from calling function
*/
void StringOfCars::push(const Car & tempCar)
{
    Car* currentCarPtr = new Car(tempCar);
    Node* currentNodePtr = new Node;

    (*currentNodePtr).data = currentCarPtr;

    if (head == nullptr)
    {
        head = currentNodePtr;
        tail = currentNodePtr;
    }
    else
    {
        (*tail).next = currentNodePtr;
        tail = currentNodePtr;
    }




}

/****************************** input ********************************
Reads in information on the train car from a file
Information saved in temporary car object in function
*/
void input(StringOfCars & carArray)
{
    string type;
    string order;
    string rMark;
    int carNum;
    string carKind;
    bool isLoaded;
    string dest;
    ifstream inputFile;

    inputFile.open("data.txt");

    if (!inputFile)
    {
        cerr << "Error while opening the file. Exitting with code 1" << endl;
        exit(1);

    }

    while (inputFile.peek() != EOF)
    {
        inputFile >> type;
        inputFile >> order;
        inputFile >> rMark;
        inputFile >> carNum;
        inputFile >> carKind;


        //Converts the user inputted true/false string to a boolean value
        string temp;
        inputFile >> temp;
        if (temp == "true")
            isLoaded = true;
        else if (temp == "false")
            isLoaded = false;

        //Skips the white space
        while (inputFile.peek() == ' ')
            inputFile.get();

        getline(inputFile, dest);


        if (type == "Car")
            buildCar(rMark, carNum, carKind, isLoaded, dest, carArray);
        else if (type == "FreightCar")
            buildFreightCar(rMark, carNum, carKind, isLoaded, dest, carArray);
        else if (type == "PassengerCar")
            buildPassengerCar(rMark, carNum, carKind, isLoaded, dest, carArray);
    }

    inputFile.close();
}

/*********************** Car buildCar ***************************
Builds a car object and outputs the results
*/
void buildCar(string rMark, int carNum, string carKind, bool isLoaded, string dest, StringOfCars & carArray)
{
    Car obj = Car(rMark, carNum, carKind, isLoaded, dest);
    carArray.push(obj);
}

/*********************** Car buildFreightCar ***************************
Builds a freight car object and outputs the results
*/
void buildFreightCar(string rMark, int carNum, string carKind, bool isLoaded, string dest, StringOfCars & carArray)
{
    FreightCar obj = FreightCar(rMark, carNum, carKind, isLoaded, dest);
    carArray.push(obj);
}

/*********************** Car buildPassengerCar ***************************
Builds a passenger car object and outputs the results
*/
void buildPassengerCar(string rMark, int carNum, string carKind, bool isLoaded, string dest, StringOfCars & carArray)
{
    PassengerCar obj = PassengerCar(rMark, carNum, carKind, isLoaded, dest);
    carArray.push(obj);
}

/*OUTPUT(10/1/2015)

//**************First output***********************************************:

Car Number 1
Reporting Mark: CN
Car Number    : 819481
Kind          : maintenance
Loaded        : false
Destination   : NONE

Car Number 2
Reporting Mark: SLSF
Car Number    : 46871
Kind          : business
Loaded        : true
Destination   : Memphis

Car Number 3
Reporting Mark: AOK
Car Number    : 156
Kind          : other
Loaded        : true
Destination   : McAlester

Car Number 4
Reporting Mark: MKT
Car Number    : 123456
Kind          : tank
Loaded        : false
Destination   : Fort Worth

Car Number 5
Reporting Mark: MP
Car Number    : 98765
Kind          : box
Loaded        : true
Destination   : Saint Louis

Car Number 6
Reporting Mark: SP
Car Number    : 567890
Kind          : flat
Loaded        : true
Destination   : Chicago

Car Number 7
Reporting Mark: GMO
Car Number    : 7878
Kind          : otherFreight
Loaded        : true
Destination   : Mobile

Car Number 8
Reporting Mark: KCS
Car Number    : 7893
Kind          : chair
Loaded        : true
Destination   : Kansas City

Car Number 9
Reporting Mark: PAPX
Car Number    : 145
Kind          : sleeper
Loaded        : true
Destination   : Tucson

Car Number 10
Reporting Mark: GN
Car Number    : 744
Kind          : otherPassenger
Loaded        : false
Destination   : NONE

Enter the car number you'd like to search: 744

Search results:

Reporting Mark: GN
Car Number    : 744
Kind          : otherPassenger
Loaded        : false
Destination   : NONE


Enter an integer to close the output window:

//**************Second output***********************************************:
Car Number 1
Reporting Mark: CN
Car Number    : 819481
Kind          : maintenance
Loaded        : false
Destination   : NONE

Car Number 2
Reporting Mark: SLSF
Car Number    : 46871
Kind          : business
Loaded        : true
Destination   : Memphis

Car Number 3
Reporting Mark: AOK
Car Number    : 156
Kind          : other
Loaded        : true
Destination   : McAlester

Car Number 4
Reporting Mark: MKT
Car Number    : 123456
Kind          : tank
Loaded        : false
Destination   : Fort Worth

Car Number 5
Reporting Mark: MP
Car Number    : 98765
Kind          : box
Loaded        : true
Destination   : Saint Louis

Car Number 6
Reporting Mark: SP
Car Number    : 567890
Kind          : flat
Loaded        : true
Destination   : Chicago

Car Number 7
Reporting Mark: GMO
Car Number    : 7878
Kind          : otherFreight
Loaded        : true
Destination   : Mobile

Car Number 8
Reporting Mark: KCS
Car Number    : 7893
Kind          : chair
Loaded        : true
Destination   : Kansas City

Car Number 9
Reporting Mark: PAPX
Car Number    : 145
Kind          : sleeper
Loaded        : true
Destination   : Tucson

Car Number 10
Reporting Mark: GN
Car Number    : 744
Kind          : otherPassenger
Loaded        : false
Destination   : NONE

Enter the car number you'd like to search: 4

Search results:

No car found.

Enter an integer to close the output window:

*/
