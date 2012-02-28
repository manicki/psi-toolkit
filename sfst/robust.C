
/*******************************************************************/
/*                                                                 */
/*     File: robust.C                                              */
/*   Author: Helmut Schmid                                         */
/*  Purpose:                                                       */
/*  Created: Wed Aug  3 08:49:16 2005                              */
/* Modified: Thu Nov 27 17:15:25 2008 (schmid)                     */
/*                                                                 */
/*******************************************************************/

#include "compact.h"

using std::vector;

namespace SFST {

  // data structure for a search path

  class Path {
  public:
    unsigned int arc_number; // number of the current transducer arc
    unsigned int position;   // number of processed input symbols
    float errors;            // errors accumulated so far
    int previous;            // back-pointer (for printing)
  
    // contructor
    Path( unsigned int n, unsigned int p, float e, unsigned int pp )
    { arc_number = n; position = p; errors = e; previous = (int)pp; };

    // constructor for the intial path
    Path() { arc_number = 0; position = 0; errors = 0; previous = -1; };

    // check whether a path is the intial path
    bool is_start() { return previous == -1; };
  };


  // search data structure containing all the search paths

  class Agenda {
  private:
    float errors_allowed;  // maximum number of errors allowed
    vector<Path> path;     // set of active and inactive paths (for printing)
    vector<size_t> active_path;    // set of currently active search paths
    vector<size_t> complete_path;  // set of complete search paths

  public:

    Agenda( float e ) {
      // initialization
      errors_allowed = e;
      path.push_back(Path());     // initial search path
      active_path.push_back(0);   // one active search path
    };

    // get the highest ranked active search path
    Path &best_active_path() { return path[active_path.back()]; };

    // retrieve an active path by its index number
    Path &get_active_path( int i ) { return path[active_path[i]]; };

    // get the highest ranked complete search path
    Path &first_complete_path() { return path[complete_path[0]]; };

    // check whether the analysis is finished
    bool finished() {
      return (active_path.size() == 0 || // no more active paths
	      (complete_path.size() > 0 && // best analysis was found
	       best_active_path().errors > first_complete_path().errors));
    };

    // add a new search path
    void add_path( int s, unsigned int pos, float e, int pp, bool final );

    void add_analysis( int sn, CAnalysis &ana );
    void extract_analyses( vector<CAnalysis> &analyses );

    friend class CompactTransducer;
  };


  // trivial error functions for the beginning

  float mismatch_error( Character c, Character c2) { 
    return 1.0;
  };
  float deletion_error( Character c) { return 1.0; };
  float insertion_error( Character c) { return 1.0; };
  float transpose_error( Character c, Character c2) { return 1.0; };




  /*******************************************************************/
  /*                                                                 */
  /*  Agenda::add_path                                               */
  /*                                                                 */
  /*******************************************************************/

  void Agenda::add_path(int arc, unsigned int pos, float e, int pp, bool final)

  {
    // check whether the number of allowed errors is exceeded
    if (e > errors_allowed)
      return;

    // check whether a complete analysis with fewer errors exists
    if (complete_path.size() > 0 && first_complete_path().errors < e)
      return;
  
    // store the new search path
    size_t sn=path.size();              // index of the new search path
    path.push_back(Path(arc, pos, e, pp)); // add the new path

    // sorted insertion of the new active path (reversed order)
    active_path.push_back(sn);    // increase the array size by 1

    // copy the paths up to the insertion position
    int i=(int)active_path.size()-1;
    while (i > 0) {
      Path &s=get_active_path(i-1);
      if (e < s.errors || (e == s.errors && pos >= s.position))
	break;
      active_path[i] = active_path[i-1];
      i--;
    }
    active_path[i] = sn;

    if (final) {
      // Is the new analysis better than the previous ones?
      if (complete_path.size() > 0 && first_complete_path().errors > e)
	complete_path.clear(); // delete all the previous analyses
      complete_path.push_back(sn);
    }
  }


  /*******************************************************************/
  /*                                                                 */
  /*  Agenda::add_analysis                                           */
  /*                                                                 */
  /*******************************************************************/
  
  void Agenda::add_analysis( int sn, CAnalysis &ana )

  {
    Path &cs=path[sn];
    if (!cs.is_start()) {
      add_analysis( cs.previous, ana );
      ana.push_back(cs.arc_number);
    }
  }
  

  /*******************************************************************/
  /*                                                                 */
  /*  Agenda::extract_analyses                                       */
  /*                                                                 */
  /*******************************************************************/
  
  void Agenda::extract_analyses( vector<CAnalysis> &analyses )

  {
    analyses.resize(complete_path.size());
    for( size_t i=0; i<complete_path.size(); i++ )
      add_analysis((int)complete_path[i], analyses[i]);
  }


  /*******************************************************************/
  /*                                                                 */
  /*  CompactTransducer::robust_analyze_string                       */
  /*                                                                 */
  /*******************************************************************/
  
  float CompactTransducer::robust_analyze_string( char *string, 
						  vector<CAnalysis> &analyses,
						  float ErrorsAllowed )
  {
    analyses.clear();
  
    // convert the input string to a sequence of symbols
    vector<Character> input;
    alphabet.string2symseq( string, input );

    // initialize the agenda
    Agenda agenda( ErrorsAllowed );

    // start the analysis
    while (!agenda.finished()) {

      // get the highest ranked search path
      unsigned int sn=(unsigned)agenda.active_path.back();
      Path cs=agenda.path[sn];
      agenda.active_path.pop_back();

      unsigned int state=cs.is_start()? 0: target_node[cs.arc_number];

      // for all transitions from the current state
      for( unsigned int i=first_arc[state]; i<first_arc[state+1]; i++ ) {
	Label l = label[i];               // label of the transition
	Character tc = l.upper_char();    // surface symbol

	if (cs.position == input.size()) {
	  if (tc == Label::epsilon)  // epsilon transition
	    agenda.add_path(i, cs.position, cs.errors, sn, 
			    finalp[target_node[i]]);

	  else  // insertion of symbol
	    agenda.add_path(i, cs.position, cs.errors + insertion_error(tc),
			    sn, finalp[target_node[i]]);
	}

	else {
	  Character ic = input[cs.position];

	  if (tc == Label::epsilon) // epsilon transition
	    agenda.add_path(i, cs.position, cs.errors, sn, false);
	  else if (tc == ic) { // matching symbols
	    bool f=(cs.position+1==input.size() && finalp[target_node[i]]);
	    agenda.add_path(i, cs.position+1, cs.errors, sn, f);
	  }

	  else {
	    // symbol mismatch
	    bool f=(cs.position+1==input.size() && finalp[target_node[i]]);
	    agenda.add_path(i, cs.position+1, cs.errors+mismatch_error(tc, ic),
			    sn, f);
	  
	    // deletion of symbol
	    f = (cs.position+1==input.size() && 
		 finalp[target_node[cs.arc_number]]);
	    agenda.add_path(cs.arc_number, cs.position+1,
			    cs.errors+deletion_error(ic), cs.previous, f);
	  
	    // insertion of symbol
	    f = (cs.position==input.size() && finalp[target_node[i]]);
	    agenda.add_path(i, cs.position, cs.errors + insertion_error(tc),
			    sn, f);
	  }
	}
      }
    }

    if (agenda.complete_path.size() == 0)
      return 0.0;
    agenda.extract_analyses( analyses );
    return agenda.first_complete_path().errors;
  }
}
