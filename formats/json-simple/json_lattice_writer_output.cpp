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

    numberOfMainElements_ = 0;
    currentArrayPointer_ = NULL;

    outputStream_ << "[";
}


JSONLatticeWriterOutput::~JSONLatticeWriterOutput() {
    outputStream_ << "]";
}


void JSONLatticeWriterOutput::push(const std::string & textElement) {
    std::string jsonStringElement;
    createEscapedJSONString_(textElement, jsonStringElement);

    pushTextElementToOutput_(jsonStringElement);
}

void JSONLatticeWriterOutput::pushTextElementToOutput_(const std::string & textElement) {
    if (currentArrayPointer_) {
        currentArrayPointer_->push_back(textElement);
    } else {
        printElementToMainOutput_(textElement);
    }
}

void JSONLatticeWriterOutput::push(
     const AnnotationItem & element,
     AnnotationItemManager * latticeAnnotationItemManager) {

    std::stringstream temporaryStream;

    temporaryStream << "{";
    printHashValueToTemporaryString_(temporaryStream, "category", element.getCategory());
    temporaryStream << ",";
    printHashValueToTemporaryString_(temporaryStream, "text", element.getText());
    temporaryStream << ",";

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
    temporaryStream << "}";

    temporaryStream << "}";

    pushTextElementToOutput_(temporaryStream.str());
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
    if (currentArrayPointer_) {
        arraysStack_.push(currentArrayPointer_);
    }

    currentArrayPointer_ = new std::vector<std::string>();
}

void JSONLatticeWriterOutput::closeSubArray() {
    closeSubArray_(false);
}

void JSONLatticeWriterOutput::closeSubArrayWithFlattenOneElement() {
    closeSubArray_(true);
}

void JSONLatticeWriterOutput::closeSubArray_(bool flattenOneElement) {
    if ( !currentArrayPointer_) {
        return;
    }

    if ( !arraysStack_.empty() ) {

        std::vector<std::string> * parentArrayPointer = arraysStack_.top();
        arraysStack_.pop();

        if ( !isCurrentArrayEmpty_()) {

            std::string elementToPrint;
            printCurrentArrayToStringWithFlatten_(elementToPrint, flattenOneElement);

            if ( !elementToPrint.empty()) {
                parentArrayPointer->push_back(elementToPrint);
            }
        }

        delete currentArrayPointer_;
        currentArrayPointer_ = parentArrayPointer;
    } else {

        if (currentArrayPointer_ && !currentArrayPointer_->empty()) {
            std::string elementToPrint;
            printCurrentArrayToStringWithFlatten_(elementToPrint, flattenOneElement);
            printElementToMainOutput_(elementToPrint);
        }

        delete currentArrayPointer_;
        currentArrayPointer_ = NULL;
    }
}

void JSONLatticeWriterOutput::printElementToMainOutput_(const std::string & textElement) {
    if (numberOfMainElements_) {
        outputStream_ << "," << textElement;
    } else {
        outputStream_ << textElement;
    }

    ++numberOfMainElements_;
}

void JSONLatticeWriterOutput::printCurrentArrayToStringWithFlatten_(
     std::string & elementToPrint, bool flattenOneElement) {
    if (flattenOneElement) {
        if ( ! tryToFlattenOneElementCurrentArray(elementToPrint)) {
            printCurrentArrayToString_(elementToPrint);
        }
    } else {
        printCurrentArrayToString_(elementToPrint);
    }
}

void JSONLatticeWriterOutput::printCurrentArrayToString_(std::string & output) {

    if (currentArrayPointer_ && (! currentArrayPointer_->empty())) {
        std::stringstream tempStream;
        tempStream << "[";

        bool first = true;
        BOOST_FOREACH(std::string currentValue, *currentArrayPointer_) {
            if (!first) {
                tempStream << ",";
            } else {
                first = false;
            }

            tempStream << currentValue;
        }
        tempStream << "]";

        output = tempStream.str();
    }
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
