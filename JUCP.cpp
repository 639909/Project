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
  if (argc != 2){
	std::cout << "--Usage: ./UCP <Input File> \n";
	exit (1);
  }
  std::string inFileString = argv[1];
  if (inFileString.find(".cnf") == std::string::npos){
    std::cout << "--Error: Wrong file extension \n--Input file should be in .cnf Dimacs format \n";
  //  exit (1);
  }
  clock_t t1, t2;
  t1 = clock();
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
  //Finding the propagator
  int propagator = 0;
  for(auto iterator = std::begin(clauseVector) ; iterator != std::end(clauseVector);){
    const auto& currentClauseSet = *iterator;
      if(currentClauseSet.size() == 0)
        clauseVector.erase( iterator, clauseVector.end() );
      if(currentClauseSet.size() == 1)
        for(const int j : currentClauseSet){
          propagator = j;
          //std::cout << "propagator = " << propagator << "\n";
        }
        //Processing
        if(propagator != 0){
          auto iter = std::remove_if( clauseVector.begin(), clauseVector.end(),[propagator] ( const std::set<int>& i ){
            return i.find(propagator) != i.end() ; } ) ;
          clauseVector.erase( iter, clauseVector.end() ) ;
          int secondPropagator;
          if (propagator > 0)
            secondPropagator = -abs(propagator);
          else if (propagator < 0)
            secondPropagator = abs(propagator);
          std::for_each(clauseVector.begin(), clauseVector.end(), [&](std::set<int>& s){s.erase(secondPropagator);});
          iterator = clauseVector.begin() -1;
        }
        propagator = 0;
      iterator++;
    }
  //output
  std::ofstream outFile("out.txt");
  for (const auto &comments : commentsVector)
      outFile << comments << "\n";
  time_t now = time(0);
  char* dt = ctime(&now);
  outFile << "c This file was propagated on " << dt;
  for(const auto &countVariable : clauseVector)
    for(const int i : countVariable){
      int positiveVariable;
      if (i < 0) positiveVariable = abs(i);
      else positiveVariable = i;
      variableCount.insert(positiveVariable);

    }
  outFile << "p cnf " << variableCount.size() << " " << clauseVector.size() << "\n";
  for(const auto &printSet : clauseVector){
    for(const int i : printSet){
      outFile << i << " ";
    }
  outFile << "0" <<"\n";
  }
  t2 = clock();
  float seconds = floorf(((((float)t2 - (float)t1)/CLOCKS_PER_SEC) *100) /100);

  std::cout << " Time = " << seconds << " Secodns \n";
}
