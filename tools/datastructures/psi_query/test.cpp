#include <iostream>
#include <string>
#include <fstream>

#include "Algorithms.hpp"
#include "HashFSA.hpp"
#include "NDFSA.hpp"
#include "BinDFSA.hpp"

using namespace std;
using namespace psi;

int main(int argc, char** argv) {
  
  // Create the main empty finite state automaton
  // `NDFSA` allows for fast insert and deletion
  NDFSA<> fsa;

  // Create a deterministic finite state automaton from a string
  NDFSA<> tfsa1("this is a test");
  
  // Compute the union of `dfsa` and `tdfsa1`;
  unify(fsa, tfsa1);
  
  NDFSA<> tfsa2("this is the second test");
  unify(fsa, tfsa2);
  
  NDFSA<> tfsa3("the next test");
  unify(fsa, tfsa3);
  
  NDFSA<> tfsa4("and another test");
  unify(fsa, tfsa4);
  
  // Minimize `dfsa` (required for hashing)
  minimize(fsa);
  
  // Create an empty DFSA for easy binarization 
  BinDFSA<> binfsa;
  
  // Copy `dsfa` to `bsfa`
  unify(binfsa, fsa);
  
  // Wrap `bfsa` into a `HashFSA`
  HashFSA<BinDFSA<> > hasher(binfsa);
  
  // Retrieve index of input string (corresponds to lexicographical order of
  // input strings)
  size_t index = hasher.hash("this is the second test");
  
  if(index != hasher.size())
    std::cout << "Found index: " << index << std::endl;  
  else
    std::cout << "Not found" << std::endl;
  
  // Retrieve string at hash position 3
  std::cout << hasher.unhash(3) << std::endl;
  
  // Save binary finite state automaton
  ofstream of("hash.fsa");
  binfsa.save(of);

  return 0;
}
