#ifndef GENERAL_CASE_CONVERTER_HDR
#define GENERAL_CASE_CONVERTER_HDR

#include <boost/shared_ptr.hpp>

#include "range_based_case_converter.hpp"
#include "special_casing_converter.hpp"

#include "utf8/utf8.h"

template<typename octet_iterator, typename output_iterator>
class GeneralCaseConverter {

public:
    GeneralCaseConverter(
        boost::shared_ptr<RangeBasedCaseConverter> rangeBasedCaseConverter,
        boost::shared_ptr<SpecialCasingConverter> specialCasingConverter)
        :rangeBasedCaseConverter_(rangeBasedCaseConverter),
         specialCasingConverter_(specialCasingConverter) {
    }

    bool willBeTouchedWhenConverted(octet_iterator start, octet_iterator end) const {
        while (start != end) {
            uint32_t code_point = utf8::unchecked::next(start);

            if (specialCasingConverter_->convert(code_point)
                || rangeBasedCaseConverter_->convert(code_point) != code_point)
                return true;
        }

        return false;
    }

    bool willBeTouchedWhenHeadConverted(octet_iterator start, octet_iterator end) const {
        if (start == end)
            return false;

        octet_iterator prev_start = start;
        utf8::unchecked::next(start);
        return willBeTouchedWhenConverted(prev_start, start);
    }

    bool willBeTouchedWhenTailConverted(octet_iterator start, octet_iterator end) const {
        if (start == end)
            return false;

        utf8::unchecked::next(start);
        return willBeTouchedWhenConverted(start, end);
    }

    void convert(octet_iterator start, octet_iterator end, output_iterator out) const {
        while (start != end) {
            uint32_t code_point = utf8::unchecked::next(start);

            if(const char* special = specialCasingConverter_->convert(code_point)) {
                while (*special)
                    *out++ = *special++;
            } else {
                uint32_t converted_code_point = rangeBasedCaseConverter_->convert(code_point);
                utf8::unchecked::append(converted_code_point, out);
            }
        }
    }

    void headConvert(octet_iterator start, octet_iterator end, output_iterator out) const {
        bool first = true;

        while (start != end) {
            if (first) {
                octet_iterator prev_start = start;
                utf8::unchecked::next(start);
                convert(prev_start, start, out);
                first = false;
            } else {
                *out++ = *start++;
            }
        }
    }

    void tailConvert(octet_iterator start, octet_iterator end, output_iterator out) const {
        if (start != end) {
            uint32_t code_point = utf8::unchecked::next(start);

            utf8::unchecked::append(code_point, out);

            convert(start, end, out);
        }
    }


private:

    boost::shared_ptr<RangeBasedCaseConverter> rangeBasedCaseConverter_;
    boost::shared_ptr<SpecialCasingConverter> specialCasingConverter_;

};

#endif
