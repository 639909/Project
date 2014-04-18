#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

int main(int argc, char *argv[]) {
  //input
  std::set<int> clause;
  std::vector<std::set<int> > clauseVector;
  std::vector<std::string> commentsVector, problemVector;
  std::ifstream inFile("in.cnf");
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
	    clauseVector.push_back(clause);
        clause.clear();
     }
  }
  //processing
  std::set<int> tempSet;
  int propagator = 0;
  for(auto clause : clauseVector){
    for(const int i : clause)
	  tempSet.insert(i);
    if(tempSet.size() == 1)
      for(const int i : clause)
	    propagator = i;
	if(propagator!= 0){
      auto iter = std::remove_if( clauseVector.begin(), clauseVector.end(),[propagator] ( const std::set<int>& s ){ 
	    return s.find(propagator) != s.end() ; } ) ;
      clauseVector.erase( iter, clauseVector.end() ) ;
    }
    tempSet.clear();
    propagator = 0;
  }
  //output
  std::ofstream outFile("in.cnf");
  for(std::set<int> const &mySet : clauseVector){
    for(const int i : mySet){
      outFile << i << " ";
    }
  outFile << "0" <<"\n";
  }
}