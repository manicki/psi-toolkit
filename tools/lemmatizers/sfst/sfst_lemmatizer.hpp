#ifndef SFST_HDR
#define SFST_HDR

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include <sfst-1.0/sfst/fst.h>
#include "annotation_item_manager.hpp"
#include "lemmatizer_output_iterator.hpp"

#include <map>
#include <list>
#include <set>

class SfstLemmatizer {
	public:

    SfstLemmatizer(const boost::program_options::variables_map& options);
    
    static std::string getName();
    
    std::string getLanguage() const;
    
    /**
     * Sets the word processing level, default 0.
     * @param level
     *  Word processing level:
     *  0 - normal mode...
     */
    void setLevel(int);
    
    
    /**
     * Stems word using sfstLematizer tool.
     * @param word
     *  A string containing word to stem.
     * @return
     *  A vector of found stems with repetitions.
     * @see stem()
     */
    std::vector<std::string> simpleStem(const std::string & word);
    
    private:
		AnnotationItemManager * annotationManager;
		int level;
		
		static std::string tagSeparator;
		
		Transducer turkishTransducer;
		
		void initializeTurkishTransducer();
};

#endif
