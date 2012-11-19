#include "Register.hpp"

namespace poleng
{

namespace bonsai
{

int Register::load_index = 0;

Register::Register(int sig_length_) : sig_length(sig_length_), verbose(false), accesses(0), actual_load_index(load_index) {
    if(verbose) std::cerr << "New register for " << sig_length << " created" << std::endl;
    unflush();
}

Register::~Register() {
    flush();
}

void Register::register_state(StateSignature p) {
    if(verbose) std::cerr << "Adding signature for state " << p.state << std::endl;
    sig.insert(p);
    accesses++;
    if(verbose) std::cerr << "Number of signatures: " << size() << std::endl;
}

int Register::get_load_index() const { return actual_load_index; }

int Register::find_equivalent(StateSignature p) {
    Signatures::iterator it = sig.find(p);
    accesses++;
    if(it != sig.end())
        return it->state;
    else
        return -1;
}

void Register::unflush() {
    if(verbose) std::cerr << "Unflushing memory (act_size: " << size() << ")" << std::endl;
    char filename[50];
    int length = std::sprintf(filename, "%d.reg", sig_length);
    std::ifstream in;
    in.open(filename, std::ios::binary);
    
    if(!in.fail()) {
        boost::archive::binary_iarchive ia(in);
        ia >> sig;
        ia >> accesses;
    
        if(verbose) std::cerr << "size of sig: " << sig.size() << std::endl;
    
        in.close();            
    }
    else {
        if(verbose) std::cerr << "File " << filename << " not found" << std::endl;
    }
    actual_load_index = load_index;
    load_index++;
}

void Register::flush() {
    if(verbose) std::cerr << "Flushing memory" << std::endl;
    char filename[50];
    int length = std::sprintf(filename, "%d.reg", sig_length);
    
    std::ofstream out;
    out.open(filename, std::ios::binary);
    
    boost::archive::binary_oarchive oa(out);
    oa << sig;
    oa << accesses;

    out.close();
    
    sig.clear();
}

RegisterPtr RegisterManager::request_register(int length) {
    
    if(registers.count(length) == 0) {
        registers[length] = RegisterPtr( new Register( length ) );
        loaded.insert(registers[length]);
        //std::cerr << "loaded size: " << loaded.size() << std::endl;
    }

    if(loaded.size() > max) {
            LoadedRegisters::iterator reg_it = loaded.begin();
            loaded.erase(reg_it);
            registers.erase((*reg_it)->length());
            //std::cerr << "Registers: " << registers.size() << std::endl;
    }
    
    if(all.count(length) == 0)
        all.insert(length);
        
    return registers[length];
}

RegisterManager::RegisterManager(int max_) : max(max_) {
    Register::set_load_index(0);        
}

RegisterManager::~RegisterManager() {
    registers.clear();
    loaded.clear();
    for(std::set<int>::iterator it = all.begin(); it != all.end(); it++) {
        char filename[50];
        int length = std::sprintf(filename, "%d.reg", *it);
        std::remove(filename);        
    }
}

void RegisterManager::register_state(StateSignature p) {
    RegisterPtr reg = request_register(p.sig_length);
    reg->register_state(p);
}

int RegisterManager::find_equivalent(StateSignature p) {
    RegisterPtr reg = request_register(p.sig_length);
    return reg->find_equivalent(p);
}

}

}

