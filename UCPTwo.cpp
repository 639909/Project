#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

int main(int argc, char *argv[]) {
  std::set<int> clause;
  std::vector<std::set<int> > clauseVector;
  std::vector<std::string> commentsVector, problemVector;
  std::ifstream inFile("input.txt");
  std::string currentClause;
  while (std::getline(inFile, currentClause)){
	std::istringstream iss(currentClause);
	std::string c("c");
	if(currentClause.compare(0, c.length(), c)==0)
	  commentsVector.push_back(currentClause);
	std::string p("p");
	if(currentClause.compare(0, p.length(), p)==0)
      problemVector.push_back(currentClause);
	int variable;
	for (std::istringstream iss(currentClause); iss >> variable;) 
	  if (variable != 0)
	    clause.insert(variable);
      if (variable == 0){
	    clause.insert(variable);
	    clauseVector.push_back(clause);
        clause.clear();
     }
  }
  std::set<int> tempSet;
  int propagator = 0;
  for(auto clause : clauseVector){
    for(const int i : clause)
	  tempSet.insert(i);
    if(tempSet.size() == 1)
      for(const int i : clause)
	    propagator = i;
	if(propagator!= 0)
    tempSet.clear();
    propagator = 0;
  }
}