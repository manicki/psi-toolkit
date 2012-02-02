#include "key_value_store.hpp"

#include <assert.h>
#include <vector>

KeyValueStore::Builder::Builder()
    :store_(new KeyValueStore()) {
}

KeyValueStore::Builder::~Builder() {
    delete store_;
}

void KeyValueStore::Builder::add(const std::string& key, const std::string& value) {
    assert(store_ != 0);

    store_->perfectHashIndex_.addKey(key);
    valuesInOrder_.push_back(value);
}

KeyValueStore* KeyValueStore::Builder::build() {

    store_->perfectHashIndex_.create();

    size_t totalSize = store_->perfectHashIndex_.getSize();

    std::vector<size_t> translationTable(totalSize);

    for (size_t i = 0; i < totalSize; ++i) {
        const char* key = store_->perfectHashIndex_.getKey(i);

        translationTable[store_->perfectHashIndex_.getHash(key)] = i;
    }

    for (size_t i = 0; i < totalSize; ++i) {
        store_->stringVector_.push_back(valuesInOrder_[translationTable[i]]);
    }

    store_->perfectHashIndex_.clearKeys();

    KeyValueStore* ret = store_;
    store_ = 0;
    return ret;
}

boost::optional<std::string> KeyValueStore::get(const std::string& key) const {
    size_t index = perfectHashIndex_[key];

    if (index == perfectHashIndex_.getSize())
        return boost::optional<std::string>();

    return boost::optional<std::string>(stringVector_[index].str());
}


void KeyValueStore::load(std::FILE* mphf) {
    perfectHashIndex_.load(mphf);
    stringVector_.load(mphf);

    assert(perfectHashIndex_.getSize() == stringVector_.size());
}

void KeyValueStore::save(std::FILE* mphf) const {
    perfectHashIndex_.save(mphf);
    stringVector_.save(mphf);
}
