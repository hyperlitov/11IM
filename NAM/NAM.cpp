#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

struct trio{
    std::string start = "";
    std::string end = "";
    bool ifend = 0;
};

std::string tapereading(std::string filenametxt){
    std::string s; char c;

    std::fstream file;
    file.open(filenametxt);

    while (!file.eof()) {
        file >> c;
        s += c;
    }

    s.pop_back();
    file.close();

    return s;
}



trio stringparse(std::string s){
    int i = 0;
    trio a;

    for(i; s[i] != '-' && s[i] != '|'; i++) a.start += s[i];
    if(s[i] == '|') {a.ifend = 1; i++;}

    i+=2;

    for(i; i<s.size(); i++) a.end +=s[i];
    return a;

    // std::cout << a.start<<" "<<a.end <<" "<<a.ifend<<"\n";
}

std::vector<std::string> r (std::string filenametxt){
    std::fstream file; std::string s; std::vector<std::string> v; int ind = 0;
    file.open(filenametxt);

    while(std::getline(file, s) && !s.empty()){
        trio tr = stringparse(s);
        v.push_back(tr.start);
        // std::cout << "Rule " << v[ind]<<"\n";
        ind++;
    }
    file.close();
    return v;

}

std::map<std::string, std::pair<std::string, bool>> rulesreading(std::string filenametxt){
    std::string s;
    std::map<std::string, std::pair<std::string, bool>> x;
    trio tr;

    std::fstream file;
    file.open(filenametxt);

    while(std::getline(file, s) && !s.empty()){
        tr = stringparse(s);
        x[tr.start].first = tr.end;
        x[tr.start].second = tr.ifend;

        // std::cout<<tr.start<<" "<<x[tr.start].first<<" "<<x[tr.start].second<<"\n";
    }
    file.close();

    return x;
}

std::string nam(std::string tape, std::map<std::string, std::pair<std::string, bool>> rules, std::string filenametxt){
    int index;
    bool q;
    std::pair<std::string, bool> p;
    std::vector<std::string> ru = r(filenametxt);
    
    while(true){
        q = 0; index = 0;
        int k;
        for(index; index < ru.size(); index++){
            k = tape.find(ru[index]);
            // std::cout << index <<" "<< k <<"\n";
            if(k != std::string::npos) break;
            else if (index == ru.size()-1) q = 1;
        }
        if(q) break;
        tape.erase(k, ru[index].size());
        tape.insert(k, rules[ru[index]].first);
        if(rules[ru[index]].second) break;
    }
    return tape;
}

int main(){
    std::string tape;
    tape = tapereading("namTape.txt");

    std::cout << tape << "\n";

    // std::cout << tape.find("8888")<<"\n";

    std::map<std::string, std::pair<std::string, bool>> rule;
    rule = rulesreading("namRule.txt");

    std::cout << nam(tape, rule, "namRule.txt");


    return 0;
}