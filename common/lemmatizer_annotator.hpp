#ifndef LEMMATIZER_ANNOTATOR_HDR
#define LEMMATIZER_ANNOTATOR_HDR

#include "annotator.hpp"
#include "annotator_factory.hpp"
#include "lattice_worker.hpp"
#include "layer_tag_collection.hpp"
#include "lemmatizer_output_iterator.hpp"
#include "general_case_converter.hpp"
#include "string_case_converter_manager.hpp"
#include "simple_convert.hpp"

template<typename L>
class LemmatizerAnnotator : public Annotator {

private:
    L lemmatizer_;
    boost::shared_ptr<GeneralCaseConverter<std::string::const_iterator,
                                           std::back_insert_iterator<std::string> > >
       lowerCaseConverter_;


public:

    LemmatizerAnnotator(const boost::program_options::variables_map& options)
        :lemmatizer_(options) {
        lowerCaseConverter_ = StringCaseConverterManager::getInstance().
            getLowerCaseConverter(lemmatizer_.getLanguage());
    }

    class Factory : public AnnotatorFactory {
    private:
        virtual Annotator* doCreateAnnotator(
            const boost::program_options::variables_map& options) {

            return new LemmatizerAnnotator<L>(options);
        }

        virtual boost::program_options::options_description doOptionsHandled() {
            return L::optionsHandled();
        }

        virtual LanguagesHandling doLanguagesHandling(
            const boost::program_options::variables_map& options) const {

            return L::languagesHandling(options);
        }

        virtual std::list<std::string> doLanguagesHandled(
            const boost::program_options::variables_map& options) const {

            return L::languagesHandled(options);
        }

        virtual std::string doGetName() const {
            return L::getName();
        }

        virtual boost::filesystem::path doGetFile() const {
            return L::getFile();
        }

        virtual std::list<std::list<std::string> > doRequiredLayerTags() {
            return std::list<std::list<std::string> >();
        }

        virtual std::list<std::list<std::string> > doOptionalLayerTags() {
            return std::list<std::list<std::string> >();
        }

        virtual std::list<std::string> doProvidedLayerTags() {
            std::list<std::string> layerTags;
            layerTags.push_back("lemma");
            return layerTags;
        }
    };

    class Worker : public LatticeWorker {

    private:
        Processor& processor_;
        LayerTagMask tokenMask_;

        class WorkerOutputIterator : public LemmatizerOutputIterator {
        private:
            Lattice& lattice_;
            LayerTagCollection correctionTag_;
            LayerTagCollection normalizationTag_;
            LayerTagCollection lemmaTag_;
            LayerTagCollection lexemeTag_;
            LayerTagCollection formTag_;
            Lattice::EdgeDescriptor tokenEdge_;
            Lattice::EdgeDescriptor lastCorrection_;
            Lattice::EdgeDescriptor lastNormalization_;
            Lattice::EdgeDescriptor lastLemma_;
            Lattice::EdgeDescriptor lastLexeme_;

        public:
            WorkerOutputIterator(
                LayerTagCollection layerTags, Lattice& lattice, Lattice::EdgeDescriptor tokenEdge)
                :lattice_(lattice),
                 correctionTag_(
                     createUnion(
                         lattice.getLayerTagManager().createSingletonTagCollection(
                             "token,corrected"),
                         layerTags)),
                 normalizationTag_(
                     createUnion(
                         lattice.getLayerTagManager().createSingletonTagCollection("normalization"),
                         layerTags)),
                 lemmaTag_(
                     createUnion(
                         lattice.getLayerTagManager().createSingletonTagCollection("lemma"),
                         layerTags)),
                 lexemeTag_(
                     createUnion(
                         lattice.getLayerTagManager().createSingletonTagCollection("lexeme"),
                         layerTags)),
                 formTag_(
                     createUnion(
                         lattice.getLayerTagManager().createSingletonTagCollection("form"),
                         layerTags)),
                 tokenEdge_(tokenEdge),
                 lastCorrection_(tokenEdge),
                 lastNormalization_(tokenEdge),
                 lastLemma_(tokenEdge) {
            }

        private:
            virtual void doAddCorrection(
                const std::string& correction,
                Lattice::Score score,
                int ruleId) {

                Lattice::EdgeSequence::Builder seqBuilder(lattice_);
                seqBuilder.addEdge(tokenEdge_);

                AnnotationItem item("word", correction);

                lastCorrection_ = lastNormalization_ = lastLemma_ = lastLexeme_ =
                    lattice_.addEdge(
                        lattice_.getEdgeSource(tokenEdge_),
                        lattice_.getEdgeTarget(tokenEdge_),
                        item,
                        correctionTag_,
                        seqBuilder.build(),
                        score,
                        ruleId);
            }

