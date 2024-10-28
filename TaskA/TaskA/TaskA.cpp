// See bottom of main
//int findArg(int argc, char* argv[], string pattern);

/*
 *
 * The user can launch application as follows:
 *
 * TaskA <filename> <search term> [-regex]
 *
 * <database file>              REQUIRED. Specifies the file to search (required). This is ALWAYS the first parameter. The file must exist in order to be searched
 * <search term>                REQUIRED. The search term as either a single word or a regular expression.
                                This is a single word, made up of alpha-numeric characters only.
 * -regex                       OPTIONAL. If this flag is present, then the search term is a regular expression (as opposed to a single word).
                                It must be a valid regex expression.
 *
 * ****************
 * *** EXAMPLES ***
 * ****************
 *
 * TaskA lorum.txt comp1000             Searches for the string comp1000 in the file lorum.txt
 * TaskA lorum.txt "^(\\d)\\d" -regex   Searches the file lorum.txt for all patterns that match the regular expression "^(\\d)\\d"
 * TaskA lorum.txt -regex "^(\\d)\\d"   Searches the file lorum.txt for all patterns that match the regular expression "^(\\d)\\d"
 * TaskA lorum.txt                      Error - search expression provided
 *
 * *************
 * *** NOTES ***
 * *************
 *
 * o Try to write your code such that is can be reused in other tasks.
 * o Code should be consistently indented and commented
 * o Consider error conditions, such as missing parameters or non-existent files
*/

//int main(int argc, char* argv[])
//{
    // argv is an array of strings, where argv[0] is the path to the program, argv[1] is the first parameter, ...
    // argc is the number of strings in the array argv
    // These are passed to the application as command line arguments
    // Return value should be EXIT_FAILURE if the application exited with any form of error, or EXIT_SUCCESS otherwise

    //if (argc == 3) {
        //Welcome message
        //cout << "TaskA (c)2024" << endl;

        //BASIC EXAMPLE: Get parameters for the simple case
        //string fileName(argv[1]);
        //string searchString(argv[2]);

        //Confirm
       // cout << "TaskA " << fileName << " " << searchString << endl;

        //Done
       // return EXIT_SUCCESS;
   // }

    //EXAMPLE: Scan command line for -regex switch
   // int p = findArg(argc, argv, "-regex");
   // if (p) {
       // cout << "The search term is a regular expression. See https://www.softwaretestinghelp.com/regex-in-cpp/ for examples of how to do this " << endl;
   // }

    //**************************************************************
    //You could continue here :)
    //**************************************************************

// Include necessary libraries
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <string>
using namespace std;

// Task A1: File Reading - FileSearcher class definition
class FileSearcher {
public:
    // Constructor to initialize FileSearcher with filename, search term, and regex flag
    FileSearcher(const string& filename, const string& searchTerm, bool isRegex)
        : m_filename(filename), m_searchTerm(searchTerm), m_isRegex(isRegex) {}

    // Task A1: Method to read file contents
    bool readFile() {
        ifstream file(m_filename); // Open the file
        if (!file.is_open()) { // Check if file opened successfully
            cout << "Error: Unable to open file " << m_filename << endl;
            return false;
        }
        string line;
        while (getline(file, line)) { // Reads file line by line
            m_lines.push_back(line); // Stores each line in the vector
        }
        file.close(); // Closes the file
        return true;
    }

    // Getter methods
    const vector<string>& getLines() const { return m_lines; }
    const string& getSearchTerm() const { return m_searchTerm; }
    bool isRegex() const { return m_isRegex; }

private:
    string m_filename; // Stores filename
    string m_searchTerm; // Stores search term
    bool m_isRegex; // Flag to indicate if search term is a regex
    vector<string> m_lines; // Stores file contents
};

// Task A5: CSV Logging - CSVLogger class definition
class CSVLogger {
public:
    // Static method to save search results to CSV file
    static void saveResults(const string& filename, const string& searchTerm, double frequency) {
        ofstream resultFile("results.csv", ios::app); // Opens CSV file in append mode
        if (resultFile.is_open()) {
            // Write results to CSV file
            resultFile << filename << "," << searchTerm << "," << frequency << endl;
            resultFile.close();
            cout << "\nResults appended to results.csv" << endl;
        }
        else {
            cout << "Unable to open results.csv for writing" << endl;
        }
    }
};

// Function declaration for finding command-line arguments
int findArg(int argc, char* argv[], string pattern);

