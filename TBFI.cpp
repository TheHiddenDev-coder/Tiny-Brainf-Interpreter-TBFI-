// BRAIN**** INTERPRETER IN C++
// OPTIMIZED WITH PRECOMPUTED BRACKET MATCHES

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename.bf>\n";
        return 1;
    }

    // Read the Brainfuck code from file
    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << argv[1] << "\n";
        return 1;
    }

    string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Precompute matching brackets for fast jumps
    unordered_map<size_t, size_t> bracketMap;
    stack<size_t> bracketStack;

    for (size_t i = 0; i < code.size(); ++i) {
        if (code[i] == '[') {
            bracketStack.push(i);
        } else if (code[i] == ']') {
            if (bracketStack.empty()) {
                cerr << "Error: Unmatched ']' at position " << i << "\n";
                return 1;
            }
            size_t openPos = bracketStack.top();
            bracketStack.pop();
            bracketMap[openPos] = i;
            bracketMap[i] = openPos;
        }
    }

    if (!bracketStack.empty()) {
        cerr << "Error: Unmatched '[' at position " << bracketStack.top() << "\n";
        return 1;
    }

    const int TAPE_SIZE = 30000;
    vector<unsigned char> tape(TAPE_SIZE, 0);
    size_t ptr = 0;

    for (size_t i = 0; i < code.size(); ++i) {
        switch (code[i]) {
            case '>': 
                ++ptr; 
                if (ptr >= TAPE_SIZE) ptr = 0; // wrap around
                break;
            case '<': 
                if (ptr == 0) ptr = TAPE_SIZE - 1; else --ptr;
                break;
            case '+': 
                ++tape[ptr]; 
                break;
            case '-': 
                --tape[ptr]; 
                break;
            case '.': 
                cout << tape[ptr]; 
                break;
            case ',': 
                tape[ptr] = cin.get(); 
                break;
            case '[':
                if (tape[ptr] == 0) i = bracketMap[i]; // jump forward
                break;
            case ']':
                if (tape[ptr] != 0) i = bracketMap[i]; // jump backward
                break;
            default:
                // Ignore other characters
                break;
        }
    }

    return 0;
}
