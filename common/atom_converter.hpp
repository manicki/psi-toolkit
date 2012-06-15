#ifndef ATOM_CONVERTER_HDR
#define ATOM_CONVERTER_HDR


#include "zvalue.hpp"


/**
 * converts between Atom type (integer or zvalue) and integer
 */
namespace AtomConverter {

    int toInt(zvalue atom) {
        return ZVALUE_TO_INTEGER(atom);
    }

    int toInt(int atom) {
        return atom;
    }

    template <typename Atom>
    Atom toAtom(int number) {
        return (Atom)(number);
    }

    template <>
    zvalue toAtom< zvalue >(int number) {
        return INTEGER_TO_ZVALUE(number);
    }

    template <>
    int toAtom< int >(int number) {
        return number;
    }

};


#endif
