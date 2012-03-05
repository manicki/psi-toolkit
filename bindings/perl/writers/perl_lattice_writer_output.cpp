#include <iostream>
#include <list>
#include <string>

#include "logging.hpp"

#include "perl_lattice_writer_output.hpp"

typedef std::pair<std::string, std::string> string_pair;

PerlLatticeWriterOutput::PerlLatticeWriterOutput(AV * arrayPointer)
    : AbstractSimpleDataLatticeWriterOutput() {
    currentArrayPointer_ = arrayPointer;
}

void PerlLatticeWriterOutput::pushToCurrentArrayPointer_(const std::string & textElement) {
    SV * element_sv = newSVpv(textElement.c_str(), 0);
    av_push(getPerlCurrentArrayPointer_(), element_sv);
}

void PerlLatticeWriterOutput::pushToCurrentArrayPointer_(
     const AnnotationItem & element,
     AnnotationItemManager * latticeAnnotationItemManager) {

    HV * newHashEntry = newHV();
    SV * categoryValue = newSVpv(element.getCategory().c_str(), 0);
    SV * textValue = newSVpv(element.getText().c_str(), 0);

    hv_store(newHashEntry,
             "category", 8,
             categoryValue, 0);
    hv_store(newHashEntry,
             "text", 4,
             textValue, 0);

    HV * itemValuesHash = newHV();
    
    std::list< std::pair<std::string, std::string> > values =
        latticeAnnotationItemManager->getValues(element);

    BOOST_FOREACH(string_pair currentValue, values) {
        SV * currentValueScalar = newSVpv(currentValue.second.c_str(), 0);
        hv_store(itemValuesHash,
                 currentValue.first.c_str(), currentValue.first.size(),
                 currentValueScalar, 0);
    }

    hv_store(newHashEntry,
             "values", 6,
             newRV_inc((SV *)itemValuesHash), 0);

    PerlReference newHashEntryReference = newRV_inc((PerlReference) newHashEntry);
    av_push( getPerlCurrentArrayPointer_(), newHashEntryReference);

}

void PerlLatticeWriterOutput::pushToArrayPointer_(ArrayPointer arrayPointer, ReferencePointer what) {
    av_push( (AV*) arrayPointer, (SV *) what);
}


ArrayPointer PerlLatticeWriterOutput::createNewArrayPointer_() {
    return newAV();
}

ReferencePointer PerlLatticeWriterOutput::doFlattenOneElementCurrentArray() {
    PerlReference elementReference = av_pop(getPerlCurrentArrayPointer_());
    av_undef(getPerlCurrentArrayPointer_());
    return elementReference;
}
