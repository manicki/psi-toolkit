#ifndef AVMATRIX_HPP_HDR
#define AVMATRIX_HPP_HDR

#include <vector>

#include <boost/shared_ptr.hpp>

#include "hash_wrapper.hpp"

template<typename C, typename T>
class av_matrix
{
public:
    typedef C BaseCategory;

private:
    BaseCategory cat_;
    typedef boost::shared_ptr<std::vector<T> > shared_ptr_type;

    shared_ptr_type avs_;

public:
    explicit av_matrix() { }

    explicit av_matrix(BaseCategory a_cat):cat_(a_cat) { }

    av_matrix(BaseCategory a_cat, int a_nb_attrs, T default_value):
        cat_(a_cat),
        avs_(new std::vector<T>(a_nb_attrs, default_value))
    { }

    av_matrix(const av_matrix& a_avm) :
        cat_(a_avm.cat_),
        avs_(a_avm.avs_)
    { }

    av_matrix(BaseCategory a_cat, const av_matrix& a_avm) :
        cat_(a_cat),
        avs_(a_avm.avs_)
    { }

    bool operator==(const av_matrix<C,T>& b) const {
        if (this == &b) {
            return true;
        }
        if (cat_ != b.cat_) {
            return false;
        }
        if (avs_ == b.avs_) {
            return true;
        }
        if (!avs_ || !b.avs_) {
            return false;
        }
        if (avs_->size() != b.avs_->size()) {
            return false;
        }
        for (unsigned int i = 0; i < avs_->size(); ++i) {
            if ((*avs_)[i] != (*b.avs_)[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const av_matrix<C,T>& b) const {
        return !operator==(b);
    }

    T get_attr(int attr_ix) const {
        return (*avs_)[attr_ix];
    }

    T get_attr(int attr_ix, T default_value) const {
        if (!avs_ || attr_ix >= (signed int)avs_->size()) {
            return default_value;
        }
        return (*avs_)[attr_ix];
    }

    T set_attr(int attr_ix, T val, T default_value) {
        if (!avs_) {
            if (val == default_value) {
                return val;
            }
            avs_.reset(new std::vector<T>(attr_ix + 1, default_value));
        } else if (attr_ix >= (signed int)avs_->size()) {
            if (val == default_value) {
                return val;
            }
            if (!avs_.unique()) {
                avs_.reset(new std::vector<T>(*avs_));
            }
            avs_->resize(attr_ix + 1, default_value);
        } else {
            if ((*avs_)[attr_ix] == val) {
                return val;
            }
            if (!avs_.unique()) {
                avs_.reset(new std::vector<T>(*avs_));
            }
        }
        return ((*avs_)[attr_ix] = val);
    }

    BaseCategory get_cat() const {
        return cat_;
    }

    BaseCategory catprint() const {
        return cat_;
    }

    int nb_attrs() const {
        return (avs_ ? avs_->size() : 0);
    }

    void copy_attrs(const av_matrix& a_avm) {
        avs_ = a_avm.avs_;
    }

    operator std::string() const {
        std::stringstream ss;
        ss << cat_;
        return ss.str();
    }
};

HASH_WRAPPER_HASH_INTRO
    template<>
    struct HASH_WRAPPER_HASH_TRAITS<av_matrix<int,int> >
    {
        size_t operator()(const av_matrix<int,int>& k) const
        {
            size_t r = HASH_WRAPPER_HASH_TRAITS<int>().operator()(k.get_cat());

            int i;
            for(i = 0; i < k.nb_attrs(); ++i)
            r ^= HASH_WRAPPER_HASH_TRAITS<int>().operator()(k.get_attr(i));

            return r;
        }

        HASH_WRAPPER_EXTRA_STUFF

#ifdef __VS__
        bool operator()(const av_matrix<int,int>& a,
                const av_matrix<int,int>& b) const
        {
            return a != b;
        }
#endif
    };
HASH_WRAPPER_HASH_OUTRO


template<typename T>
struct bare_av_matrix
{
    struct null_av_matrix_category
    {
    bool operator==(const null_av_matrix_category&) const { return true; }
    bool operator!=(const null_av_matrix_category&) const { return false; }
    };

    typedef av_matrix<null_av_matrix_category,T> type;
};

#endif
