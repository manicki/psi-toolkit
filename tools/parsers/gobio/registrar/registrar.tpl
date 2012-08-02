#ifndef REGISTRAR_TPL_HDR
#define REGISTRAR_TPL_HDR

#include "registrar.hpp"

template<typename T>
int registrar<T>::get_id(const T& k)
{
    hash_iterator_type_ found_it(hash_.find(k));

    if (found_it == hash_.end())
    {
	vec_.push_back(k);
	return (hash_[k] = vec_.size() - 1);
    }
    else
	return found_it->second;
}

template<typename T>
bool registrar<T>::is_registered(const T& k)
{
    return (hash_.count(k) > 0);
}

template<typename T>
const T& registrar<T>::get_obj(int id) const
{
    return vec_[id];
}

template<typename T>
int registrar<T>::last_id() const
{
    return (vec_.size() - 1);
}

template<class T> template<class Archive>
void registrar<T>::save(Archive & ar, const unsigned int /*version*/) const
{
    ar & vec_;
}

template<class T> template<class Archive>
void registrar<T>::load(Archive & ar, const unsigned int /*version*/)
{
    ar & vec_;

    for (typename std::vector<T>::const_iterator it = vec_.begin();
	it != vec_.end();
	++it)
	hash_[*it] = it - vec_.begin();
}



#endif
