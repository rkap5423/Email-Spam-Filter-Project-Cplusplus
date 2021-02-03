#include <iostream>

#include "ourvector.h"

#include <fstream>

#include <string>

using namespace std;

/*Raj Kapadia, 1-17-20, CS251 UIC
 Spamfilter project. This program takes emails inputted, whether manually one at a time or from 
 an entire text file, and check/filters it against a loaded text file of spam emails.*/



/*load function-loads a spamlist (string filename) and inputs file entries into a vector (ourvector<string> contents).
*/
void load(string& filename, ourvector<string>& contents) { 
  ifstream inFS;                                                
  string email;
  inFS.open(filename); //open spamlist
  if (!inFS.is_open()) {
    cout << "**Error, unable to open '" << filename << "'" << endl; //error message if file can't be opened

  } else {
    contents.clear(); //clear current spam vector before loading new one
    cout << "Loading '" << filename << "'" << endl;
    while (!inFS.eof()) { //loop runs until the end of the file
      inFS >> email;
      if (!inFS.fail()) {

        contents.push_back(email); //current line of file pushed into vector

      }
    }
    cout << "# of spam entries: " << contents.size() << endl;
  }

}


/*display function- displays contents of loaded spamlist, by iterating through the 
  spam vector (ourvector<string> contents)
 */
void display(ourvector<string>& contents) { 
  for (int i = 0; i < contents.size(); i++) {
    cout << contents.at(i) << endl; //prints spam emails
  }
}


/*binary search function- searches for if an inputted email (string inputEmail) is in 
   the spam list (ourvector<string>& contents). Also takes the high and low bounds of the search,
   the parsed input username and input domain, and passes by reference variables counting the number
   of emails processed and the number of non spam emails. 
   Boolean function, returns true if spam found, returns false if no spam found
 */
bool binarySearch(ourvector<string>& contents, string inputEmail, int high, int low, string inputUsername, string inputDomain, int& numProcessed, int& nonSpam){ 
  string midEmail;
  int j;
  string midUsername;
  string midDomain;
  int mid;
  while (high >= low) { //loop runs as long as the high and low bounds don't cross
    mid = (high + low) / 2;
    midEmail = contents[mid];

    for (j = 0; j < midEmail.size(); j++) {
      if (midEmail[j] == ':')                //searches for position ":" in the current email
        break;
    }
    midUsername = midEmail.substr(j + 1);
    midDomain = midEmail.substr(0, j);

    if (midEmail == inputEmail) {    //returns true if spam (current email and inputted email match)

      numProcessed++;
      return true;
    } else if (midUsername == "*" && midDomain == inputDomain) { //returns true if spam (current username is "*"
                                                                 //and current domain matched inputted domain
      numProcessed++;
      return true;
    } else if (midEmail < inputEmail) {
      low = mid + 1;
    } else if (midEmail > inputEmail) {
      high = mid - 1;
    }

  }
  nonSpam++;
  numProcessed++;      
  return false;      //if no spam found, returns false
}


/* Secondary binary search function, almost exact copy of first. It differs by modifying its search to 
   scan the current usernames, to pass Test 3 where an asterisk input email is alphabetically above the 
   middle email
 */ 
bool backupBinary(ourvector<string>& contents, string inputEmail, int high, int low, string inputUsername, string inputDomain, int& numProcessed, int& nonSpam) {
  int mid;
  string midEmail;
  int j;
  string midUsername;
  string midDomain;
  while (high >= low) {
    mid = (high + low) / 2;
    midEmail = contents[mid];

    for (j = 0; j < midEmail.size(); j++) {
      if (midEmail[j] == ':')
        break;
    }
    midUsername = midEmail.substr(j + 1);
    midDomain = midEmail.substr(0, j);

    if (midEmail == inputEmail) {

      return true;
    } else if (midUsername == "*" && midDomain == inputDomain) {

      nonSpam--; //subtracts from the "non-spam" email counter, so those emails aren't double counted
      return true;
    } else if (midUsername < inputEmail) { //Scans for the current username in the search, checks compares against inputted email
      low = mid + 1;
    } else if (midUsername > inputEmail) {
      high = mid - 1;
    }

  }

  return false;
}
                  
                  
/* Check function- checks an inputted email (string inputEmail) against a loaded spamlist. If the email is found
   on the spamlist (ourvector<string> contents), the email is printed as spam. 
 */ 
