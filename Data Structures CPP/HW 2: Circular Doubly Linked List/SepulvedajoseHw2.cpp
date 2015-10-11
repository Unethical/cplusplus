//    Jose Sepulveda - 10/08/2015 - Linux Mint - CodeBlocks - GCC 4.9.2
//       This program uses a circular doubly linked list in order to create
//       a primitive text manipulator.
//

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <iterator>

#include "CircDLL.h"

using namespace std;

class textManipulator{
    private:
        ifstream inFile;
        string commands;
        CircDLL<string> *list;
        int lineCount;

    public:

        textManipulator(string s);
        void updateLineCount();
        void displayLineCount();
        bool controller(string com);
        bool isNum(string s);
        string getComList();
        void saveToFile(string fileName);
        void outputForw();
        void outputBack();
        void displayHelp();
        ~textManipulator();

};


int main(){
    textManipulator notePad("input.txt");
    string commandList = notePad.getComList();
    string command;
    char choice;
    do{

        cout << "Enter a command: ";

        getline(cin, command);
        command[0] = toupper(command[0]);
        choice = command[0];
        if((commandList.find(choice) != string::npos) &&
            ((command[1] == ' ') || (command.length() == 1)))
            notePad.controller(command);




    }while(choice != 'Q');
    notePad.saveToFile("backup.txt");
    cout << "Saved to: backup.txt\n";




    return 0;
}

    // Initializes the textManipulator object with a text file.
    textManipulator::textManipulator(string s){
        commands = "TFBILDSHAQ";
        list = new CircDLL<string>;
        inFile.open(s, fstream::in);
        string line;
        if(inFile){
            int i = 0;
            while(getline(inFile, line)){
                list->insertNode(line, i);
                i++;
            }
            lineCount = list->getCount();
            inFile.close();
        }


    }

    // Returns the line count
    void textManipulator::updateLineCount(){ lineCount = list->getCount(); }

    // The main controller for the textManipulator
    bool textManipulator::controller(string com){
        istringstream iss(com);
        vector<string> tokens{istream_iterator<string>{iss},
                                istream_iterator<string>{}};
        if(tokens[0] == "T"){
            displayLineCount();
            return true;
        }
        if(tokens[0] == "F"){ outputForw(); return true;}
        if(tokens[0] == "B"){ outputBack(); return true;}
        if(tokens[0] == "I"){
            updateLineCount();
            if(isNum(tokens[1])){
                int lineNum = atoi(tokens[1].c_str());
                if(lineNum > lineCount)
                    lineNum = lineCount+1;
                if(lineNum <= 0)
                    lineNum = 1;
                tokens.erase(tokens.begin(), tokens.begin()+2);
                string line = "";
                for(auto s: tokens){
                    line += s;
                    line += " ";
                }
                list->insertNode(line, lineNum - 1);

                updateLineCount();
                return true;
            }
            return false;

        }
        if(tokens[0] == "L"){
            if(tokens.size() == 2){
                if(isNum(tokens[1])){
                    int temp;
                    temp = atoi(tokens[1].c_str());
                    if((temp > 0) && (temp <= lineCount)){
                        cout << setw(4) << temp << " | ";
                        list->print(temp);
                        cout << "\n";
                        return true;
                    }
                    return false;
                }
            }
            if((tokens.size() >= 3) && (isNum(tokens[1])) && (isNum(tokens[2]))){
                int rangeA = atoi(tokens[1].c_str());
                int rangeB = atoi(tokens[2].c_str());
                if(rangeA > rangeB){
                    if(rangeA > lineCount)
                        rangeA = lineCount;
                    for(int i = rangeA; i >= rangeB; i--){
                        cout << setw(4) << i << " | ";
                        list->print(i);
                        cout << "\n";
                    }
                    return true;
                }
                if(rangeB > lineCount)
                    rangeB = lineCount;
                for(int i = rangeA; i <= rangeB; i++){
                    cout << setw(4) << i << " | ";
                    list->print(i);
                    cout << "\n";
                }
                return true;


            }else{
                return false;
            }
        }
        if(tokens[0] == "D"){
            if(isNum(tokens[1])){
                int lower = atoi(tokens[1].c_str());
                if((tokens.size() == 2) || (!isNum(tokens[2]))){
                    if((lower > 0) && (lower <= lineCount)){
                        list->deleteNode(lower);
                        updateLineCount();
                        return true;
                    }else{
                        cout << "Invalid, deletion was rejected. Please enter a number within bounds.\n";
                        return false;
                    }
                }
                if(isNum(tokens[2])){
                    int upper;
                    int diff = -1;
                    if(atoi(tokens[1].c_str()) >= atoi(tokens[2].c_str())){
                        lower = atoi(tokens[2].c_str());
                        upper = atoi(tokens[1].c_str());
                    }else{
                        upper = atoi(tokens[2].c_str());
                        lower = atoi(tokens[1].c_str());
                    }
                    if(upper > lineCount)
                        upper = lineCount;
                    diff = upper - lower;
                    for(int i = 1; i <= diff+1; i++){
                        list->deleteNode(lower);
                    }
                    updateLineCount();
                    return true;
                }
            }

        }
        if(tokens[0] == "S"){
            saveToFile(tokens[1]);
        }
        if(tokens[0] == "H"){
            displayHelp();

        }
        if(tokens[0] == "A"){
            cout << setw(10) << "Author | " << "Jose Sepulveda\n";
            cout << setw(10) << "Class | " << "CIS22C: Data Structures Fall 2015\n";
            cout << setw(10) << "College | " << "De Anza College\n";
        }
    }

    // Returns ture if string is composed of nothing but digits
    bool textManipulator::isNum(string s){
        for(char c : s){
            if(!isdigit(c)){
                return false;
            }
        }
        return true;
    }

    //Returns a string containing all valid commands for the textManipulator
    string textManipulator::getComList(){
        return commands;
    }

    //Saves the contents of the textManipulator to the filename argument
    void textManipulator::saveToFile(string fileName){
        ofstream outFile;
        if(fileName.find(".txt") == string::npos)
            fileName += ".txt";
        outFile.open((fileName));

            string s;
            for(int i=1; i <=lineCount; i++){
                list->getData(s, i);
                outFile << setw(4) << i << " | ";
                outFile << s;
                outFile << "\n";
            }

            outFile.close();



    }

    //Displays the contents of textManipulator forwards
    void textManipulator::outputForw(){
        list->print();

    }

    //Displays the contents of textManipulator backwards
    void textManipulator::outputBack(){
        list->printReversed();
    }

    //Displays the textManipulator help menu
    void textManipulator::displayHelp(){
        cout << "\n  " << "T\n\t// Display the total number of lines in the text.";
        cout << "\n\n  " << "F\n\t// Print all lines from first to last";
        cout << "\n\n  " << "B\n\t// Print all lines from last to first";
        cout << "\n\n  " << "I <line #> <text>\n\t // Insert new line in the provided line number";
        cout << "\n\n  " << "L <line #1> <line #2>\n\t // List all lines between #1 and #2 inclusively.";
        cout << "\n\n  " << "L <line #> \n\t // Display the given line.";
        cout << "\n\n  " << "D <line #1> <line #2>\n\t // Delete all lines between #1 and #2 inclusively.";
        cout << "\n\n  " << "D <line #> \n\t // Delete the given line.";
        cout << "\n\n  " << "S <output file name>\n\t // Saves progress to output file name provided.";
        cout << "\n\n  " << "H \n\t// Display help.";
        cout << "\n\n  " << "A \n\t// Display author information.";
        cout << "\n\n  " << "Q \n\t// Quit edditing the file.(autosaves to backup.txt)\n";

    }


    //Displays the number of lines in the textManipulator
    void textManipulator::displayLineCount(){
        cout << lineCount << " lines on record.\n";
    }

    //Destructor, frees the list.
    textManipulator::~textManipulator(){
        delete list;
    }
