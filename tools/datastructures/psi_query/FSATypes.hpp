#ifndef FSATYPES_HDR
#define FSATYPES_HDR

namespace psi {

    typedef int State;
    typedef char Symbol;
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

        inline const SymbolT& getSymbol() const { return m_a; }
        inline const StateT& getDest() const { return m_q; }

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
            return (lhs.getSymbol() < rhs.getSymbol()
                || (lhs.getSymbol() == rhs.getSymbol()
                && lhs.getDest() < rhs.getDest()));
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

template <class OStream, typename SymbolT, typename StateT>
OStream& operator<<(OStream &o, psi::Arc<SymbolT, StateT> a) {
  o << a.getDest() << " " << (long)a.getSymbol() << std::endl;
  return o;
}

#endif
