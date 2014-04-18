#include <iostream>
#include <set>
#include <vector>

int main(int argc, char *argv[]) {
    std::set<int> clause;
	std::vector<std::set<int> > clauseVector;

	std::cout<<"Clauses added "<<(clauseVector.size());
	  
	std::set<int> tempSet;
    int propagator = 0;
    for(auto clause : clauseVector){
      for(const int i : clause)
	    tempSet.insert(i);
	  if(tempSet.size() == 1)
	    for(const int i : clause)
	      propagator = i;
		if(propagator!= 0)
	      std::cout << " Propagator = " <<propagator << "\n";
	    tempSet.clear();
	    propagator = 0;
	}
}