void check(string& inputEmail, ourvector<string>& contents) {
  //int mid = 0;
  int low = 0;
  int high = contents.size() - 1;
  string inputDomain;
  string inputUsername;
  string input;
  int numProcessed = 0;
  int nonSpam = 0;
  int atPos = inputEmail.find("@");
  string inUsername = inputEmail.substr(0, atPos);
  string inDomain = inputEmail.substr(atPos + 1);
  input = inDomain + ":" + inUsername; //rearranges the inputted email so its in the same format as the spamlist emails

  int i;
  int j;
  for (i = 0; i < input.size(); i++) {
    if (input[i] == ':')
      break;
  }

  inputUsername = input.substr(i + 1);
  inputDomain = input.substr(0, i);
    
  //runs preliminary binary search
  if (!binarySearch(contents, input, high, low, inputUsername, inputDomain, numProcessed, nonSpam)) { 
    //if the first binary search returns false, the second search for the asterisk triggers
    if (backupBinary(contents, input, high, low, inputUsername, inputDomain, numProcessed, nonSpam)) {
      cout << inputEmail << " is spam" << endl;
    } else {
      cout << inputEmail << " is not spam" << endl;
    }
  } else if (binarySearch(contents, input, high, low, inputUsername, inputDomain, numProcessed, nonSpam))
    cout << inputEmail << " is spam" << endl;
  else
    cout << inputEmail << " is not spam" << endl;

}

                  
/* Filter function- takes a list of emails (string file1), and checks it against a list of spam emails (ourvector<string> contents).
   It outputs all the non-spam emails to a user named file (string file2).
 */ 
void filter(ourvector<string>& contents, string& file1, string& file2) {
  ifstream inFS2;
  inFS2.open(file1);
  if (!inFS2.is_open()) { //Checks if user-inputted email list can be opened or not
    cout << "**Error, unable to open '" << file1 << "'" << endl;
    return;
  }
  int dot;
  dot = file2.find(".");
  if (file2.substr(dot) != ".txt") { //checks if user-named output file is a .txt file
    cout << "**Error, " << file2 << " is an incorrect file type" << endl;
    return;
  } else {
    ofstream outFS; //output file stream
    outFS.open(file2);

    string emailLine;
    string number;
    string listEmail;
    string desc;
    int low = 0;
    int high = contents.size() - 1;
    string input;
    int atPos;
    string inUsername;
    string inDomain;
    int numProcessed = 0;
    int nonSpam = 0;
    while (!inFS2.eof()) { //loop runs until end of email list file

      if (!inFS2.fail()) {
        inFS2 >> number;
        inFS2 >> listEmail; //extracts the email address from the current line in email list file
        getline(inFS2, desc);
        if (!inFS2.fail()) {

          atPos = listEmail.find("@");
          inUsername = listEmail.substr(0, atPos);
          inDomain = listEmail.substr(atPos + 1);
          input = inDomain + ":" + inUsername; //converts email address into spamlist email format

          if (!binarySearch(contents, input, high, low, inUsername, inDomain, numProcessed, nonSpam)) { //checks if spam or not
            if (!backupBinary(contents, input, high, low, inUsername, inDomain, numProcessed, nonSpam)) {
              outFS << number << " " << listEmail << desc << endl;
            }
          }
        }

      }
    }
    cout << "# emails processed: " << numProcessed << endl;
    cout << "# non-spam emails: " << nonSpam << endl;
    outFS.close(); //closes file

  }
}

//Main function. Where main code is run and where functions are called                
int main() {
  cout << "** Welcome to spam filtering app **" << endl; //Welcome message
  string input;
  string prefix;
  int pos;
  string command;
  string filename;
  string inputEmail;
  string inDomain;
  string inUsername;
  string file1;
  string file2;
  int atPos;
  int atPos2;
  ourvector<string> contents;
  while (input != "#") { //loop runs as long as # isn't inputted
    cout << "Enter command or # to exit> ";
    getline(cin, input);
    pos = input.find(" ");
    prefix = input.substr(0, pos);//extracts the command prefix from the input line ("load", "check", etc)
    command = input.substr(pos + 1); //stores rest of input line in a variable

    if (prefix == "load") { //calls load function
      filename = command;

      load(filename, contents);
      cout << endl;
    } else if (prefix == "display") { //calls display function
      display(contents);
      cout << endl;
    } else if (prefix == "check") { //calls check function
      inputEmail = command;

      check(inputEmail, contents);
      cout << endl;
    } else if (prefix == "filter") { //calls filter function
      atPos2 = command.find(" ");
      file1 = command.substr(0, atPos2); //extracts email list file from input line
      file2 = command.substr(atPos2 + 1); //extracts output file name from input line
      filter(contents, file1, file2);
      cout << endl;

    } else if (prefix == "#") { //ends program
      break;
    } else {
      cout << "**invalid command" << endl;
      cout << endl;
    }

  }

}