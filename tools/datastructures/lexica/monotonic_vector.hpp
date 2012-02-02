#ifndef MONOTONIC_VECTOR_HDR_HDR
#define MONOTONIC_VECTOR_HDR_HDR


#include <vector>
#include <limits>
#include <algorithm>
#include <cstdio>

/**
 * MonotonicVector represents a monotonic increasing function that
 * maps positive integers of any size onto a given number type. Each
 * value has to be equal or larger than the previous one. Depending on
 * the stepSize it can save up to 80% of memory compared to a
 * std::vector<long>. Time complexity is roughly constant, in the
 * worst case, however, stepSize times slower than a normal
 * std::vector.
 */
template<typename PosT = size_t, typename LenT = unsigned char, PosT stepSize = 25>
class MonotonicVector {
private:
    std::vector<LenT> m_diffs;
    std::vector<PosT> m_anchors;
    std::vector<std::pair<PosT, PosT> > m_rests;

public:
    typedef PosT value_type;

    PosT size() const {
        return m_diffs.size() + m_anchors.size();
    }

    PosT at(PosT i) const {
        PosT s = stepSize;
        PosT j = m_anchors[i / s];
        if (i % stepSize != 0) {
            PosT start = (i / s) * (s - 1);
            PosT end   = start + (i % s);
            for (PosT k = start; k < end; k++) {
                j += m_diffs[k];
                if (m_diffs[k] == (std::numeric_limits<LenT>::max)()) {
                    typename std::vector<std::pair<PosT, PosT> >::const_iterator it
                        = std::lower_bound(m_rests.begin(),
                                           m_rests.end(),
                                           std::make_pair(k, 0),
                                           std::less<std::pair<PosT, PosT> >());

                    if (it != m_rests.end() && it->first == k)
                        j += it->second;
                }
            }
        }
        return j;
    }

    PosT operator[](PosT i) const {
        return at(i);
    }

    PosT back() const {
        return at(size()-1);
    }

    void push_back(PosT i) {
        if (size() % stepSize == 0) {
            m_anchors.push_back(i);
        }
        else {
            PosT last = back();
            PosT diff = i - last;
            if (diff <= (std::numeric_limits<LenT>::max)()) {
                m_diffs.push_back(diff);
            }
            else {
                m_rests.push_back(
                    std::make_pair(m_diffs.size(), diff - (std::numeric_limits<LenT>::max)()));
                m_diffs.push_back((std::numeric_limits<LenT>::max)());
            }
        }
    }

    void load(std::FILE* in) {
        size_t size;

        fread(&size, sizeof(size_t), 1, in);
        m_diffs.resize(size);
        fread(&m_diffs[0], sizeof(LenT), size, in);

        fread(&size, sizeof(size_t), 1, in);
        m_anchors.resize(size);
        fread(&m_anchors[0], sizeof(PosT), size, in);

        fread(&size, sizeof(size_t), 1, in);
        m_rests.resize(size);
        fread(&m_rests[0], sizeof(std::pair<PosT, PosT>), size, in);
    }

    void save(std::FILE* out) const {
        size_t size = m_diffs.size();
        fwrite(&size, sizeof(size_t), 1, out);
        fwrite(&m_diffs[0], sizeof(LenT), size, out);

        size = m_anchors.size();
        fwrite(&size, sizeof(size_t), 1, out);
        fwrite(&m_anchors[0], sizeof(PosT), size, out);

        size = m_rests.size();
        fwrite(&size, sizeof(size_t), 1, out);
        fwrite(&m_rests[0], sizeof(std::pair<PosT, PosT>), size, out);
    }

    void swap(MonotonicVector<PosT, LenT, stepSize> &mv) {
        m_diffs.swap(mv.m_diffs);
        m_anchors.swap(mv.m_anchors);
        m_rests.swap(mv.m_rests);
    }
};

#endif
