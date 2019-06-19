#include <iostream>
#include <cctype>       // For the letter checking functions
#include <fstream>      // For file input
#include <vector>
#include <cstring>      // For strlen()
#include <cstdlib>      // For rand()
#include <algorithm>    // For find()
using namespace std;

//---------------------------------------------------------------------
void getInputWords( vector<string> &allWords)   // List of all the words in the selected input file
{
    // Prompt for and get user choice for which data file to use
    cout << "Menu Options: \n"
         << "  1. Use Trump tweets \n"
         << "  2. Use Dalai Lama tweets \n"
         << "  3. Use Mixed Trump / Dalai Lama tweets \n"
         << "  4. Use small test file \n"
         << "  5. Use tiny test file \n"
         << "Your choice: ";
    int dataSelectionOption;
    cin >> dataSelectionOption;
    
    ifstream inputFileStream;  // declare the input file stream
    // open input file and verify
    switch( dataSelectionOption) {
        case 1: inputFileStream.open("TrumpTweets.txt"); break;
        case 2: inputFileStream.open("DalaiLamaTweets.txt"); break;
        case 3: inputFileStream.open("TrumpLama.txt"); break;
        case 4: inputFileStream.open("Test.txt"); break;
        case 5: inputFileStream.open("tiny.txt"); break;
        default: cout << "Invalid option chosen, exiting program. ";
            exit( -1);
            break;
    }
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file.  Exiting..." << endl;
        exit( -1);
    }
    
    char c = ' ';
    char inputWord[ 81];
    
    // Read a character at a time from the input file, separating out individual words.
    inputWord[ 0] = '\0';
    int index = 0;
    while( inputFileStream.get(c)) {
        if( ! isprint( c) && c != '\r' && c != '\n') {
            continue;   // Skip non-printable characters and get the next one
        }
        if( c != ' ') {
            // If it is not a return or newline, add it to the string.
            // If it is a return or newline character, only add it if there
            //    are already other characters in the string.
            if( (c != '\r' && c != '\n') ||
                ( (c == '\r' || c == '\n') && index > 0)
              ) {
                inputWord[ index++] = c;
            }
        }
        // End the word when encountering a space or a return character.
        if( c == ' ' || c == '\r' || c == '\n'){
            // Null terminate the input word. Store it if its length is > 0 and it is printable.
            inputWord[ index] = '\0';
            if( strlen( inputWord) > 0 && isprint( inputWord[0])) {
                allWords.push_back( string( inputWord));
            }
            // Check for special case where there is a space at the end of the line.  We don't want
            // to lose the end of line character ('\n' or '\r'), so we concatenate it to the
            // last word that was previously stored on the list.  First check to see if this is the case:
            if( (c == '\r' || c == '\n') && strlen( inputWord) == 0) {
                long indexOfLastWord = allWords.size() - 1;
                allWords.at( indexOfLastWord).append("\r");
            }
            
            index = 0;    // Restart the index for the next word
        }
    }//end while( inputFileStream...)
}


//---------------------------------------------------------------------
void checkWords( vector<string> startWords,  // List of first words in sentences
                 vector<string> allWords,    // All words from input file
                 vector<string> wordList,    // List of all words following search phrase
                 string nextWord)            // Nexts word found after search phrase
{
    int debugMenuOption = 0;
    
    do {
        // Prompt for and get user choice for which debug option to use
        cout << " \n"
             << "    >>> Debug menu options: <<<\n"
             << "        1. Display a start word \n"
             << "        2. Display one of all words \n"
             << "        3. Display wordWindow and next words \n"
             << "        4. Exit debugging \n"
             << "    Your choice -> ";
        cin >> debugMenuOption;
        
        int wordIndexValue = 0;    // Used to read user input for debug options below
        
        switch( debugMenuOption) {
            case 1: cout << "    Enter an index value from 0 to " << startWords.size()-1 << " to display a start word: ";
                    cin >> wordIndexValue;
                    cout << "    " << startWords.at( wordIndexValue) << endl;
                    break;
            case 2: cout << "    Enter an index value from 0 to " << allWords.size()-1 << " to display one of all words: ";
                    cin >> wordIndexValue;
                    cout << "    " << allWords.at( wordIndexValue) << endl;
                    break;
            case 3: cout << "    WordWindow and next words are: ";
                    for( int i=0; i<wordList.size(); i++) {
                        cout << wordList.at( i) << " ";
                    }
                    cout << nextWord <<endl;
                    break;
        }//end switch(...)
    }while( debugMenuOption != 4);
    
}//end debug()
// *** Arvan: Write more meaningful comments.

