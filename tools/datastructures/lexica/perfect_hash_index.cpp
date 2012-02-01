#include "perfect_hash_index.hpp"

#include <assert.h>

static int key_phrasetable_read(void* data, char** key, cmph_uint32* keylen) {
    FILE* fd = (FILE*)data;
    *key = NULL;
    *keylen = 0;
    while (1) {
        char buf[BUFSIZ];
        char* c = fgets(buf, BUFSIZ, fd);

        if (c == NULL)
            return -1;
        if (feof(fd))
            return -1;

        *key = (char*)realloc(*key, *keylen + strlen(buf) + 1);
        memcpy(*key + *keylen, buf, strlen(buf));
        *keylen += (cmph_uint32)strlen(buf);

        if (buf[strlen(buf) - 1] != '\n')
            continue;

        break;
    }

    if ((*keylen) && (*key)[*keylen - 1] == '\n') {
        (*key)[(*keylen) - 1] = 0;
        --(*keylen);
    }

    return (int)(*keylen);
}

static void key_phrasetable_dispose(void* /*data*/, char* key, cmph_uint32 /*keylen*/) {
    free(key);
}

static void key_phrasetable_rewind(void* data) {
    std::FILE* fd = (FILE*) data;
    std::rewind(fd);
}

static cmph_uint32 count_phrasetable_keys(std::FILE* fd) {
    cmph_uint32 count = 0;
    rewind(fd);
    while (1) {
        char buf[BUFSIZ];
        fgets(buf, BUFSIZ, fd);
        if (feof(fd))
            break;
        if (buf[strlen(buf) - 1] != '\n')
            continue;
        ++count;
    }

    rewind(fd);
    return count;
}

cmph_io_adapter_t *cmph_io_phrasetable_adapter(std::FILE* phrasetable_fd) {
    cmph_io_adapter_t* key_source = (cmph_io_adapter_t*)malloc(sizeof(cmph_io_adapter_t));
    assert(key_source);
    key_source->data    = (void*)phrasetable_fd;
    key_source->nkeys   = count_phrasetable_keys(phrasetable_fd);
    key_source->read    = key_phrasetable_read;
    key_source->dispose = key_phrasetable_dispose;
    key_source->rewind  = key_phrasetable_rewind;
    return key_source;
}

void PerfectHashIndex::clearKeys() {
    for (std::vector<char*>::iterator it = m_keys.begin(); it != m_keys.end(); ++it)
        delete [] *it;
    m_keys.clear();
}

void PerfectHashIndex::create() {
    calcHashFunction();
    calcFprints();
}

void PerfectHashIndex::save(std::string filename) {
    std::FILE* mphf = std::fopen(filename.c_str(), "w");
    save(mphf);
    std::fclose(mphf);
}

void PerfectHashIndex::save(std::FILE* mphf) {
    cmph_dump(m_hash, mphf);

    size_t nkeys = m_fprints.size();
    std::fwrite(&nkeys, sizeof(nkeys), 1, mphf);
    std::fwrite(&m_fprints[0], sizeof(Fprint), nkeys, mphf);
}

void PerfectHashIndex::load(std::string filename) {
    std::FILE* mphf = std::fopen(filename.c_str(), "r");
    load(mphf);
    std::fclose(mphf);
}

void PerfectHashIndex::load(std::FILE* mphf) {
    m_hash = cmph_load(mphf);

    size_t nkeys;
    std::fread(&nkeys, sizeof(nkeys), 1, mphf);
    m_fprints.resize(nkeys, 0);
    std::fread(&m_fprints[0], sizeof(Fprint), nkeys, mphf);
}
