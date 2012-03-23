#include <iostream>
#include <list>
#include <string>

#include <sstream>

#include "logging.hpp"

#include "json_lattice_writer_output.hpp"

#include <boost/algorithm/string.hpp> 

typedef std::pair<std::string, std::string> string_pair;

JSONLatticeWriterOutput::JSONLatticeWriterOutput(std::ostream & outputStream)
  : outputStream_(outputStream)
{
    latticeAnnotationItemManager_ =  NULL;

    currentArrayPointer_ = new std::vector<std::string>();
    openNewSubArray();
}


JSONLatticeWriterOutput::~JSONLatticeWriterOutput() {
    closeSubArray();
}


void JSONLatticeWriterOutput::push(const std::string & textElement) {
    std::string jsonStringElement;
    createEscapedJSONString_(textElement, jsonStringElement);
    currentArrayPointer_->push_back(jsonStringElement);
}

void JSONLatticeWriterOutput::push(
     const AnnotationItem & element,
     AnnotationItemManager * latticeAnnotationItemManager) {

    std::stringstream temporaryStream;

    temporaryStream << "{\n";
    printHashValueToTemporaryString_(temporaryStream, "category", element.getCategory());
    temporaryStream << ",\n";
    printHashValueToTemporaryString_(temporaryStream, "text", element.getText());
    temporaryStream << ",\n";

    temporaryStream << "\"values\" : {";
    std::list< std::pair<std::string, std::string> > values =
        latticeAnnotationItemManager->getValues(element);

    bool first = true;
    BOOST_FOREACH(string_pair currentValue, values) {

        if (!first) {
            temporaryStream << ",";
        } else {
            first = false;
        }

        printHashValueToTemporaryString_(temporaryStream, currentValue.first, currentValue.second);
    }
    temporaryStream << "}\n";

    temporaryStream << "}\n";

    currentArrayPointer_->push_back(temporaryStream.str());
}

void JSONLatticeWriterOutput::printHashValueToTemporaryString_(
    std::stringstream & outputStream, const std::string & key, const std::string & value) {

    std::string escapedKey;
    createEscapedJSONString_(key, escapedKey);
    std::string escapedValue;
    createEscapedJSONString_(value, escapedValue);

    outputStream << escapedKey << ":" << escapedValue;
}

void JSONLatticeWriterOutput::openNewSubArray() {
    arraysStack_.push(currentArrayPointer_);
    currentArrayPointer_ = new std::vector<std::string>();
}

void JSONLatticeWriterOutput::closeSubArray() {
    closeSubArray_(false);
}

void JSONLatticeWriterOutput::closeSubArrayWithFlattenOneElement() {
    closeSubArray_(true);
}

void JSONLatticeWriterOutput::closeSubArray_(bool flattenOneElement) {
    if ( !arraysStack_.empty() ) {

        std::vector<std::string> * parentArrayPointer = arraysStack_.top();
        arraysStack_.pop();

        if ( !isCurrentArrayEmpty_()){

            if (flattenOneElement) {
                std::string elementToPrint;
                if (tryToFlattenOneElementCurrentArray(elementToPrint)) {
                    parentArrayPointer->push_back(elementToPrint);
                } else {
                    printCurrentArrayToOutput_();
                }
            } else {
                printCurrentArrayToOutput_();
            }

        }

        currentArrayPointer_ = parentArrayPointer;
    }
}

void JSONLatticeWriterOutput::printCurrentArrayToOutput_() {

    outputStream_ << "[\n";

    bool first = true;
    BOOST_FOREACH(std::string currentValue, *currentArrayPointer_) {
        if (!first) {
            outputStream_ << ",\n";
        } else {
            first = false;
        }

        outputStream_ << currentValue;
    }
    outputStream_ << "\n]\n";
}

void JSONLatticeWriterOutput::createEscapedJSONString_(
                const std::string & content, std::string & output) {
    output = content;
    boost::replace_all(output, "\\", "\\\\");
    boost::replace_all(output, "\"", "\\\"");
    
    output = "\"" + output + "\"";
}


bool JSONLatticeWriterOutput::isCurrentArrayEmpty_() {
    return 0 == getCurrentArrayLength_();
}


long JSONLatticeWriterOutput::getCurrentArrayLength_() {
    long arrayLength = currentArrayPointer_->size();
    return arrayLength;
}

bool JSONLatticeWriterOutput::tryToFlattenOneElementCurrentArray(std::string & flattenElement) {

    long arrayLength = getCurrentArrayLength_();

    if ( 1 == arrayLength) {
        flattenElement = currentArrayPointer_->back();
        return true;
    } else {
        return false;
    }
}


