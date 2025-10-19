#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

const int N = 100;

struct State {
    char symbol = '.';
    std::string state_name = "";
    
    
};

struct Rule {
    char write_symbol = '.';
    char move_direction = 'N';
    std::string next_state = "";
};

std::string removeSpaces(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (c != ' ' && c != '\t') {
            result += c;
        }
    }
    return result;
}

std::pair<State, Rule> parseRule(const std::string& s) {
    State state;
    Rule rule;
    std::string line = removeSpaces(s);
    if (line.empty()) return {state, rule};

    size_t dash_pos = line.find('-');
    if (dash_pos == std::string::npos) return {state, rule};

    std::string left_part = line.substr(0, dash_pos);
    size_t comma_pos = left_part.find(',');
    if (comma_pos != std::string::npos) {
        state.symbol = left_part[0];
        state.state_name = left_part.substr(comma_pos + 1);
    } else {
        state.symbol = left_part[0];
    }

    std::string right_part = line.substr(dash_pos + 1);
    std::vector<std::string> parts;
    std::string current_part;
    for (char c : right_part) {
        if (c == ',') {
            if (!current_part.empty()) {
                parts.push_back(current_part);
                current_part.clear();
            }
        } else current_part += c;
    }
    if (!current_part.empty()) parts.push_back(current_part);

    if (parts.size() > 0) rule.write_symbol = parts[0][0];
    if (parts.size() > 1) rule.move_direction = parts[1][0];
    if (parts.size() > 2) rule.next_state = parts[2];

    return {state, rule};
}

std::map<State, Rule> readRules(const std::string& filename) {
    std::map<State, Rule> rules;
    std::ifstream file(filename);
    if (!file.is_open()) return rules;
    std::string line;
    while (std::getline(file, line)) {
        bool empty = true;
        for (char c : line) {
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                empty = false; break;
            }
        }
        if (empty) continue;
        auto [state, rule] = parseRule(line);
        if (!state.state_name.empty() || state.symbol != '.') {
            rules[state] = rule;
        }
    }
    file.close();
    return rules;
}

std::string readTape(const std::string& filename) {
    std::string tape;
    std::ifstream file(filename);
    if (!file.is_open()) return tape;
    char c;
    while (file.get(c)) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') tape += c;
    }
    file.close();
    return tape;
}

class TuringMachine {

    std::string tape;
    std::map<State, Rule> rules;
    State currentState;
    int headPosition;
    char lambda;

    TuringMachine(const std::string& initialTape, 
                 const std::map<State, Rule>& ruleset,
                 const State& startState,
                 char blank = '.')
        : tape(initialTape), rules(ruleset), 
          currentState(startState), headPosition(0), lambda(blank) {}
    
    bool step() {
        
        char currentSymbol;
        if (headPosition < 0 || headPosition >= (int)tape.size())
            currentSymbol = lambda;
        else
            currentSymbol = tape[headPosition];
        
        State key{currentSymbol, currentState.state_name};
        auto it = rules.find(key);
        if (it == rules.end()) return false;
        
        const Rule& rule = it->second;

        if (headPosition >= 0 && headPosition < (int)tape.size()) {
            tape[headPosition] = rule.write_symbol;
        } else {
            if (headPosition < 0) {
                std::string new_tape(-headPosition, lambda);
                new_tape += tape;
                tape = new_tape;
                headPosition = 0;
            } else {
                tape += std::string(headPosition - tape.size() + 1, lambda);
            }
            tape[headPosition] = rule.write_symbol;
        }

        if (rule.move_direction == 'L') headPosition--;
        else if (rule.move_direction == 'R') headPosition++;

        currentState.state_name = rule.next_state;
        return true;
    }
    
    void run(int maxSteps = N) {
        int steps = 0;
        while (steps < maxSteps && step()) {
            steps++;
            display();
        }
    }
    
    void display() const {
        std::cout << "State: " << currentState.state_name << "\n";
        std::cout << "Head: " << headPosition << "\n";
        std::cout << "Tape: ";
        for (size_t i = 0; i < tape.size(); i++) {
            if ((int)i == headPosition) std::cout << "[" << tape[i] << "]";
            else std::cout << " " << tape[i] << " ";
        }
        std::cout << "\n\n";
    }
};

int main() {

    
    return 0;
}
