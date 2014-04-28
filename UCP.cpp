#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

//Reminder
//Switch the order of propagation, single variable first then full clause

int main(const int argc, const char* const argv[]) {
    std::vector<std::string> clauses;
    std::ofstream outFile("output.txt");
    if (argc !=2)
        std::cout << "Usage: " << "./UCP <Input filename>\n";
    else{
        std::cout << "File has been read in \n";
        std::ifstream inFile(argv[1]);
        std::string line;
        std::string space = " ";
        while (std::getline(inFile, line)) {
            std::string c("c");
            std::string p("p");
            if(line.compare(0, c.length(), c)==0)
                outFile << line << "\n";
            else if(line.compare(0, p.length(), p)==0)
                outFile << line << "\n";
            else(clauses.push_back(space + line));
        }
        std::cout << "Clauses added to vector \n";
        outFile << "c This file has been propagated \n" << "c Solution: ";
        std::string propagator, mString;
        int numspaces = 0;
        char nextChar;
        for (int i=0; i<int(clauses.size()); i++){
            mString = clauses[i];
            for (int j=0; j<mString.length(); j++){
                nextChar = mString.at(j);
                if (isspace(mString[j]))
                    numspaces++;
            }
            int secondClause;
            if(numspaces == 2){
                size_t pos1 = 1;
                size_t pos2;
                pos2 = clauses[i].find(" ", pos1);
                propagator = clauses[i].substr(pos1, (pos2 - pos1));
                outFile << propagator << " ";
                i = -1;
                std::cout << "Currently propagating " << propagator << "\n";
                secondClause = 1;
            }
            clauses.erase(std::remove_if(clauses.begin(), clauses.end(),[&](const std::string &s) {return s.find(space + propagator + space) != std::string::npos;}),clauses.end());
            if(secondClause == 1){
                std::string dash = "-";
                int pos = propagator.find(dash);
                if(pos!=std::string::npos){
                    propagator.replace(pos,dash.size(),"");
                    propagator = space + propagator + space;
                    secondClause = 0;
                }
                else{
                    propagator = space + dash + propagator + space;
                    secondClause = 0;
                }
            }
            for (int i=0; i<int(clauses.size()); i++){
                std::string nString = clauses[i];
                int pos3 = nString.find(propagator);
                if(pos3 >= 0){
                    nString.replace(pos3,propagator.size()," ");
                    clauses[i] = nString;
                }
            }
            numspaces = 0;
        }
        outFile << "\n";
        std::cout << "Currently printing to output file";
        for (const auto &clause : clauses)
            outFile << clause << "\n";
        if(propagator.empty())
            outFile << "Propagation not possible";
    }
}