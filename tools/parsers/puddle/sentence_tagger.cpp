
#include "sentence_tagger.hpp"

namespace poleng
{

namespace bonsai
{

    Tagger::Tagger(puddle::PuddlePtr parser_)
    {
        this->parser = parser_;
    }

//Tagger::Tagger(std::string tagset_file, std::string desc_file, std::string rules_file, std::string dict_file)
//{
//    parser.loadTagset(tagset_file.c_str(), desc_file.c_str());
//    parser.loadRules(rules_file.c_str());
//    parser.initNewTagger(dict_file.c_str());
////    converter.loadTagset("parser/tagset.pl.cfg", "parser/desc.pl.cfg");
//}

Tagger::~Tagger()
{
}

std::string Tagger::tagSentence(std::string sentence)
{
    if (sentence.length() == 0)
        return "";

    ParseGraphPtr pg = parser->parseString(sentence);
//    converter.addDescription(pg);

    std::stringstream ss;
    ParseGraph::Graph *g = pg->getBoostGraph();

    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

    int i = 0;
    while (v != end)
    {
        v = vertex(i, *g);
        for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first)
        {
            ParseGraph::Edge e = *p.first;
            if (map[e].getDepth() == 0)
            {
                if (i > 0)
                    ss << " ";
                ss << map[e].getLabel();
            }
        }
        i ++;
    }


    return ss.str();
}

}

}

//
//int main (int argc, char **argv)
//{
//    Tagger tagger;
//    std::string sentence = tagger.tagSentence("Ala ma kota");
//}
