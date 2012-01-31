#include "test_batch.hpp"

#include <boost/algorithm/string/replace.hpp>

std::string TestBatch::processPipeline_(const std::string& pipeline) {
    return boost::algorithm::replace_all_copy(
        pipeline, "%TESTDIR%", mDirectory_.string());
}
