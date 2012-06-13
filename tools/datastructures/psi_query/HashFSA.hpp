#ifndef HashFSA_HDR
#define HashFSA_HDR

#include <vector>
#include <iterator>
#include <iostream>
#include <cassert>
#include <algorithm>

#include "FSATypes.hpp"

/*******************************************************************************
# HashFSA

A wrapper class for using finite state automata as ordered minimal perfect hash
functions. Any finite state automaton implementation can be used if the
`ArcWeighted` template is used as the arc type (default template parameter).

The automaton should be acyclic and minimal. Otherwise behaviour is
undefined and can (will!) lead to errors.

## Synopsis

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

      if (index != hasher.size())
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

*******************************************************************************/

namespace psi {

  template <class FSA>
  class HashFSA {
    private:

      typedef typename FSA::arc_iterator_type ArcIt;

      FSA& m_fsa;
      size_t m_size;

      void rec_compute_word_numbers(size_t start, std::vector<size_t> &tc);

      ArcIt find(typename FSA::state_type p,
                 typename FSA::arc_type::symbol_type a) const;
      ArcIt find_num(typename FSA::state_type p, size_t count);
      ArcIt linsearch_num(ArcIt start, ArcIt end, Weight value);
      ArcIt binsearch_num(ArcIt start, ArcIt end, Weight value);


    public:

/*******************************************************************************
## Constructor

  HashFSA(FSA& fsa);

Requires a finite state implementation with the ArcWeighted<...>
template as the arc type.

*******************************************************************************/
      HashFSA(FSA& fsa);

/*******************************************************************************
## Checking for inclusion

    template <typename InputIterator>
    bool in(InputIterator begin, InputIterator end);

    bool in(std::string);

Check whether the passed string belongs to the language of the underlying finite
state automaton.

*******************************************************************************/
      template <typename InputIterator>
      bool in(InputIterator begin, InputIterator end);
      bool in(std::string);

/*******************************************************************************
## Return number of hashed strings

    size_t size();

Returns the number of string belonging to the underlying automaton.

*******************************************************************************/
      size_t size();

/*******************************************************************************
## Retrieve hash value

    template <typename InputIterator>
    size_t hash(InputIterator begin, InputIterator end);

    size_t hash(std::string input);

Retrieve the hash value assigned to the given string. The hash value is smaller
thatn `size()` if it belongs to the language of the automaton. If the hashed
string does not belong to the language of the automaton, `size()` is returned.

Hash values are assigned according to the lexicographical order of strings in
the automaton's language.

*******************************************************************************/
      template <typename InputIterator>
      size_t hash(InputIterator begin, InputIterator end);
      size_t hash(std::string input);

/*******************************************************************************
## Retrieve string by hash value

    template <typename OutputIterator>
    void unhash(size_t index, OutputIterator out);

    std::string unhash(size_t index);

The automaton can be queried in both directions. The hash value can also be
used to retrieve the orginal string from the automaton. The hash value `index`
has to be smaller than `size()`;

*******************************************************************************/
      template <typename OutputIterator>
      void unhash(size_t index, OutputIterator out);
      std::string unhash(size_t index);


/*******************************************************************************
## Return number of hashed strings

    template <class OStream>
    void print(Ostream ostream);

    void print();

Prints a textual representation of the underlying automaton to an output stream
or std::cout

*******************************************************************************/
      template <class OStream>
      void print(OStream& ostream);

      void print();
  };

  template <class FSA>
  void HashFSA<FSA>::rec_compute_word_numbers(size_t start, std::vector<size_t> &tc) {
    if (tc[start] == 0) {
      ArcRange<ArcIt> r = m_fsa.getArcs(start);

      for(ArcIt arcIt = r.first; arcIt != r.second; arcIt++) {
        const_cast<typename FSA::arc_type&>(*arcIt).setWeight(tc[start]);
        rec_compute_word_numbers(arcIt->getDest(), tc);
        tc[start] += tc[arcIt->getDest()];
      }

      if (m_fsa.isEndState(start)) {
        tc[start]++;
      }
    }
  }

  template <class FSA>
  typename HashFSA<FSA>::ArcIt HashFSA<FSA>::find(typename FSA::state_type p,
                                                  typename FSA::arc_type::symbol_type a) const {

    ArcRange<ArcIt> r = m_fsa.getArcs(p);

    typename FSA::arc_type test(a, -1);
    return std::lower_bound(r.first, r.second, test, ArcSorter());
  }

