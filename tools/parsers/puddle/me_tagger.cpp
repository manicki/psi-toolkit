#include "me_tagger.hpp"

namespace poleng {

namespace bonsai {
    namespace puddle {

        MeTagger::MeTagger() : openForEvents(false), posModelLoaded(false), Tagger() {
            rxUpperCaseFirst = boost::make_u32regex("^[[:Lu:]]");
            rxUpperCaseAll   = boost::make_u32regex("^[[:Lu:]]+$");

            rxContainsNumber = boost::make_u32regex("\\d");
            rxIsNumber = boost::make_u32regex("^\\d+([.,]\\d+)(%)?$");

            rxContainsLetter = boost::make_u32regex("[[:L:]]");
            rxContainsPunct  = boost::make_u32regex("[.,;:?!<>%&'\\-\"/()\\[\\]+=_*$#@~|{}\\\\]");
        }

        std::string MeTagger::tagSentence(std::string sentence, Entities &entities, Edges &edges) {
            std::string untagged_sentence = Tagger::tagSentence(sentence, entities, edges);

            if( !posModelLoaded ) // dzialaj jak Tagger, gdy nie jest zaladowany model
                return untagged_sentence;

            return tag(untagged_sentence, entities, edges);
        }

        std::string MeTagger::tagSentenceITF(std::string sentence, Entities &entities, Edges &edges) {

            if(! posModelLoaded) // dzialaj jak Tagger, gdy nie jest zaladowany model
                return sentence;

            return tag(sentence, entities, edges);
        }

        std::string MeTagger::tag(std::string sentence, Entities &entities, Edges &edges)
        {
            int n = tagset->getNumberOfAttributes()+1;

            std::vector<Outcome> tags;
            tags.push_back("sb");
            for(int i = 1; i < entities.size()-1; i++) {
                Outcome prevtag = tags[i-1];
                Context context = create_context(entities, i, 2);
                context.push_back(Feature("prevtag=" + prevtag));

                Token* curr_tok = (Token*)entities[i];
                std::string curr_orth = curr_tok->getOrth();
                std::vector<std::string> amb = curr_tok->getCompiledInterpretations();

                Outcome best_tag = "empty";
                double best_prob = 0;

                for(std::vector<std::string>::iterator it = amb.begin(); it != amb.end(); it++) {
                    std::string tag   = it->substr( 0, n );
                    std::string lemma = it->substr( n );

                    std::string ign = "ign";
                    if( tag[0] == tagset->mapPos( ign ) ) {

                        std::vector<std::string> open_classes;

            if(boost::u32regex_search(curr_orth, rxContainsPunct) || boost::u32regex_search(curr_orth, rxContainsNumber)) {
                open_classes.push_back("card");
                open_classes.push_back("name");
            }
            else {
                open_classes = tagset->getOpenClasses();
            }

            if( open_classes.size() > 0 ) {

                BOOST_FOREACH( std::string pos, open_classes ) {
                    char pos_char = tagset->mapPos( pos );
                    std::string mapped_pos = boost::lexical_cast<std::string>(pos_char);

                    std::vector<Outcome> open_tags = tagset->getMappedMorphologies( mapped_pos );
                    BOOST_FOREACH ( Outcome open_tag, open_tags ) {

                        double prob = m.eval( context, open_tag );
                        //std::cerr << lemma << " " << tagset->unmapMorphology(open_tag) << " " << prob << std::endl;

                        if( prob > best_prob ) {
                            best_tag  = open_tag;
                            best_prob = prob;
                        }

                    }
                }

            }
            else {
                best_tag = m.predict( context );
            }

                        break;
                    }
                    else {
                        double prob = m.eval(context, tag);

                        /*if(prob == -1) {
                          best_prob = -1;
                          best_tag = tag;
                          break;
                          }*/

                        if(prob > best_prob) {
                            best_tag = tag;
                            best_prob = prob;
                        }
                    }
                }

                if(best_tag == "empty") {
                    best_tag = m.predict(context);
                }

                tags.push_back(best_tag);
            }
            tags.push_back("se");

            std::stringstream tagged_sentence;
            for(int i=0; i<tags.size(); i++) {
                if( entities[i]->getType() == "token" ) {
                    std::string tag = tags[i];
                    Token *token = (Token*)entities[i];

                    token->deleteInterpretation(tag_to_regex(tag), edges[i-1]);
                    token->updateCompiled();

                    if(token->getCompiledInterpretations().size() == 0) {  // jesli zostaly usuniety wszystkie interpretacje
                        std::string compiled = tag + token->getOrth();
                        std::string morph = token->getOrth() + ":" + tagset->unmapMorphology(tag);

                        token->addInterpretation(morph,compiled);
                        token->updateCompiled();

                        TransitionInfo *ti = edges[i-1];
                        PosInfo pi(token->getOrth(), tagset->unmapMorphology(tag), 1);
                        ti->addMorphology(pi);

                    }
                    tagged_sentence << token->getCompiled();
                }
                else if( entities[i]->getType() == "special" ) {
                    SpecialToken* t = (SpecialToken*)entities[i];
                    tagged_sentence << t->getCompiled();
                }
            }

            return tagged_sentence.str();
        }
        /*
        $test !~ /(NAA000)/
        $test =~ /([^N].....|N[^A]....|NA[^A]...|NAA[^0]..|NAA0[^0].|NAA00[^0])/
        */

