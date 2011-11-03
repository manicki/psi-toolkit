#ifndef CONFIGURATOR_HDR_HDR
#define CONFIGURATOR_HDR_HDR

#include <boost/filesystem.hpp>

class Configurator {
public:
    static Configurator& getInstance();

    bool isRunAsInstalled() const;
    void setRunAsInstalled(bool state);

    boost::filesystem::path getDataDir() const;

    boost::filesystem::path getFinalPath(
        const boost::filesystem::path& componentName,
        const boost::filesystem::path& sourcePath,
        const boost::filesystem::path& subdirectory) const;

private:
    Configurator();

    boost::filesystem::path underscores2minuses_(
        const boost::filesystem::path& segment) const;

    bool runAsInstalled_;
};

#endif
