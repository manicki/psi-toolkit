#ifndef LANG_SPECIFIC_PROCESSOR_FILE_FETCHER_HDR
#define LANG_SPECIFIC_PROCESSOR_FILE_FETCHER_HDR

#include "processor_file_fetcher.hpp"

/*!
Specifying files for processors
===============================

Processors (mostly annotators) may need to read some external files
(rule files, configuration files etc.) In order to facilitate handling
such files `LangSpecificProcessorFileFetcher' class was created. 
It should be used in the factory of a given processor. Two arguments
are given in the constructor of `LangSpecificProcessorFileFetcher':
* the path of the source file in which the processor and its factory
  are defined (in most cases simply __FILES__ preprocessor definition
  should be used), this path is used to determine the `data' directory
  of a given processor
* the language used in the processor.

`LangSpecificProcessorFileFetcher' has two main methods for fetching
files (defined in `FileFetcher' base class):
* `getOneFile' - should be used when exactly one file is wanted
* `getFiles' - when multiple files are wanted (e.g. when rules could be
  kept in more than one file), the files are returned via an output
  iterator.

Both methods expect a file specification, a file specification is a
path in which the following special expressions can be used:
* `*' and '?' standard filename wildcards
* `**' wildcards for directories and subdirectories (just as in Ant
  configuration files, see http://ant.apache.org/manual/dirtasks.html)
* `%FOO%' where `FOO' is a so-called param, params are substituted with
  their values, values can be set with `addParam' method, however
  two params are set by `LangSpecificProcessorFileFetcher' constructor
  by default, namely: %ITSDATA% (`data' directory of a given processor)
  %LANG% (the code of the language expected, e.g. `pl' for Polish).

Such file specification should be expected in processor's options
(default values should be specified as well) and should be converted
to file paths in the processor's factory using `getOneFile' and
`getFiles' methods.

For an example of using LangSpecificProcessorFileFetcher see 
tp_tokenizer.cpp
*/
class LangSpecificProcessorFileFetcher : public ProcessorFileFetcher {

public:
    LangSpecificProcessorFileFetcher(boost::filesystem::path sourceFilePath,
                                     const std::string& lang);

private:
    static const std::string LANG_PARAM;

};


#endif
