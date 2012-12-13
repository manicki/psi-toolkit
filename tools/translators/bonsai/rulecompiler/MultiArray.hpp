#ifndef INTMULTIARRAY_H__
#define INTMULTIARRAY_H__

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include <boost/regex.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>

#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/flat_map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

namespace poleng
{

namespace bonsai
{

typedef std::vector<int> SubArray;
typedef SubArray::iterator SubIterator;
    
class MultiArray {
  protected:
    std::string pack(SubArray &v) {
        size_t len = v.size() * sizeof(int);
        
        char *p;
        char *data_buffer = new char[len];
    
        p = data_buffer;
        for(SubIterator it = v.begin(); it != v.end(); it++) {
            int value = *it;
            memcpy(p, &value, sizeof(int));
            p += sizeof(int);
        }
        return std::string(data_buffer,len);
    }
    
    std::string pack(int value) {
        size_t len = sizeof(int);
        char *data_buffer = new char[len];
        memcpy(data_buffer, &value, sizeof(int));
        return std::string(data_buffer,len);
    }
    
    SubArray unpack(std::string s) {
        size_t len = s.size();
        SubArray v;
        
        const char* sc = s.c_str();
        for(const char* p = sc; p < sc+len; p += sizeof(int)) {
            int value;
            memcpy(&value, p, sizeof(int));
            v.push_back(value);
        }
        return v;
    }
    
  public:
    virtual SubArray range(int) = 0;
    virtual void insert(int, SubArray &) = 0;
    virtual void insert(int, int) = 0;
    virtual void open(std::string) = 0;
    virtual void text2bin(std::istream&, std::string, int) = 0;
    virtual void text2bin(std::ifstream&, std::string, int) = 0;
};

class SerialMultiArray : public MultiArray {  
  private:
    std::multimap<int,int>* array;
    
  public:
    SerialMultiArray() {
	array = new std::multimap<int,int>();
    }
    
    SubArray range(int key) {
        std::pair<std::multimap<int,int>::iterator, std::multimap<int,int>::iterator> r = array->equal_range(key);
        SubArray a;
        while(r.first != r.second)
            a.push_back((r.first++)->second);
        return a;
    }
    
    void insert(int key, SubArray &a) {
        for(SubIterator it = a.begin(); it != a.end(); it++)
          array->insert(array->end(), std::make_pair(key, *it));
    }
    
    void insert(int key, int value) {
        array->insert(array->end(), std::make_pair(key, value));
    }
    
    void open(std::string inf) {
	std::ifstream in(inf.c_str());
	load(in);
    }
    
    void text2bin(std::istream& in, std::string out, int size = 0) {
	load_text(in);
	save(out);
    }
    
    void text2bin(std::ifstream& in, std::string out, int size = 0) {
	load_text(in);
	save(out);
    }
    
    template<class IStream>
    void load_text(IStream &in) {
	std::string line;
	int c = 1;
	while(getline(in,line)) {
	    std::vector<int> input;      
	    boost::regex re("(\\d+)");
	    boost::sregex_token_iterator it(line.begin(), line.end(), re, 1);
	    boost::sregex_token_iterator end;
	
	    while(it != end)
	      input.push_back(boost::lexical_cast<int>(*it++));
	
	    insert(input[0],input[1]);
	    if(c % 100000 == 0)
		std::cerr << c << " pairs read" << std::endl;
	    c++;
	}
    }
    
    template<class IStream>
    void load(IStream &in) {
        boost::archive::binary_iarchive ia(in);
        ia >> *this;
    }
    
    void save(std::string outf) {
        std::ofstream out(outf.c_str());
	save(out);
    }
    
    template<class OStream>
    void save(OStream &out) {
	boost::archive::binary_oarchive oa(out);
	oa << *this;
    }
    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & array;
    }    

};

namespace ip = boost::interprocess;
typedef int  KeyType;
typedef int  MappedType;
typedef std::pair<int, int> ValueType;
typedef ip::allocator<ValueType, ip::managed_mapped_file::segment_manager> ShmemAllocator;
typedef ip::flat_multimap<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> FlatMap;

class MappedMultiArray : public MultiArray {  
  private:    
    
    FlatMap* array;
    std::string file;
    int size;

  public:
    MappedMultiArray() : size(20000000) {}
    
    SubArray range(int key) {
        std::pair<FlatMap::iterator, FlatMap::iterator> r = std::make_pair(array->lower_bound(key), array->upper_bound(key));
        SubArray a;
        while(r.first != r.second)
            a.push_back((r.first++)->second);
        return a;
    }
    
    void insert(const int key, SubArray &a) {
        for(SubIterator it = a.begin(); it != a.end(); it++) {
            ValueType p(key, *it);
            array->insert(array->end(),p);
        }
    }
    
    void insert(int key, int value) {
        ValueType p(key, value);
        array->insert(array->end(),p);
    }
    
    void open(std::string inf) {
	file = inf;
      	ip::managed_mapped_file* mfile = new ip::managed_mapped_file(ip::open_only, file.c_str());
        ShmemAllocator* alloc = new ShmemAllocator(mfile->get_segment_manager());
        array = mfile->find_or_construct<FlatMap>("map")(std::less<int>(), *alloc);
    }
    
    void text2bin(std::istream& in, std::string out, int size_ = 20000000) {
        file = out;
	size = size_;
	load_text(in);
    }
    
    void text2bin(std::ifstream& in, std::string out, int size_ = 20000000) {
	file = out;
	size = size_;
	load_text(in);
    }
    
    template<class IStream>
    void load_text(IStream &in) {
	ip::managed_mapped_file* mfile = new ip::managed_mapped_file(ip::open_or_create, file.c_str(), size);
        ShmemAllocator *alloc = new ShmemAllocator(mfile->get_segment_manager());
        array = mfile->find_or_construct<FlatMap>("map")(std::less<int>(), *alloc);
    
	std::string line;
	int c = 1;
	while(getline(in,line)) {
	    std::vector<int> input;      
	    boost::regex re("(\\d+)");
	    boost::sregex_token_iterator it(line.begin(), line.end(), re, 1);
	    boost::sregex_token_iterator end;
	
	    while(it != end)
	      input.push_back(boost::lexical_cast<int>(*it++));
	
	    insert(input[0],input[1]);
	    if(c % 100000 == 0)
		std::cerr << c << " pairs read" << std::endl;
	    c++;
	}
	ip::managed_mapped_file::shrink_to_fit(file.c_str());
    }
};

}

}

#endif