  template <class FSA>
  typename HashFSA<FSA>::ArcIt HashFSA<FSA>::find_num(typename FSA::state_type p, size_t count) {
    ArcRange<ArcIt> r = m_fsa.getArcs(p);
    size_t dist = std::distance(r.second, r.first);

    ArcIt arcIt;
    if (dist > 10)
        arcIt = binsearch_num(r.first, r.second-1, count);
    else if (dist > 0)
        arcIt = linsearch_num(r.first, r.second-1, count);
    return arcIt;
  }

  template <class FSA>
  typename HashFSA<FSA>::ArcIt HashFSA<FSA>::linsearch_num(ArcIt start, ArcIt end, Weight value) {
      for(ArcIt it = start; it <= end; it++) {
          if (it->getWeight() < value and
             (it == end or (it+1)->getWeight() >= value)) {
              return it;
          }
      }
      return end;
  }

  template <class FSA>
  typename HashFSA<FSA>::ArcIt HashFSA<FSA>::binsearch_num(ArcIt start, ArcIt end, Weight value) {
      ArcIt L = start;
      ArcIt R = end;
      while (L <= R) {
          ArcIt m = L + ((R - L)/2);
          if (m->getWeight() < value and (m == end or (m+1)->getWeight() >= value))
              return m;
          else if (m->getWeight() < value)
              L = m + 1;
          else
              R = m - 1;
      }
      return end;
  }

  template <class FSA>
  HashFSA<FSA>::HashFSA(FSA& fsa)
  : m_fsa(fsa), m_size(0) {
    std::vector<size_t> tempCounts(m_fsa.size(), 0);
    rec_compute_word_numbers(0, tempCounts);

    m_size = tempCounts[0];
  }

  template <class FSA>
  template <typename InputIterator>
  bool HashFSA<FSA>::in(InputIterator begin, InputIterator end) {
    return m_fsa.in(begin, end);
  }

  template <class FSA>
  size_t HashFSA<FSA>::size() {
    return m_size;
  }

  template <class FSA>
  template <class OStream>
  void HashFSA<FSA>::print(OStream &ostream) {
    for (size_t i = 0; i < m_fsa.size(); i++) {
      ArcRange<ArcIt> r = m_fsa.getArcs(i);
      for (ArcIt it = r.first; it != r.second; it++) {
        ostream << i << "\t" << it->getDest()
          << "\t" << (long)it->getSymbol()
          << "\t" << (long)it->getWeight()
          << std::endl;
      }

      if (m_fsa.isEndState(i)) {
        ostream << i << std::endl;
      }
    }
  }

  template <class FSA>
  void HashFSA<FSA>::print() {
    print(std::cout);
  }

  template <class FSA>
  template <typename InputIterator>
  size_t HashFSA<FSA>::hash(InputIterator begin, InputIterator end) {
    int index = 0;
    typename FSA::state_type current_state = 0;

    while (begin != end) {
      ArcRange<ArcIt> r = m_fsa.getArcs(current_state);
      typename FSA::arc_type::symbol_type a = *begin;
      ArcIt arcIt = find(current_state, a);
      if (arcIt != r.second) {
        index += arcIt->getWeight();
        current_state = arcIt->getDest();
        if (m_fsa.isEndState(current_state))
          index++;
      }
      else
        return size();
      begin++;
    }

    if (m_fsa.isEndState(current_state))
      return index-1;
    else
      return size();
  }

  template <class FSA>
  size_t HashFSA<FSA>::hash(std::string input) {
    return hash(input.begin(), input.end());
  }

  template <class FSA>
  template <typename OutputIterator>
  void HashFSA<FSA>::unhash(size_t index, OutputIterator out) {
    assert(index < size());

    typename FSA::state_type current_state = 0;

    index++;
    while (index > 0) {
      ArcRange<ArcIt> r = m_fsa.getArcs(current_state);
      ArcIt arcIt = find_num(current_state, index);
      if (arcIt != r.second) {
        current_state = arcIt->getDest();
        *out = arcIt->getSymbol();
        out++;
        index -= arcIt->getWeight();

        if (m_fsa.isEndState(current_state))
          index--;
      }
    }
  }

  template <class FSA>
  std::string HashFSA<FSA>::unhash(size_t index) {
    std::string output;
    std::back_insert_iterator<std::string> outIt(output);
    unhash(index, outIt);
    return output;
  }

}

#endif
