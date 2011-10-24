#ifndef DFSATYPES_HPP__
#define DFSATYPES_HPP__

#include <vector>
#include <set>
#include <boost/tuple/tuple.hpp>
    
namespace psi {

    typedef int State;    
    typedef unsigned int Symbol;
    typedef unsigned int Weight;
    
    const Symbol EPS = -1;
    const Symbol ANY = -2;
    const Symbol ERROR = -3;
    
    template <typename SymbolT = Symbol,
              typename StateT  = State>
    class Arc {
      public:  
        typedef SymbolT symbol_type;
        typedef StateT  state_type;
        
      private:
        SymbolT m_a; StateT m_q;
        
      public:
        Arc() {}
        Arc(SymbolT a, StateT q) : m_a(a), m_q(q) {}
        
        inline SymbolT getSymbol() const { return m_a; }
        inline StateT getDest() const { return m_q; }
        
        inline void setSymbol(SymbolT a) { m_a = a; }
        inline void setDest(StateT q) { m_q = q; }
    };
    
    template <typename SymbolT = Symbol,
              typename StateT  = State,
              typename WeightT = Weight>
    class ArcWeighted : public Arc<SymbolT, StateT> {
      public:
        typedef WeightT weight_type;
      
      private:
        WeightT m_w;
        
      public:
        ArcWeighted(SymbolT a, StateT q, WeightT w) : Arc<SymbolT, StateT>(a, q), m_w(w) {}
        
        inline WeightT getWeight() const { return m_w; }
        
        inline void setWeight(WeightT w) { m_w = w; }
    };
    
    struct ArcSorter {
        template <typename ArcT>
        bool operator()(ArcT lhs, ArcT rhs) const {
            return (lhs.getSymbol() < rhs.getSymbol() || lhs.getDest() < rhs.getDest());
        }
    };
    
    template <typename ArcIteratorT>
    class ArcRange : public std::pair<ArcIteratorT, ArcIteratorT> {
      public:
        typedef ArcIteratorT iterator_type;
        
        ArcRange(ArcIteratorT begin, ArcIteratorT end)
          : std::pair<ArcIteratorT, ArcIteratorT>(begin, end) {};
    };
}

#endif
