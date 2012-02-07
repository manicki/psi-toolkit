#ifndef ONE_EDGE_AT_A_TIME_ANNOTATOR_HDR_HDR
#define ONE_EDGE_AT_A_TIME_ANNOTATOR_HDR_HDR

#include "annotator.hpp"
#include "annotator_factory.hpp"

template<typename T>
class OneEdgeAtATimeAnnotator : public Annotator {
private:
    T engine_;

public:
    OneEdgeAtATimeAnnotator(const boost::program_options::variables_map& options)
        :engine_(options) {
    }

    class Factory : public AnnotatorFactory {
    private:
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options) {

            return new OneEdgeAtATimeAnnotator<T>(options);
        }

        virtual boost::program_options::options_description doOptionsHandled() {
            return T::optionsHandled();
        }

        virtual std::string doGetName() {
            return T::getName();
        }

        virtual boost::filesystem::path doGetFile() {
            return T::getFile();
        }

        virtual std::list<std::list<std::string> > doRequiredLayerTags() {
            return std::list<std::list<std::string> >();
        }

        virtual std::list<std::list<std::string> > doOptionalLayerTags() {
            return std::list<std::list<std::string> >();
        }

        virtual std::list<std::string> doProvidedLayerTags() {
            return T::providedLayerTags();
        }
    };

    class Worker : public LatticeWorker {

    private:
        Processor& processor_;
        LayerTagMask tokenMask_;

    public:
        Worker(Processor& processor, Lattice& lattice)
            :LatticeWorker(lattice),
             processor_(processor),
             tokenMask_(lattice.getLayerTagManager().getMask(T::tagsToOperateOn())) {
        }

    private:
        virtual void doRun() {
            T& engine = dynamic_cast<OneEdgeAtATimeAnnotator&>(processor_).engine_;

            Lattice::EdgesSortedByTargetIterator edgeIterator
                = lattice_.edgesSortedByTarget(tokenMask_);

            while (edgeIterator.hasNext()) {
                Lattice::EdgeDescriptor edge = edgeIterator.next();

                engine.processEdge(lattice_, edge);
            }
        }
    };

    virtual LatticeWorker* doCreateLatticeWorker(Lattice& lattice) {
        return new Worker(*this, lattice);
    }

    virtual std::string doInfo() {
        return "bla";
    }
};

#endif
