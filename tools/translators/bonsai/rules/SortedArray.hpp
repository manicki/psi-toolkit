#ifndef SORTEDARRAY_HPP__
#define SORTEDARRAY_HPP__

#include <cstring>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/lexical_cast.hpp>

namespace poleng
{
namespace bonsai
{

namespace rules {

    template <class ValT, class TailT>
    struct CheckPoint {
        ValT index;
        ValT base;
        TailT *tail;
        
        CheckPoint() : index(0), base(0), tail((TailT*)malloc(sizeof(TailT))) {
            tail[0] = TailT(0);
        }
        
        CheckPoint(ValT index_, ValT base_) : index(index_), base(base_), tail((TailT*)malloc(sizeof(TailT))) {
            tail[0] = TailT(0);
        }
        
        CheckPoint(const CheckPoint &cp) {
            index = cp.index;
            base = cp.base;
            
            ValT length = cp.size() + 1;
            
            tail = (TailT*)malloc(sizeof(TailT)*length);
            std::memcpy(tail, cp.tail, sizeof(TailT)*length);
        }
        
        ~CheckPoint() { free(tail); }
        
        ValT size() const {
            return (ValT)tail[0];
        }
        
        void set_index(ValT val) { index = val; }
        void set_base(ValT val) { base = val; }
        
        ValT get_index() const { return index; }
        ValT get_base() const { return base; }
        
        ValT at(ValT val) {
            TailT i = (TailT)(val-index);
            if(i < size()) {
                return ValT(((ValT)tail[i+1])+base);
            }
            else
                return ValT(-1);
        }
        
        void push_back(ValT val) {
            TailT length = size() + 2;
            tail = (TailT*)realloc(tail, sizeof(TailT)*length);
            tail[length-1] = (TailT)(val-base);
            tail[0]++;
        }
            
        friend class boost::serialization::access;
        
        template<class Archive>
        void save(Archive & ar, const unsigned int version) const {
            ar & index;
            ar & base;
            
            for(int i = 0; i < tail[0]+1; i++)
                ar & tail[i];
        }
        
        template<class Archive>
        void load(Archive & ar, const unsigned int version) {
            ar & index;
            ar & base;
            
            ar & tail[0];
            int length = tail[0]+1;
            tail = (TailT*)realloc(tail, length*sizeof(TailT));
            for(int i = 1; i < length; i++)
                ar & tail[i];
        }
        BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    };    
    
    template <class ValT, class TailT>
    struct CheckPointCompare {
        bool operator()(const CheckPoint<ValT, TailT> &cp1, const CheckPoint<ValT, TailT> &cp2) {
            return cp1.get_index() < cp2.get_index();
        }
    };
    
    template <class ValT, class TailT>
    class SortedArray {
      private:
        typedef std::vector<CheckPoint<ValT, TailT> > CheckPoints; 
        CheckPoints cps;
    
      public:
        template <class IStream>
        void read_binary_data(IStream &in) {
            ValT count = ValT(0);
            
            ValT base;
            while(in.read((char*)&base, sizeof(ValT))) {            
                CheckPoint<ValT, TailT> cp(count, base);
                
                ValT act_value = base;
                while(act_value - base < std::pow((double)2,(double)8*sizeof(TailT))) {
                    cp.push_back(act_value);
                    
                    if(in.read((char*)&act_value, sizeof(ValT)) == 0) {
                        break;
                    }
                    count++;
                    
                    if(count % 1000000) {
                        std::cerr << "Mapped " << count << " integers" << std::endl;
                    }
                }
                in.seekg((int)in.tellg()-(int)sizeof(ValT));
                cps.push_back(cp);
            }
            std::cerr << "cps size: " << cps.size() << std::endl;
        }
        
        template <class IStream>
        void read_text_data(IStream &in) {
            ValT count = ValT(0);
            
            std::string line;
            std::getline(in, line);
            
            ValT base = boost::lexical_cast<ValT>(line);
            
            typedef boost::shared_ptr<CheckPoint<ValT, TailT> > CheckPointPtr;
            CheckPointPtr cp_ptr( new CheckPoint<ValT, TailT>(count, base) );
            cp_ptr->push_back(base);
            
            while(std::getline(in, line)) {
                count++;
                ValT act_value = boost::lexical_cast<ValT>(line);
                
                if(act_value - base < std::pow((double)2,(double)8*sizeof(TailT))) {
                    cp_ptr->push_back(act_value);
                }
                else {
                    cps.push_back(*cp_ptr);
                    base = act_value;
                    cp_ptr.reset( new CheckPoint<ValT, TailT>(count,base) );
                    cp_ptr->push_back(base);
                }
                if(count % 1000000 == 0) {
                    std::cerr << "Mapped " << count << " integers" << std::endl;
                }
            }
            cps.push_back(*cp_ptr);
            std::cerr << "cps size: " << cps.size() << std::endl;
        }
        
        ValT at(ValT i) {
            CheckPoint<ValT,TailT> cmp(i,0);
            typename CheckPoints::iterator it = std::lower_bound(cps.begin(), cps.end(), cmp, CheckPointCompare<ValT,TailT>());
            
            if(it == cps.end() or it->get_index() > i)
                it--;
            return it->at(i);
        }
        
        template <class OStream>
        void to_txt(OStream &out) {
            ValT total_size = 0;
            for(ValT i = 0; i < cps.size(); i++)
                total_size += cps[i].size();
                
            for(ValT i = 0; i < total_size; i++)
                out << at(i) << std::endl;
        }
        
        template <class OStream>
        void save(OStream &out) {
            boost::archive::binary_oarchive oa(out);
    
            oa << cps;    
        }
    
        template <class IStream>
        void open(IStream &in) {
            boost::archive::binary_iarchive ia(in);
    
            ia >> cps;    
        }
                
        void open(std::string infile) {
            std::ifstream in(infile.c_str());
            open(in);    
        }
    };

    typedef SortedArray<unsigned int, unsigned char> CharSortedArray;
}

}

}
#endif