        boost::u32regex MeTagger::tag_to_regex(std::string tag) {
            std::stringstream regex_str;
            regex_str << "[^<>]+<(";
            for(int i = 0; i < tag.size(); i++) {
                regex_str << tag.substr(0,i);
                regex_str << "[^" << tag.substr(i,1) << "]";
                for(int j=0; j<tag.size()-i-1; j++)
                    regex_str << ".";
                if(i < tag.size()-1)
                    regex_str << "|";
            }
            regex_str << ")[^<>]+";
            return boost::make_u32regex(regex_str.str());
        }

        void MeTagger::add_sample_sentence(std::string tagged) {
            if(! openForEvents) {
                m.begin_add_event();
                openForEvents = true;
            }

            int n = tagset->getNumberOfAttributes()+1;

            Entities tagged_entities; Edges tagged_edges;
            Entities plain_entities; Edges plain_edges;

            Tagger::processInput(tagged, tagged_entities, tagged_edges);

            std::stringstream plainstream;
            for(Entities::iterator it = tagged_entities.begin(); it != tagged_entities.end(); it++) {
                if((*it)->getType() == "token")
                    plainstream << ((Token*)(*it))->getOrth() << " ";
            }
            std::string plain = plainstream.str();
            Tagger::tagSentence(plain, plain_entities, plain_edges);

            for(int i=1; i<tagged_entities.size()-1; i++) {
                std::string prevtag = "sb";
                if(i > 0) {
                    Entity* prev = tagged_entities[i-1];
                    if(prev->getType() == "token") {
                        Token* prevtok = (Token*)prev;

                        std::vector<std::string> amb = prevtok->getCompiledInterpretations();
                        prevtag = amb[0].substr(0,n);
                    }
                }

                Context context = create_context(plain_entities, i, 2);
                context.push_back(Feature("prevtag=" + prevtag));

                Token* currtok = (Token*)tagged_entities[i];
                std::vector<std::string> amb = currtok->getCompiledInterpretations();
                Outcome currtag = amb[0].substr(0,n);

                if(currtag.substr(0,1) != "0") {
                    m.add_event(context, currtag);
                }
            }

            for(bonsai::puddle::Entities::iterator ent_it = tagged_entities.begin(); ent_it != tagged_entities.end(); ent_it++)
                delete *ent_it;
            for(bonsai::Edges::iterator edg_it = tagged_edges.begin(); edg_it != tagged_edges.end(); edg_it++)
                delete *edg_it;

            for(bonsai::puddle::Entities::iterator ent_it = plain_entities.begin(); ent_it != plain_entities.end(); ent_it++)
                delete *ent_it;
            for(bonsai::Edges::iterator edg_it = plain_edges.begin(); edg_it != plain_edges.end(); edg_it++)
                delete *edg_it;

        }

        void MeTagger::train(int iterations) {
            m.end_add_event();
            openForEvents = false;
            maxent::verbose = 1;
            m.train(iterations);
            maxent::verbose = 0;
        }

        void MeTagger::saveModel(std::string file) {
            m.save(file);
        }

        void MeTagger::loadModel(std::string file) {
            m.load(file);
            posModelLoaded = true;
        }

