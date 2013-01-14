#ifndef DFSA_HDR
#define DFSA_HDR

#include "NDFSA.hpp"

namespace psi {

    template <typename ArcT = ArcWeighted<Symbol, State, Weight> >
    class DFSA : public NDFSA<ArcT> {
      protected:
        typedef typename NDFSA<ArcT>::Arcs Arcs;
        typedef typename NDFSA<ArcT>::States States;

      public:
        typedef typename NDFSA<ArcT>::state_type state_type;
        typedef typename NDFSA<ArcT>::arc_type arc_type;
        typedef typename NDFSA<ArcT>::symbol_type symbol_type;
        typedef typename NDFSA<ArcT>::arc_iterator_type arc_iterator_type;

/*******************************************************************************
## Constructor

  DFSA();

Basic constructor. Creates a deterministic finite-state automaton and an
underlying non-deterministic finite-state automaton.

*******************************************************************************/
        DFSA() : NDFSA<ArcT>::NDFSA() {};

        DFSA<ArcT>& operator=(const NDFSA<ArcT> &ndfsa) {
            NDFSA<ArcT>::operator=(ndfsa);
            return *this;
        }

/*******************************************************************************
## Deterministic inclusion function

    template <typename InputIterator> bool in(InputIterator, InputIterator);

Checks whether given sequence of labels belongs to the language of the automaton.
    
*******************************************************************************/
        template <typename InputIterator>
        bool in(InputIterator it, InputIterator end) {
            state_type current_state = 0;
            while (it != end) {
                state_type next_state = delta(current_state, *it);
                if (next_state != state_type(-1))
                    current_state = next_state;
                else
                    return false;
                it++;
            }
            return this->isEndState(current_state);
        }

/*******************************************************************************
## Deterministic delta transtion function

    state_type delta(state_type p, symbol_type a) const;
    
*******************************************************************************/
        state_type delta(state_type p, symbol_type a) const {
            if (NDFSA<ArcT>::m_states.size() < (size_t)p)
                return state_type(-1);

            arc_iterator_type arc = this->find(p, a);
            if (arc == NDFSA<ArcT>::m_states[p]->end())
                return state_type(-1);
            else
                return arc->getDest();
        }

    };
}

#endif
