#ifndef SEMANTICS_WRAPPER_HPP_HDR
#define SEMANTICS_WRAPPER_HPP_HDR


template<typename Atom, typename Master>
class semantics_wrapper {

public:
    semantics_wrapper(Master &, int bonus_divisor = 1);

    Atom intersect(Atom sem, Atom pattern, double& bonus);
    Atom tri_intersect(Atom word, Atom sem, Atom pattern, double& bonus);

    void set_divisor(int bonus_divisor);

private:
    Atom tri_intersect_(Atom /*word*/, Atom sem, Atom pattern, double& bonus);

    Master & master_;
    double bonus_divisor_;

};


#endif
