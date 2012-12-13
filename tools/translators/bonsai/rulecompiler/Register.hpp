#ifndef REGISTER_H__
#define REGISTER_H__

#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <cstdio>

#include <boost/shared_ptr.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>

namespace poleng
{

namespace bonsai
{

struct StateSignature {
    int state; 
    int sig_length;
    int* signature;
        
    StateSignature() : sig_length(0), signature(NULL) {};
    StateSignature(int length) : sig_length(length), signature(new int[length*2+1]) {}
    
    StateSignature(const StateSignature &s) {
	state = s.state;
	sig_length = s.sig_length;
	signature = new int[sig_length*2+1];
        memcpy(signature, s.signature, (sig_length*2+1)*sizeof(int));
    }
    
    ~StateSignature() {
	delete [] signature;
	signature = NULL;
    }

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const {
        ar & state;
        ar & sig_length;
        
        for(int i = 0; i < sig_length*2+1; i++)
            ar & signature[i];
    }
    
    template<class Archive>
    void load(Archive & ar, const unsigned int version) {
        ar & state;
        ar & sig_length;
        
        signature = new int[sig_length*2+1];
        for(int i = 0; i < sig_length*2+1; i++)
            ar & signature[i];
    }
    
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

struct SignatureSorter {
    bool operator() (const StateSignature &s1, const StateSignature &s2) const {
	char* s1p = (char*)s1.signature;
	char* s2p = (char*)s2.signature;
	
	int size1 = (s1.sig_length * 2 + 1) * sizeof(int);
	int size2 = (s2.sig_length * 2 + 1) * sizeof(int);
	
	if(size1 == size2) {
	    return (std::memcmp(s1p,s2p,size1) < 0);
	}
	else {
	    return size1 < size2;
	}
    }
};

typedef std::set<StateSignature, SignatureSorter> Signatures;


class Register {
  private:    
    Signatures sig;
    
    int sig_length;
    int accesses;
    
    int actual_load_index;
    bool verbose;
    
    static int load_index;
    
  public:
    Register(int);
    ~Register();
    
    void register_state(StateSignature);
    int find_equivalent(StateSignature);

    void unflush();
    void flush();
    
    static void set_load_index(int val) {
        load_index = val;
    }
    
    int get_load_index() const;
    
    inline int length() { return sig_length; }
    inline int size() { return sig.size(); }
    inline int bytes() { return (8*sig_length+8)*sig.size(); } 
};

typedef boost::shared_ptr<Register> RegisterPtr;

class RegisterManager {
  private:
    typedef std::map<int, RegisterPtr> Registers;
    
    struct RegisterSorter {
        bool operator() (const RegisterPtr &r1, const RegisterPtr &r2) const {
            return r1->get_load_index() < r2->get_load_index();
        }
    };

    typedef std::set<RegisterPtr, RegisterSorter> LoadedRegisters;
    
    RegisterPtr request_register(int);
    
    Registers registers;
    std::set<int> all;
    LoadedRegisters loaded;
    int max;
    
  public:
    RegisterManager(int);
    ~RegisterManager();
    void register_state(StateSignature);
    int find_equivalent(StateSignature);

};

}

}

#endif