// Main function
int main(int argc, char* argv[])
{
    // Check for minimum required arguments
    if (argc < 3) {
        cout << "Error: Insufficient arguments. Usage: TaskA <filename> <search term> [-regex]" << endl;
        return EXIT_FAILURE;
    }

    // Extract filename and search term from command line arguments
    string fileName(argv[1]);
    string searchTerm(argv[2]);
    // Check if -regex flag is present
    bool isRegex = findArg(argc, argv, "-regex") != 0;

    // Task A1: Open and read the file
    FileSearcher searcher(fileName, searchTerm, isRegex);
    if (!searcher.readFile()) {
        return EXIT_FAILURE;
    }

    // Display file contents
    cout << "File contents:" << endl;
    for (const auto& l : searcher.getLines()) {
        cout << l << endl;
    }
    cout << endl;

    // Task A2 & A3: Perform search and display results
    cout << "Search results for '" << searchTerm << "':" << endl;

    // If regex is used, compile the regex pattern
    regex searchRegex;
    if (isRegex) {
        try {
            searchRegex = regex(searchTerm);
        }
        catch (const regex_error& e) {
            cout << "Error: Invalid regular expression" << endl;
            return EXIT_FAILURE;
        }
    }

    // Iterate through each line and word to perform the search
    for (size_t lineNum = 0; lineNum < searcher.getLines().size(); ++lineNum) {
        const string& currentLine = searcher.getLines()[lineNum];
        istringstream iss(currentLine);
        string word;
        size_t wordNum = 0;

        while (iss >> word) {
            bool match = false;
            if (isRegex) {
                match = regex_search(word, searchRegex);
            }
            else {
                match = (word == searchTerm);
            }

            if (match) {
                cout << "Line " << lineNum + 1 << ", Word " << wordNum + 1 << ": " << word << endl;
            }
            ++wordNum;
        }
    }

    // Task A4: Display n-gram statistics
    map<string, int> unigrams;
    map<string, int> bigrams;

    // Calculate unigram and bigram frequencies
    for (const auto& line : searcher.getLines()) {
        for (size_t i = 0; i < line.length(); ++i) {
            if (isalnum(line[i]) || ispunct(line[i])) {
                // Count unigrams
                string unigram(1, line[i]);
                unigrams[unigram]++;

                // Count bigrams
                if (i < line.length() - 1) {
                    if (isalnum(line[i + 1]) || ispunct(line[i + 1])) {
                        string bigram = unigram + line[i + 1];
                        bigrams[bigram]++;
                    }
                }
            }
        }
    }

    // Display unigram statistics
    cout << "\nUnigram statistics:" << endl;
    for (const auto& pair : unigrams) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Display bigram statistics
    cout << "\nBigram statistics:" << endl;
    for (const auto& pair : bigrams) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Task A5: Save Results
    int totalChars = 0;
    int hitCount = 0;
    // Calculate total characters and hit count
    for (const auto& line : searcher.getLines()) {
        totalChars += line.length();
        size_t pos = 0;
        while ((pos = line.find(searchTerm, pos)) != string::npos) {
            hitCount++;
            pos += searchTerm.length();
        }
    }
    // Calculate hit frequency as a percentage
    double frequency = (totalChars > 0) ? (static_cast<double>(hitCount) / totalChars) * 100 : 0;

    // Use CSVLogger to save results
    CSVLogger::saveResults(fileName, searchTerm, frequency);

    return EXIT_SUCCESS;
}

// Function to find a specific argument in the command line
int findArg(int argc, char* argv[], string pattern)
{
    for (int n = 1; n < argc; n++) {
        string s1(argv[n]);
        if (s1 == pattern) {
            return n;
        }
    }
    return 0;
}


////////////////////////////////////////////////////////////
//         All sites I used to complete the tasks         // 
///////////////////////////////////////////////////////////

// https://www.w3schools.com/cpp/default.asp
// https://openresearchsoftware.metajnl.com/articles/10.5334/jors.105
// https://gist.github.com/gaulinmp/da5825de975ed0ea6a24186434c24fe4
// https://cloud.google.com/bigquery/docs/reference/standard-sql/bigqueryml-syntax-ngrams
// https://stackoverflow.com/questions/47118015/n-gram-ignoring-line-breaks
// https://www.kaggle.com/code/sauravmaheshkar/auto-completion-using-n-gram-models