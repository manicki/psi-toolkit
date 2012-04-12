#ifndef BinDFSA_HDR
#define BinDFSA_HDR

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
#include <boost/foreach.hpp>

#include "FSATypes.hpp"

namespace psi {

    template <typename ArcT = Arc<Symbol, size_t>, typename PosT = size_t>
    class BinDFSA {
      protected:
        typedef std::vector<PosT> States;
        typedef std::vector<ArcT> Arcs;

      public:
        typedef size_t state_type;
        typedef ArcT arc_type;
        typedef typename ArcT::symbol_type symbol_type;
        typedef typename Arcs::const_iterator arc_iterator_type;

      protected:
        States m_states;
        Arcs m_arcs;

        PosT setLastBit(PosT) const;
        PosT unsetLastBit(PosT) const;

        arc_iterator_type find(size_t, symbol_type) const;

      public:
        BinDFSA();

        size_t delta(size_t, symbol_type) const;

        template <typename InputIterator>
        bool in(InputIterator, InputIterator);

        const std::set<size_t> getStartStates() const;
        bool isStartState(size_t) const;

        bool isEndState(size_t) const;
        const std::set<size_t> getEndStates() const;

        const ArcRange<arc_iterator_type> getArcs(size_t) const;

        void setStartState(size_t);
        void unsetStartState(size_t);

        void setEndState(size_t);
        void unsetEndState(size_t);

        size_t addState(bool = false);
        void addArc(size_t, arc_type);

        size_t size() const;
        void swap(BinDFSA &);
        void print();

    };

    template <typename ArcT, typename PosT>
    void BinDFSA<ArcT, PosT>::setStartState(size_t p) {}

    template <typename ArcT, typename PosT>
    void BinDFSA<ArcT, PosT>::unsetStartState(size_t p) {}

    template <typename ArcT, typename PosT>
    void BinDFSA<ArcT, PosT>::setEndState(size_t p) {
        m_states[p] = setLastBit(m_states[p]);
    }

    template <typename ArcT, typename PosT>
    void BinDFSA<ArcT, PosT>::unsetEndState(size_t p) {
        m_states[p] = unsetLastBit(m_states[p]);
    }

    template <typename ArcT, typename PosT>
    size_t BinDFSA<ArcT, PosT>::addState(bool start) {
        size_t p = m_states.size();
        m_states.resize(p+1);
        m_states[p] = m_arcs.size();
        return p;
    }

    template <typename ArcT, typename PosT>
    void BinDFSA<ArcT, PosT>::addArc(size_t p, ArcT a) {
        ArcSorter cmp;
        if (p < m_states.size()) {
            PosT pos = unsetLastBit(m_states[p]);
            PosT end = m_arcs.size();

            if (p < m_states.size()-1)
                end = unsetLastBit(m_states[p+1]);

            while (pos < end && cmp(m_arcs[pos], a))
                pos++;
            m_arcs.insert(m_arcs.begin() + pos, a);

            for (size_t q = p+1; q < m_states.size(); q++)
                if (isEndState(q))
                    m_states[q] = setLastBit(unsetLastBit(m_states[q]) + 1);
                else
                    m_states[q]++;
        }
    }

    template <typename ArcT, typename PosT>
    BinDFSA<ArcT, PosT>::BinDFSA() {}

    template <typename ArcT, typename PosT>
    inline size_t BinDFSA<ArcT, PosT>::delta(size_t p, typename ArcT::symbol_type a) const {
        if (BinDFSA<ArcT, PosT>::m_states.size() < p)
            return size_t(-1);

        arc_iterator_type arc = find(p, a);
        if (arc == BinDFSA<ArcT, PosT>::m_arcs->end())
            return size_t(-1);
        else
            return arc->getDest();
    }

    template <typename ArcT, typename PosT>
    inline typename BinDFSA<ArcT, PosT>::arc_iterator_type
    BinDFSA<ArcT, PosT>::find(size_t p, typename ArcT::symbol_type a) const {
        ArcRange<arc_iterator_type> r = getArcs(p);

        ArcT test(a, -1);
        return std::lower_bound(r.first, r.second, test, ArcSorter());
    }

    template <typename ArcT, typename PosT>
    template <typename InputIterator>
    bool BinDFSA<ArcT, PosT>::in(InputIterator it, InputIterator end) {
        size_t current_state = 0;
        while (it != end) {
            size_t next_state = delta(current_state, *it);
            if (next_state != size_t(-1))
                current_state = next_state;
            else
                return false;
            it++;
        }
        return isEndState(current_state);
    }

    template <typename ArcT, typename PosT>
    bool BinDFSA<ArcT, PosT>::isStartState(size_t state) const {
        return state == 0;
    }

    template <typename ArcT, typename PosT>
    PosT BinDFSA<ArcT, PosT>::setLastBit(PosT pos) const {
        return pos | (PosT(1) << (sizeof(PosT)*8-1));
    }

    template <typename ArcT, typename PosT>
    PosT BinDFSA<ArcT, PosT>::unsetLastBit(PosT pos) const {
        return pos & ~(PosT(1) << (sizeof(PosT)*8-1));
    }


    template <typename ArcT, typename PosT>
    bool BinDFSA<ArcT, PosT>::isEndState(size_t state) const {
        return (m_states[state] & (PosT(1) << (sizeof(PosT)*8-1)));
    }

    template <typename ArcT, typename PosT>
    const std::set<size_t> BinDFSA<ArcT, PosT>::getEndStates() const {
        std::set<size_t> end_states;

        for (size_t p = 0; p < m_states.size(); p++)
            if (isEndState(p))
                end_states.insert(p);

        return end_states;
    }

    template <typename ArcT, typename PosT>
    const ArcRange<typename BinDFSA<ArcT, PosT>::arc_iterator_type>
    BinDFSA<ArcT, PosT>::getArcs(size_t p) const {
        if (p < m_states.size()-1) {
            PosT pos1 = unsetLastBit(m_states[p]);
            PosT pos2 = unsetLastBit(m_states[p+1]);

            arc_iterator_type ait1 = m_arcs.begin();
            arc_iterator_type ait2 = m_arcs.begin();

            ait1 += pos1;
            ait2 += pos2;

            return ArcRange<arc_iterator_type>(
                ait1,
                ait2
            );
        }
        else if (p == m_states.size()-1) {
            PosT pos = unsetLastBit(m_states[p]);
            arc_iterator_type ait = m_arcs.begin();
            ait += pos;

            return ArcRange<arc_iterator_type>(
                ait,
                m_arcs.end()
            );
        }
        else {
            return ArcRange<arc_iterator_type>(
                m_arcs.end(),
                m_arcs.end()
            );
        }
    }

    template <typename ArcT, typename PosT>
    size_t BinDFSA<ArcT, PosT>::size() const {
        return m_states.size();
    }

    template <typename ArcT, typename PosT>
    void BinDFSA<ArcT, PosT>::swap(BinDFSA<ArcT, PosT> &fsa) {
        m_states.swap(fsa.m_states);
        m_arcs.swap(fsa.m_arcs);
    }

    template <typename ArcT, typename PosT>
    void BinDFSA<ArcT, PosT>::print() {
        for (size_t p = 0; p < m_states.size(); p++) {
            ArcRange<arc_iterator_type> r = getArcs(p);
            for (arc_iterator_type it = r.first; it != r.second; it++) {
                std::cout << p << "\t" << it->getDest() << "\t" << it->getSymbol()
                    << std::endl;
            }
            if (isEndState(p)) {
                std::cout << p << std::endl;
            }
        }
    }
}

#endif
