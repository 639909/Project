#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>

void printToScreen(int var, int clause, std::string inFile, std::string outFile, int varCount, int clauCount, int seconds, int variableCountActual, int actualClauseCount){
  std::cout << "-------------------------------------------------------\n";
  std::cout << "-- Initial Variable Count on Problem Line            " << var << "\n";
  if (var != variableCountActual)
    std::cout << "      -- Actual Variable Count                       " << variableCountActual << "\n";
  std::cout << "-- Initial Clause Count on Problem Line              " << clause << "\n";
  if (clause != actualClauseCount)
    std::cout << "      -- Actual Clause Count                         " << actualClauseCount << "\n";
  std::cout << "-- Input File                                        " << inFile << "\n";
  std::cout << "-- Output File                                       " << outFile<< "\n";
  std::cout << "-- Variables after propagation                       " << varCount << "\n";
  std::cout << "-- Clauses after propagation                         " << clauCount << "\n";
  std::cout << "-- Time taken                                        " << seconds << " Seconds\n";
  std::cout << "-------------------------------------------------------\n";
}
void noFile(){
  std::cout << "-- Usage:\n";
  std::cout << "-- ./JUCP -v\n";
  std::cout << "    -- Will show the current version of the software\n";
  std::cout << "-- ./JUCP <Input File> \n";
  std::cout << "    -- Runs solver with <Input File> then will output to <InputFile>-propagated.cnf\n";
  std::cout << "-- ./JUCP <Input File>  <Output File> \n";
  std::cout << "    -- Runs solver with <Input File> then Will output to <Output File>.cnf\n";
  exit (1);
}
int inversePropagator(int propagator){
  int secondPropagator;
  if (propagator > 0)
    secondPropagator = -abs(propagator);
  else if (propagator < 0)
    secondPropagator = abs(propagator);
  return secondPropagator;
}
int countVariable(std::vector<std::set<int> > clauseVector, std::set<int> variableCount){
  for(const auto &countVariable : clauseVector)
    for(const int i : countVariable){
      int positiveVariable;
      if (i < 0) positiveVariable = abs(i);
      else positiveVariable = i;
      variableCount.insert(positiveVariable);
    }
    int variableCountInt = variableCount.size();
    return variableCountInt;
}

int main(const int argc, char *argv[]) {
  if (argc < 2)
    noFile();
  std::string inFileString = argv[1];
  if (inFileString == "-v"){
    std::cout << "-- JUCP release version: 1.0 \n-- Github version: 0a761f05e45b632fb1392382b846d62ba7c7bd78 \n";
    exit(1);
  }
  if (inFileString.find(".cnf") == std::string::npos){
    std::cout << "-- Error: Wrong file extension \n-- Input file should be in .cnf Dimacs format \n";
    exit (1);
  }
  clock_t t1, t2;
  t1 = clock();
  std::set<int> clause;
  std::vector<std::set<int> > clauseVector;
  std::vector<std::string> commentsVector;
  std::ifstream inFile (argv[1]);
  std::string currentClause, problem, problemComment;
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
  if (problem == ""){
    std::cout << "-- Error: No problem line\n";
    exit(1);
  }
  int commentCount = commentsVector.size() + 1;
  clauseVector.erase(clauseVector.begin(), clauseVector.begin() + commentCount);
  int actualClauseCount = clauseVector.size();
  std::string outFileName;
  if (argc == 3){
    std::string outPrefix = argv[2];
    outFileName = outPrefix + ".cnf";
  }
  else {
    int lastIndex = inFileString.find_last_of(".");
    std::string fileName = inFileString.substr(0, lastIndex);
    outFileName = fileName + "-propagated.cnf";
  }
  std::set<int> variableCount;
  int variableCountActual = countVariable(clauseVector, variableCount);
  int initianVariableCount, initialClauseCount;
  problemComment = problem;
  problem.erase (0,6);
  std::stringstream str(problem);
  str >> initianVariableCount >> initialClauseCount;
  std::vector<int> propagators;
  int propagator = 0;
  for(auto iterator = std::begin(clauseVector) ; iterator != std::end(clauseVector);){
    const auto& currentClauseSet = *iterator;
      if(currentClauseSet.size() == 0)
        clauseVector.erase( iterator, clauseVector.end() );
      if(currentClauseSet.size() == 1)
        for(const int j : currentClauseSet){
          propagator = j;
          propagators.push_back(propagator);
        }
        if(propagator != 0){
          auto iter = std::remove_if( clauseVector.begin(), clauseVector.end(),[propagator] ( const std::set<int>& i ){
            return i.find(propagator) != i.end() ; } ) ;
          clauseVector.erase( iter, clauseVector.end() ) ;
          int secondPropagator = inversePropagator(propagator);
          std::for_each(clauseVector.begin(), clauseVector.end(), [&](std::set<int>& s){s.erase(secondPropagator);});
          iterator = clauseVector.begin() -1;
        }
      propagator = 0;
      iterator++;
    }
  for (auto pr : propagators)
    std::cout << pr << ' ';
  std::cout << "\n";
  std::ofstream outFile(outFileName);
  for (const auto &comments : commentsVector)
      outFile << comments << "\n";
  time_t now = time(0);
  char* dt = ctime(&now);
  outFile << "c This file was propagated on " << dt;
  outFile << "c Orginal problem line: " << problemComment << "\n";
  outFile << "c Propagators: ";
  for (int k = 0; k < propagators.size(); ++k){
    outFile << propagators[k] << " ";
    if ((k + 1) % 10 == 0){
      outFile << "\nc Propagators: ";
    }
  }
  int variableCountInt = countVariable(clauseVector, variableCount);
  outFile << "\np cnf " << variableCountInt << " " << clauseVector.size() << "\n";
  for(const auto &printSet : clauseVector){
    for(const int i : printSet){
      outFile << i << " ";
    }
  outFile << "0" <<"\n";
  }
  t2 = clock();
  float seconds = floorf(((((float)t2 - (float)t1)/CLOCKS_PER_SEC) *100) /100);
  printToScreen(initianVariableCount,initialClauseCount,argv[1], outFileName, variableCountInt, clauseVector.size(), seconds, variableCountActual, actualClauseCount);
}