            virtual void doAddNormalization(
                const std::string& normalization,
                Lattice::Score score,
                int ruleId) {

                Lattice::EdgeSequence::Builder seqBuilder(lattice_);
                seqBuilder.addEdge(lastCorrection_);

                AnnotationItem item("word", normalization);

                lastNormalization_ = lastLemma_ = lastLexeme_ =
                    lattice_.addEdge(
                        lattice_.getEdgeSource(lastCorrection_),
                        lattice_.getEdgeTarget(lastCorrection_),
                        item,
                        normalizationTag_,
                        seqBuilder.build(),
                        score,
                        ruleId);

            }

            virtual void doAddLemma(
                const std::string& lemma,
                Lattice::Score score,
                int ruleId) {

                Lattice::EdgeSequence::Builder seqBuilder(lattice_);
                seqBuilder.addEdge(lastNormalization_);

                AnnotationItem item("word", lemma);

                lastLemma_ = lastLexeme_ =
                    lattice_.addEdge(
                        lattice_.getEdgeSource(lastNormalization_),
                        lattice_.getEdgeTarget(lastNormalization_),
                        item,
                        lemmaTag_,
                        seqBuilder.build(),
                        score,
                        ruleId);
            }

            virtual void doAddLexeme(
                const AnnotationItem& item,
                Lattice::Score score,
                int ruleId) {

                Lattice::EdgeSequence::Builder seqBuilder(lattice_);
                seqBuilder.addEdge(lastLemma_);

                lastLexeme_ =
                    lattice_.addEdge(
                        lattice_.getEdgeSource(lastLemma_),
                        lattice_.getEdgeTarget(lastLemma_),
                        item,
                        lexemeTag_,
                        seqBuilder.build(),
                        score,
                        ruleId);
            }

            virtual void doAddForm(
                const AnnotationItem& item,
                Lattice::Score score,
                int ruleId) {

                Lattice::EdgeSequence::Builder seqBuilder(lattice_);
                seqBuilder.addEdge(lastLexeme_);

                lattice_.addEdge(
                    lattice_.getEdgeSource(lastLexeme_),
                    lattice_.getEdgeTarget(lastLexeme_),
                    item,
                    formTag_,
                    seqBuilder.build(),
                    score,
                    ruleId);
            }
        };

    public:
        Worker(Processor& processor, Lattice& lattice)
            :LatticeWorker(lattice),
             processor_(processor),
             tokenMask_(lattice.getLayerTagManager().getMask("token")) {
        }

   private:
        virtual void doRun() {
            L& lemmatizer = dynamic_cast<LemmatizerAnnotator&>(processor_).lemmatizer_;
            LayerTagCollection layerTags =
                lattice_.getLayerTagManager().createTagCollection(lemmatizer.getLayerTags());

            Lattice::EdgesSortedByTargetIterator edgeIterator
                = lattice_.edgesSortedByTarget(tokenMask_);

            while (edgeIterator.hasNext()) {
                Lattice::EdgeDescriptor edge = edgeIterator.next();

                if (lattice_.getAnnotationCategory(edge) == "T"
                    || lattice_.getAnnotationCategory(edge) == "word") {
                    WorkerOutputIterator outputIterator(layerTags, lattice_, edge);
                    lemmatizeSingleWord(lattice_.getAnnotationText(edge), outputIterator);
                }
            }
        }

        void lemmatizeSingleWord(const std::string& word, WorkerOutputIterator& outputIterator) {
            L& lemmatizer = dynamic_cast<LemmatizerAnnotator&>(processor_).lemmatizer_;
            GeneralCaseConverter<std::string::const_iterator,
                                 std::back_insert_iterator<std::string> >& lowerCaseConverter =
                *(dynamic_cast<LemmatizerAnnotator&>(processor_).lowerCaseConverter_);

            std::string w = word;

            lemmatizer.lemmatize(
                w,
                lattice_.getAnnotationItemManager(),
                outputIterator);

            if (simpleWillBeTouchedWhenTailConverted(lowerCaseConverter, w)) {
                w = simpleTailConvert(lowerCaseConverter, w);
                lemmatizer.lemmatize(
                    w,
                    lattice_.getAnnotationItemManager(),
                    outputIterator);
            }

            if (simpleWillBeTouchedWhenHeadConverted(lowerCaseConverter, w)) {
                lemmatizer.lemmatize(
                    simpleHeadConvert(lowerCaseConverter, w),
                    lattice_.getAnnotationItemManager(),
                    outputIterator);
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
