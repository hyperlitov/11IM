#include <iostream>
#include <fstream>
#include <string>


struct State{
    
    char symbol;

};

struct Rule{

};

State 



int main(){

    std::fstream tapefile;
    tapefile.open("tape.txt");

    std::string tape = ".";
    char c;

    while (!tapefile.eof()){
        tapefile >> c;
        tape += c;
    }

    std::map<State>

    return 0;
}