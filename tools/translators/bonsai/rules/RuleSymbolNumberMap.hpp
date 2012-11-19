#ifndef RULESYMBOLNUMBERMAP_H__
#define RULESYMBOLNUMBERMAP_H__

#include <iostream>
#include <fstream>
#include <vector>

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "../common/TransferTypes.hpp"

namespace poleng
{

namespace bonsai
{

namespace rules {
  
    class RuleSymbol {
      public:
        RuleSymbol();
        RuleSymbol(Label);
        RuleSymbol(Labels);
        
        std::string label() const;
        std::string label(int) const;
        std::string str() const;
        
        int src_index();
        
        Labels& get_labels();
        
      private:
        std::string full_symbol;
        Labels labels;
        
        int index;
        friend class boost::serialization::access;
        
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & full_symbol;
            ar & labels;
            ar & index;
        }
    };
    
    struct RuleSymbolSorter {
        bool operator()(const RuleSymbol &sym1, const RuleSymbol &sym2) const {
            return sym1.str() < sym2.str();
        }
    };
    
    typedef boost::bimap<int, boost::bimaps::set_of<RuleSymbol, RuleSymbolSorter> > RuleSymbolBimap;
    typedef RuleSymbolBimap::value_type RuleSymbolNumber;
    
    class RuleSymbolNumberMap {
      private:
        RuleSymbolBimap bm;
        
      public:
        int size();
        void add(int, RuleSymbol);
        
        int get_number_by_string(std::string);
        int get_number_by_symbol(RuleSymbol);
        
        RuleSymbol get_symbol_by_number(int);
        std::string get_label_by_number(int);
        std::string get_label_by_number(int, int);
            
        void save(std::string filename);
        
        template <class IStream>
        void open(IStream &in) {
            boost::archive::binary_iarchive ia(in);
            ia >> bm;
        }
        void open(std::string filename);
    
        template <class IStream>    
        void create_from_text(IStream &in) {
            int c = 0;
            std::string line;
            while(getline(in,line)) {
                RuleSymbol rsym(line);
                add(c++, rsym);
            }
        }
        
        template <class OStream>
        void write_to_text(OStream &out) {        
            for( RuleSymbolBimap::iterator it = bm.begin(); it != bm.end(); it++) {
                RuleSymbol sym_t = it->right;
                out << sym_t.str() << std::endl;
            }
        }
    
    };

}

}

}

#endif
