#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>

int main(const int argc, char *argv[]) {
  //input
  std::string inFileString = argv[1];
  if (argc!=2){
	std::cout << "Usage: ./UCP <Input File>";
	return 0;
  }
  if (inFileString.find(".cnf") == std::string::npos){
    std::cout << "Error: Wrong file extension \n";
    //return 0;
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
  int commentCount = commentsVector.size() + 1;
  clauseVector.erase(clauseVector.begin(), clauseVector.begin() + commentCount);
  //Calculating variable and clause count
  std::set<int> variableCount;
  variableCount.clear();
  //Finding the propagator
  int propagator = 0;
  for(auto iter = std::begin(clauseVector) ; iter != std::end(clauseVector);){
    const auto& currentClauseSet = *iter;
      if(currentClauseSet.size() == 0)
        propagator = 0;
      if(currentClauseSet.size() == 1)
        for(const int j : currentClauseSet){
          propagator = j;
          std::cout << "propagator = "<< propagator << "\n";
        //Processing
        if(propagator != 0){
          auto iter = std::remove_if( clauseVector.begin(), clauseVector.end(),[propagator] ( const std::set<int>& i ){
            return i.find(propagator) != i.end() ; } ) ;
          clauseVector.erase( iter, clauseVector.end() ) ;
          int secondPropagator = 0;
          if (propagator >0)
            secondPropagator = -abs(propagator);
          else if (propagator < 0)
            secondPropagator = abs(propagator);
          std::for_each(clauseVector.begin(), clauseVector.end(), [&](std::set<int>& s){s.erase(secondPropagator);});
        }
        iter = iter - 1;
      }
      iter++;
    }
  //output
  std::ofstream outFile("out.txt");
  for (const auto &comments : commentsVector)
      outFile << comments << "\n";
  time_t now = time(0);
  char* dt = ctime(&now);
  outFile << "c This file was propagated on " << dt;
  for(const auto &countVariable : clauseVector)
    for(const int i : countVariable)
      variableCount.insert(i);
  outFile << "p cnf " << variableCount.size() << " " << clauseVector.size() << "\n";
  for(const auto &printSet : clauseVector){
    for(const int i : printSet){
      outFile << i << " ";
    }
  outFile << "0" <<"\n";
  }
}
