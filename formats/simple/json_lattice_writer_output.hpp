#ifndef JSON_LATTICE_WRITER_OUTPUT_HDR
#define JSON_LATTICE_WRITER_OUTPUT_HDR


#include <sstream>
#include <string>
#include <vector>
#include "config.hpp"
#include  <stack>
#include "annotation_item.hpp"
#include "annotation_item_manager.hpp"

class JSONLatticeWriterOutput {
public:
    JSONLatticeWriterOutput(std::ostream & outputStream);

    void push(const std::string & textElement);
    void push(const AnnotationItem & element,
              AnnotationItemManager * latticeAnnotationItemManager);

    void openNewSubArray();
    void closeSubArray();
    void closeSubArrayWithFlattenOneElement();

    ~JSONLatticeWriterOutput();

private:
    void closeSubArray_(bool flattenOneElement);

    void pushTextElementToOutput_(const std::string & textElement);

    bool isCurrentArrayEmpty_();
    long getCurrentArrayLength_();
    bool tryToFlattenOneElementCurrentArray(std::string & flattenElement);

    void printElementToMainOutput_(const std::string & textElement);

    void printCurrentArrayToStringWithFlatten_(std::string & output, bool flattenOneElement);
    void printCurrentArrayToString_(std::string & output);
    void printHashValueToTemporaryString_(
                       std::stringstream & outputStream, const std::string & key, const std::string & value);

    void createEscapedJSONString_(const std::string & content, std::string & output);

    std::ostream & outputStream_;

    long numberOfMainElements_;
    std::vector<std::string> * currentArrayPointer_;
    std::stack<std::vector<std::string> * > arraysStack_;

    AnnotationItemManager * latticeAnnotationItemManager_;
};

#endif