        MeTagger::Context MeTagger::create_context(Entities &sentence, int curr_index, int window) {
            int n = tagset->getNumberOfAttributes()+1;

            Context context;

            int sentence_length = sentence.size();
            Token* curr_tok = (Token*)sentence[curr_index];
            std::string curr_orth = curr_tok->getOrth();

            context.push_back( Feature( "curr_word=" + curr_orth ) );
            context.push_back( Feature( "curr_word_length=" + boost::lexical_cast<std::string>( curr_orth.size() ) ) );

            std::vector<std::string> amb = curr_tok->getCompiledInterpretations();
            for(std::vector<std::string>::iterator it = amb.begin(); it != amb.end(); it++) {
                std::string tag = it->substr(0,n);
                std::string lemma = it->substr(n);

                context.push_back(Feature("curr_has_tag=" + tag));
                context.push_back(Feature("curr_has_lemma=" + lemma));
            }

            if(boost::u32regex_search(curr_orth, rxUpperCaseFirst)) context.push_back("UpperCaseFirst");
            if(boost::u32regex_search(curr_orth, rxUpperCaseAll))   context.push_back("UpperCaseAll");
            if(boost::u32regex_search(curr_orth, rxContainsNumber)) context.push_back("ContainsNumber");
            if(boost::u32regex_search(curr_orth, rxIsNumber)) context.push_back("IsNumber");
            if(boost::u32regex_search(curr_orth, rxContainsLetter)) context.push_back("ContainsLetter");
            if(boost::u32regex_search(curr_orth, rxContainsPunct) && boost::u32regex_search(curr_orth, rxContainsNumber)) context.push_back("ContainsPunctAndNumber");
            if(!boost::u32regex_search(curr_orth, rxContainsLetter) && !boost::u32regex_search(curr_orth, rxContainsNumber)) context.push_back("ContainsNoLetterAndNoNumber");

            if(curr_orth.size() > 6) {
                context.push_back( Feature("curr_prefix=" + curr_orth.substr(0,1)) );
                context.push_back( Feature("curr_suffix=" + curr_orth.substr(curr_orth.size()-1)));

                context.push_back( Feature("curr_prefix=" + curr_orth.substr(0,2)) );
                context.push_back( Feature("curr_suffix=" + curr_orth.substr(curr_orth.size()-2)));

                context.push_back( Feature("curr_prefix=" + curr_orth.substr(0,3)) );
                context.push_back( Feature("curr_suffix=" + curr_orth.substr(curr_orth.size()-3)));

                context.push_back( Feature("curr_suffix=" + curr_orth.substr(curr_orth.size()-4)));
            }

            for(int i = 1; i <= window; i++) {

                // prev

                int pindex = 0;
                if(curr_index > i)
                    pindex = curr_index - 1;

                Token* prev_tok = (Token*)sentence[pindex];
                std::stringstream prev_word;
                prev_word << "prev" << i << "_word=" << prev_tok->getOrth();
                context.push_back( Feature(prev_word.str()) );

                if(pindex > 0) {
                    std::vector<std::string> prev_amb = prev_tok->getCompiledInterpretations();
                    for(std::vector<std::string>::iterator it = prev_amb.begin(); it != prev_amb.end(); it++) {
                        std::stringstream tag, lemma;
                        tag << "prev" << i << "_has_tag=" << it->substr(0,n);
                        lemma << "prev" << i << "_has_lemma=" << it->substr(n);

                        context.push_back(Feature( tag.str() ));
                        context.push_back(Feature( lemma.str() ));
                    }
                }
                else {
                    std::stringstream tag;
                    tag << "prev" << i << "_has_tag=" << "sb";
                    context.push_back(Feature( tag.str() ));
                }

                // next

                int nindex = curr_index + i;
                if(nindex >= sentence.size())
                    nindex = sentence.size() - 1;

                Token* next_tok = (Token*)sentence[nindex];
                std::stringstream next_word;
                next_word << "next" << i << "_word=" << next_tok->getOrth();
                context.push_back( Feature(next_word.str()) );

                if(nindex < sentence.size() - 1) {
                    std::vector<std::string> next_amb = next_tok->getCompiledInterpretations();
                    for(std::vector<std::string>::iterator it = next_amb.begin(); it != next_amb.end(); it++) {
                        std::stringstream tag, lemma;
                        tag << "next" << i << "_has_tag=" << it->substr(0,n);
                        lemma << "next" << i << "_has_lemma=" << it->substr(n);

                        context.push_back(Feature( tag.str() ));
                        context.push_back(Feature( lemma.str() ));
                    }
                }
                else {
                    std::stringstream tag;
                    tag << "prev" << i << "_has_tag=" << "se";
                    context.push_back(Feature( tag.str() ));
                }

            }

            std::set<std::string> uniq;
            uniq.insert( context.begin(), context.end() );
            context.clear();
            context.insert( context.begin(), uniq.begin(), uniq.end() );

            return context;
        }

    }
}

}

