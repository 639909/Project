#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

int main(const int argc, char *argv[]) {
  //input
  if (argc!=2){
	std::cout << "Usage: ./UCP <Input File>";
	return 0;
  }
  std::set<int> clause;
  std::vector<std::set<int> > clauseVector;
  std::vector<std::string> commentsVector;
  std::ifstream inFile (argv[1]);
  std::string currentClause, problem;
  while (std::getline(inFile, currentClause)){
	std::istringstream iss(currentClause);
	std::string c("c");
	if(currentClause.compare(0, c.length(), c)==0)
	  commentsVector.push_back(currentClause);
	std::string p("p");
	if(currentClause.compare(0, p.length(), p)==0)
      problem = currentClause;
	int variable;
	for (std::istringstream iss(currentClause); iss >> variable;)
	  if (variable != 0)
	    clause.insert(variable);
      if (variable == 0){
	    clauseVector.push_back(clause);
        clause.clear();
     }
  }
  int variableCount, clauseCount;
  problem.erase (0,6);
  std::stringstream str(problem);
  str >> variableCount >> clauseCount; 
  std::cout << "Variables: " << variableCount << " Clauses: " << clauseCount;

  int commentCount = commentsVector.size() +1;
  clauseVector.erase(clauseVector.begin(),clauseVector.begin() + commentCount);
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
      auto iter = std::remove_if( clauseVector.begin(), clauseVector.end(),[propagator] ( const std::set<int>& i ){ 
	    return i.find(propagator) != i.end() ; } ) ;
      clauseVector.erase( iter, clauseVector.end() ) ;
    int secondPropagator;
    if (propagator >0)
      secondPropagator = -abs(propagator);
    else if (propagator < 0)
      secondPropagator = abs(propagator);
    std::for_each(clauseVector.begin(), clauseVector.end(), [&](std::set<int>& s){s.erase(secondPropagator);});
    }
    tempSet.clear();
    propagator = 0;
  }
  //output
  std::set<int> clauseCountV;
  time_t now = time(0);
  char* dt = ctime(&now);
  std::ofstream outFile("out.cnf");
  for (const auto &comments : commentsVector)
      outFile << comments << "\n";
  outFile << "c This file was propagated on the " << dt;
  for(std::set<int> const &mySet : clauseVector)
    for(const int i : mySet)
	  clauseCountV.insert(i);
  outFile << "p cnf " << clauseCountV.size() << " " << clauseVector.size() <<  "\n";

  for(std::set<int> const &mySet : clauseVector){
    for(const int i : mySet){
      outFile << i << " ";
    }
  outFile << "0" <<"\n";
  }
}