void getStartWords(vector<string> &startWords, vector<string> allWords)
{
  startWords.push_back(allWords.at(0));
  for (int j = 1; j < allWords.size(); ++j) {
    char c = (allWords.at(j-1)).at((allWords.at(j-1)).size() - 1);
    if ((c == '\r') || (c == '\n')) {
      startWords.push_back(allWords.at(j));
    }
  }
}
// *** Arvan: Write more meaningful comments.

void vectorSearch (vector<string> allWords, vector<string> &nextWords, vector<string> searchVals)
{
    for (int j = 0; j < allWords.size(); ++j) {
      bool foundOne = true;
      for (int k = 0; k < searchVals.size(); ++k) {
        if ((allWords.at(j+k)).compare(searchVals.at(k)) != 0) {
          foundOne = false;
          break;
        }
      }
      if (foundOne) {
        nextWords.push_back(allWords.at(j+searchVals.size()));
      }
    }
}


//---------------------------------------------------------------------
int main()
{
    vector<string> allWords;   // List of all input words
    vector<string> startWords; // List of words that start sentences, to use
                               //   in starting generated tweets.

    cout << "Welcome to the tweet generator. " << endl << endl;
    
    getInputWords( allWords);
    // You must write the function shown below, and call it from here:
    
    getStartWords( startWords, allWords);
    
    string nextWord;
    
    // Allow looping to give more than one tweet
    int wordWindowWidth = 0;
    char userInput = ' ';
    bool DEBUG_MODE = false;
    
    // Main loop
    while( userInput != 'X') {
        cout << endl;
        cout << "Enter x to exit, or d/c to debug/continue followed by word window size: ";
        cin >> userInput;
        userInput = toupper( userInput);    // fold user input into upper case
        
        if( userInput == 'X') {
            break;   // Exit program
        }
        // Also check for input of 'D' to debug
        if (userInput == 'D')
          DEBUG_MODE = true;
        
        // Read in the wordWindowWidth to be used in both the "continue" and the "debug" modes
        cin >> wordWindowWidth;

        vector< string> wordList;   // Stores the list of consecutive words that are searched for
        
        // Find a random starting word in the startWords list
        nextWord = startWords.at(rand() % startWords.size());
      
        // Find the selected startWord within the allWords list, and remember its position
        vector<string>::iterator it = find (allWords.begin(), allWords.end(), nextWord);
        
        // Store the chosen number of consecutive starting words
        for (int i = 0; i < wordWindowWidth; ++i) {
          wordList.push_back(*(it + i));
        }
        
        int wordCounter = 0;
        while( true) {
            // Search for adjacent wordWindow words in allWords and find all the words that
            // follow the sequence of that word sequence.  Choose one at random and call it nextWord.
            vector< string> nextWords;        // List of next words.
            vectorSearch (allWords, nextWords, wordList);
            nextWord = nextWords.at(rand() % nextWords.size());
            
            // If in checkWordsMode then allow displaying single word values using the checkWords(...) function, then exit the program.
            if (DEBUG_MODE) {
              checkWords(startWords, allWords, wordList, nextWord);
              exit(-1);
            }
            
            // Display the first word in the wordWindow
            cout << " " << wordList.at( 0);
            wordCounter++;
            
            // If the first word in the wordWindow ends in a return, then we're done with our output for this tweet.
            char c = (wordList.at(0)).at((wordList.at(0)).size() - 1);
            if ((c == '\r') || (c == '\n'))
              break;
            
            // If there have been >= 30 words and the word ends with '.', '.' or '!' or '?', then we're done with output for this tweet
            if ((wordCounter >= 30) && ((c == '.') || (c == '!') || (c == '?')))
              break;
            
            // Shift words "left" to prepare for next round, shifting nextWord into the "right" of the wordWindow words.
            wordList.erase(wordList.begin());
            wordList.push_back(nextWord);
            
        }//end while( true)
        
    }// end while( userInput != 'x')
    
    cout << endl << endl;
    
    return 0;
}//end main()
