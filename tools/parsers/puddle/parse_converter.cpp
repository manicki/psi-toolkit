
#include "parse_converter.hpp"

namespace poleng
{
namespace bonsai
{

ParseConverter::ParseConverter()
{
}

ParseConverter::~ParseConverter()
{
}

std::string ParseConverter::convert(ParseGraph graph)
{
    std::stringstream ss;
//    VertexIndex index = boost::get(vertex_index, g)
    ParseGraph::Graph *g = graph.getBoostGraph();
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);    
    ParseGraph::VertexIndex index = get(boost::vertex_index, *g);
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
    ss << "<!DOCTYPE cesAna SYSTEM \"xcesAnaIPI.dtd\">" << std::endl;
    ss << "<cesAna version=\"1.0\" type=\"lex disamb\">" << std::endl;
    ss << "<chunkList>" << std::endl;
    ss << "<chunk type=\"s\">" << std::endl;

    std::vector<std::pair<int, std::string> > edgeStack;

    int i = 0;
    while ((v = vertex(i, *g)) != end)
    {

        std::vector<std::pair<int, std::string> >::iterator it = edgeStack.end();
        while (it != edgeStack.begin())
        {
            it --;
            if (it->first == i)
            {
                ss << "</" << it->second << ">" << std::endl;
                it = edgeStack.erase(it);
                continue;
            }
            //it ++;
        }

        std::vector<ParseGraph::Edge> edges;
    for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first) {
        ParseGraph::Edge e = *p.first;
        if (((map[e].getEnd() - map[e].getStart()) == 1) && (map[e].getType() == "token"))
        {
            bool tokenBefore = false;
            std::vector<ParseGraph::Edge>::iterator j = edges.begin();
            while (j != edges.end())
            {
                if (map[*j].getType() == "token")
                    if ((map[*j].getEnd() - map[*j].getStart()) > 1)
                    {
                        tokenBefore = true;
                        break;
                    }
                j ++;
            }

            if (tokenBefore)
            {
                edges.insert(j, e);
                continue;
            }
        }
        edges.push_back(e);
    }

    std::vector<ParseGraph::Edge>::iterator ei = edges.end();
    while (ei != edges.begin())
    {
        ei --;

     //   if (index[target(e, *g)] <= index[end]) {
            if (map[*ei].getType() == "token")
            {
                if ((map[*ei].getEnd() - map[*ei].getStart()) == 1)
                {
                ss << "<tok" 
                    << " id=\"" << map[*ei].getId() << "\">" 
                    << std::endl;
                ss << "<orth>" << map[*ei].getLabel() << "</orth>" << std::endl;
                for (std::vector<PosInfo>::iterator var = map[*ei].variants_.begin(); var != map[*ei].variants_.end(); var ++)
                {
                    ss << "<lex";
                    if (!boost::get<2>(*var))
                        ss << " disamb_sh=\"0\"";
                    ss << ">";
                    ss << "<base>" << boost::get<0>(*var) << "</base>";
                    ss << "<ctag>" << boost::get<1>(*var) << "</ctag>";
                    ss << "</lex>";
                    ss << std::endl;
                }
                }
                else
                {
                    ss << "<syntok"
                        << " id=\"" << map[*ei].getId() << "\""
                        << ">" << std::endl;
                }
            }
            else if (map[*ei].getType() == "group")
            {
                ss << "<group"
                    << " id=\"" << map[*ei].getId() << "\""
                    << " type=\"" << map[*ei].getLabel() << "\""
                    << " head=\"" << map[*ei].getHead() << "\""
                    << ">"
                    << std::endl;
            }

            if (map[*ei].getType() == "group") 
                edgeStack.push_back(std::pair<int, std::string>(map[*ei].getEnd(), "group")); 
            if (map[*ei].getType() == "token")
                if ((map[*ei].getEnd() - map[*ei].getStart()) == 1)
                    edgeStack.push_back(std::pair<int, std::string>(map[*ei].getEnd(), "tok")); 
                else
                    edgeStack.push_back(std::pair<int, std::string>(map[*ei].getEnd(), "syntok"));
       // }
    }
    i ++;
    }

    std::vector<std::pair<int, std::string> >::iterator it = edgeStack.end();
    while (it != edgeStack.begin())
    {
        it --;
        if (it->first == i)
        {
            ss << "</" << it->second << ">" << std::endl;
            it = edgeStack.erase(it);
            continue;
        }
        //it ++;
    }

    ss << "</chunk>" << std::endl;
    ss << "</chunkList>" << std::endl;
    ss << "</cesAna>" << std::endl;
    return ss.str(); 
}

void ParseConverter::setTagset(puddle::TagsetPtr tagset_)
{
    this->tagset = tagset_;
}

