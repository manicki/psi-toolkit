#ifndef APERTIUM_LATTICE_READER_HDR
#define APERTIUM_LATTICE_READER_HDR

#include <string>

#include "stream_lattice_reader.hpp"
#include "lattice_reader_factory.hpp"

#include "logging.hpp"

#include "apertium_deformatter.hpp"

class ApertiumLatticeReader : public StreamLatticeReader {
public:

    ApertiumLatticeReader(const boost::filesystem::path&, bool unzipData, bool keepTags);

    std::string getFormatName();

    std::string processReplacementRules(const std::string& input);
    std::vector<DeformatIndex> processFormatRules(const std::string& input);

    class Factory : public LatticeReaderFactory<std::istream> {
    public:
        virtual ~Factory();

    private:
        virtual LatticeReader<std::istream>* doCreateLatticeReader(
            const boost::program_options::variables_map& options);

        virtual boost::program_options::options_description doOptionsHandled();

        virtual std::string doGetName() const;
        virtual boost::filesystem::path doGetFile() const;

        static const std::string DEFAULT_SPEC_FILES_DIR;
        static const std::string DEFAULT_SPEC_FILE_ENDING;
    };

    bool isKeepTags();

private:

    virtual std::string doInfo();

    class Worker : public ReaderWorker<std::istream> {
    public:
        Worker(ApertiumLatticeReader& processor,
               std::istream& inputStream,
               Lattice& lattice);

        virtual void doRun();

    private:
        ApertiumLatticeReader& processor_;
        LayerTagCollection fragTags_;
        LayerTagCollection tagTags_;

        void appendFragmentToLattice_(std::string);
        void appendTagToLattice_(std::string, std::string, bool);
    };

    virtual ReaderWorker<std::istream>* doCreateReaderWorker(
        std::istream& inputStream, Lattice& lattice) {

        return new Worker(*this, inputStream, lattice);
    }

    ApertiumDeformatter apertiumDeformatter_;
    boost::filesystem::path specificationFile_;

    bool keepTags_;

};

#endif
