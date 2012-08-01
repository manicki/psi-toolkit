#include "configurator.hpp"

#include "config.hpp"

Configurator::Configurator():runAsInstalled_(
#if IS_INSTALLABLE
true
#else
false
#endif
) {
}

Configurator& Configurator::getInstance() {
    static Configurator instance;

    return instance;
}

bool Configurator::isRunAsInstalled() const {
    return runAsInstalled_;
}

void Configurator::setRunAsInstalled(bool state) {
    runAsInstalled_ = state;
}

boost::filesystem::path Configurator::getDataDir() const {
    return boost::filesystem::path(INSTALL_DATA_DIR);
}

boost::filesystem::path Configurator::getFinalPath(
    const boost::filesystem::path& componentName,
    const boost::filesystem::path& sourcePath,
    const boost::filesystem::path& subdirectory) const {

    return isRunAsInstalled()
        ? getDataDir() / underscores2minuses_(componentName)
        : boost::filesystem::path("..") / sourcePath / subdirectory;
}

boost::filesystem::path Configurator::underscores2minuses_(
    const boost::filesystem::path& segment) const {
    std::string segmentAsString = segment.string();
    std::replace(segmentAsString.begin(), segmentAsString.end(), '_', '-');

    return boost::filesystem::path(segmentAsString);
}
