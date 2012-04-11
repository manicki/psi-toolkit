#ifndef DFSA_HPP__
#define DFSA_HPP__

#include "NDFSA.hpp"

namespace psi {

    template <typename StateT = State, typename ArcT = Arc<Symbol, State> >
    class DFSA : public NDFSA<StateT, ArcT> {
      protected:
        typedef typename NDFSA<StateT, ArcT>::Arcs Arcs;
        typedef typename NDFSA<StateT, ArcT>::States States;

      public:
        typedef typename NDFSA<StateT, ArcT>::state_type state_type;
        typedef typename NDFSA<StateT, ArcT>::arc_type arc_type;
        typedef typename NDFSA<StateT, ArcT>::symbol_type symbol_type;
        typedef typename NDFSA<StateT, ArcT>::arc_iterator_type arc_iterator_type;

        DFSA() : NDFSA<State, ArcT>::NDFSA() {};

        DFSA<StateT, ArcT>& operator=(const NDFSA<StateT, ArcT> &ndfsa) {
            NDFSA<State, ArcT>::operator=(ndfsa);
            return *this;
        }

        template <typename InputIterator>
        bool in(InputIterator it, InputIterator end) {
            state_type current_state = 0;
            while(it != end) {
                state_type next_state = delta(current_state, *it);
                if (next_state != state_type(-1))
                    current_state = next_state;
                else
                    return false;
                it++;
            }
            return isEndState(current_state);
        }

        state_type delta(state_type p, symbol_type a) const {
            if (NDFSA<StateT, ArcT>::m_states.size() < (size_t)p)
                return state_type(-1);

            arc_iterator_type arc = find(p, a);
            if (arc == NDFSA<StateT, ArcT>::m_states[p]->end())
                return state_type(-1);
            else
                return arc->getDest();
        }

    };
}

#endif