void ParseConverter::addDescription(ParseGraphPtr pg)
{
    ParseGraph::Graph *g = pg->getBoostGraph();
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);    
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

    std::map<std::string, std::string> desc_by_id;

    int i = 0;

    while (v != end)
    {
        v = vertex(i, *g);
        for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first) 
        {
            ParseGraph::Edge e = *p.first;

            if ((map[e].getDepth() == 0) || (map[e].getDepth() > 0 && map[e].getLabel() == "SYNTOK"))
            {
                std::vector<PosInfo>::iterator it = map[e].variants_.begin();
                while (it != map[e].variants_.end())
                {
                    PosInfo pi = *it;
                    if (!boost::get<2>(pi))
                    {
                        it ++;
                        continue;
                    }
                    std::string desc = "";
                    std::string desc_non_orth;
                    for (std::vector<int>::iterator d = tagset->desc_order.begin(); d != tagset->desc_order.end(); d ++)
                    {
                        if (*d == DESC_BASE)
                        {
                            if (desc != "")
                                desc += tagset->desc_separator;
                            desc += boost::get<0>(pi);
                            if (desc_non_orth != "")
                                desc_non_orth += tagset->desc_separator;
                            desc_non_orth += boost::get<0>(pi);
                        }
                        else if (*d == DESC_ORTH)
                        {
                            if (desc != "")
                                desc += tagset->desc_separator;
                            desc += map[e].getOrth();
                        }
                        else if (*d == DESC_MORPHO)
                        {
                            std::string morpho = boost::get<1>(pi);
                            std::string pos = morpho.substr(0, morpho.find(":"));
                            std::vector<std::string> desc_ = tagset->getTokenDesc(pos);
                            if (desc_.size() > 0)
                            {
                                for (std::vector<std::string>::iterator d = desc_.begin(); d != desc_.end(); d ++)
                                {
                                    boost::regex reg(*d);
                                    boost::smatch what;
                                    if (boost::regex_match(morpho, what, reg))
                                    {
                                        if (desc != "")
                                            desc += tagset->desc_separator;
                                        desc += what[1];
                                        if (desc_non_orth != "")
                                            desc_non_orth += tagset->desc_separator;
                                        desc_non_orth += what[1];
                                    }
                                }
                            }
                            else
                            {
                                if (tagset->desc_include_pos)
                                {
                                    if (desc != "")
                                        desc += tagset->desc_separator;
                                    desc += pos;
                                    if (desc_non_orth != "")
                                        desc_non_orth += tagset->desc_separator;
                                    desc_non_orth += pos;
                                }
                            }
                            map[e].setDesc(desc);
                            desc_by_id.insert(std::pair<std::string, std::string>(map[e].getId(), desc_non_orth));
                        }
                        
                    }
                    break;
                }
                if (map[e].getDepth() == 0)
                    break;
            }
        }
        i ++;
    }
    i = 0;
    v = vertex(0, *g);
    while (v != end)
    {
        v = vertex(i, *g);
        for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first) 
        {
            ParseGraph::Edge e = *p.first;

            if (((map[e].getType() == "group") && (map[e].getLabel() != "SYNTOK")) || (map[e].getType() == "pos"))
            {
                //TODO sprawdzic czy nie ma typ swojego desc, jak ma to wziac, jak nie, to brac z glowy!
                std::vector<std::string> grdesc = tagset->getGroupDesc(map[e].getLabel());
                std::string desc = "";
                if (grdesc.size() > 0)
                {
                    std::vector<PosInfo>::iterator it = map[e].variants_.begin();
                    while (it != map[e].variants_.end())
                    {
                        PosInfo pi = *it;
                        if (!boost::get<2>(pi))
                        {
                            it ++;
                            continue;
                        }
                    std::string morpho = boost::get<1>(pi);

                    for (std::vector<std::string>::iterator d = grdesc.begin(); d != grdesc.end(); d ++)
                    {
                        boost::regex reg(*d);
                        boost::smatch what;
                        if (boost::regex_match(morpho, what, reg))
                        {
                            if (desc != "")
                                //desc += tagset.desc_separator;
                                desc += ":";
                            desc += what[1];
                        }
                                
                    }
                    break;
                    }
                }
                else
                {
                    std::map<std::string, std::string>::iterator it = desc_by_id.find(map[e].getHead());
                    if (it != desc_by_id.end())
                    {
                        desc = it->second;
                    }
                }
                map[e].desc_on = tagset->desc_nonterminals;
                map[e].desc_sep = tagset->desc_separator;
                if (tagset->desc_nonterminals && desc != "")
                    //map[e].setLabel(map[e].getLabel() + tagset->desc_separator + desc);
                    map[e].setDesc(map[e].getLabel() + tagset->desc_separator + desc);
            }
        }
        i ++;
    }
}

void ParseConverter::removeDescription(ParseGraphPtr pg)
{
    ParseGraph::Graph *g = pg->getBoostGraph();
    ParseGraph::Vertex v = vertex(0, *g);
    ParseGraph::Vertex end = vertex(boost::num_vertices(*g) - 1, *g);    
    ParseGraph::TransitionMap map = get(boost::edge_bundle, *g);

    std::map<std::string, std::vector<int> > desc_by_id;

    int i = 0;

    while (v != end)
    {
        v = vertex(i, *g);
        for (std::pair <ParseGraph::OutEdgeIt, ParseGraph::OutEdgeIt> p = out_edges(v, *g); p.first != p.second; ++ p.first) 
        {
            ParseGraph::Edge e = *p.first;
            map[e].unsetDesc();
        }
        i ++;
    }
}

}

}
