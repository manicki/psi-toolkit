#ifndef PERFECT_HASH_INDEX_HDR_HDR
#define PERFECT_HASH_INDEX_HDR_HDR

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>

#include <cmph.h>
#include "murmur_hash3.hpp"

cmph_io_adapter_t *cmph_io_phrasetable_adapter(std::FILE* phrasetable_fd);

class PerfectHashIndex {
private:
    typedef unsigned int Fprint;

    std::vector<char*> m_keys;
    std::vector<Fprint> m_fprints;

    CMPH_ALGO m_algo;
    cmph_t* m_hash;

    void calcHashFunction() {
        cmph_io_adapter_t *source = cmph_io_vector_adapter((char**) &m_keys[0], m_keys.size());

        cmph_config_t *config = cmph_config_new(source);
        cmph_config_set_algo(config, m_algo);
        cmph_config_set_verbosity(config, 5);

        m_hash = cmph_new(config);
        cmph_config_destroy(config);
    }

    Fprint getFprint(const char* key) const {
        Fprint hash;
        MurmurHash3_x86_32(key, std::strlen(key), 100000, &hash);
        return hash;
    }

    void calcFprints() {
        for (std::vector<char*>::iterator it = m_keys.begin(); it != m_keys.end(); ++it) {
            Fprint fprint = getFprint(*it);
            size_t idx = cmph_search(m_hash, *it, (cmph_uint32) strlen(*it));

            if (idx >= m_fprints.size())
                m_fprints.resize(idx + 1);
            m_fprints[idx] = fprint;
        }
    }

public:
    PerfectHashIndex() : m_algo(CMPH_CHD),m_hash(0) {}
    PerfectHashIndex(CMPH_ALGO algo) : m_algo(algo),m_hash(0) {}

    ~PerfectHashIndex() {
        cmph_destroy(m_hash);
        clearKeys();
    }

    size_t getHash(const char* key) const {
        size_t idx = cmph_search(m_hash, key, (cmph_uint32) strlen(key));
        if (getFprint(key) == m_fprints[idx])
            return idx;
        else
            return getSize();
    }

    size_t getHash(std::string key) const {
        return getHash(key.c_str());
    }

    size_t operator[](std::string key) const {
        return getHash(key);
    }

    size_t operator[](char* key) const {
        return getHash(key);
    }

    size_t getHashByIndex(size_t index) const {
        if (index < m_keys.size())
            return getHash(m_keys[index]);
        return getSize();
    }

    void addKey(const char* keyArg) {
        char* key = new char[std::strlen(keyArg)+1];
        std::strcpy(key, keyArg);
        m_keys.push_back(key);
    }

    void addKey(std::string keyArg) {
        addKey(keyArg.c_str());
    }

    void clearKeys();

    void create();

    void save(std::string filename);
    void save(std::FILE* mphf);

    void load(std::string filename);
    void load(std::FILE* mphf);

    size_t getSize() const {
        return m_fprints.size();
    }
};

#endif
