#include "lattice_wrapper.hpp"
#include <queue>

#define __PARSE_TAG__ "parse"
#define __POS_TAG__ "lexeme"
#define __TOKEN_TAG__ "token"

bool isNonTerminal(Lattice::EdgeDescriptor edge, Lattice& lattice)
{
    LayerTagCollection edgeTags = lattice.getEdgeLayerTags(edge);
    if(lattice.getLayerTagManager().isThere(__TOKEN_TAG__, edgeTags))
	return false;
    return true;
}

Lattice::EdgeSequence getWordTokenSequence(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end,
    std::string langCode)
{
    Lattice::EdgeSequence::Builder seqBuilder(lattice);
    
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(__TOKEN_TAG__);
    Lattice::EdgeSequence tokens = lattice.getPath(start, tokenMask);
    Lattice::EdgeSequence::Iterator tokensIt
      = Lattice::EdgeSequence::Iterator(lattice, tokens);
    
    while(tokensIt.hasNext()) {
	Lattice::EdgeDescriptor token = tokensIt.next();
	if(lattice.getEdgeAnnotationItem(token).getCategory() != "B")
	    seqBuilder.addEdge(token);
        if(token == end)
	    break;
    }
    
    return seqBuilder.build();
}

Lattice::EdgeSequence getTopParseSequence(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end,
    std::string langCode)
{
    Lattice::EdgeSequence::Builder seqBuilder(lattice);
    
    LayerTagMask mask = lattice.getLayerTagManager().getAlternativeMask(
        lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(__PARSE_TAG__, langCode),
        lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(__POS_TAG__, langCode),
	lattice.getLayerTagManager().createSingletonTagCollectionWithLangCode(__TOKEN_TAG__, langCode)
    );
    
    Lattice::EdgeSequence topParseSeq = lattice.getPath(start, mask);
    
    Lattice::EdgeSequence::Iterator topParseSeqIt(lattice, topParseSeq);  
    while(topParseSeqIt.hasNext()) {
        Lattice::EdgeDescriptor topCandidate = topParseSeqIt.next();
        if(lattice.getEdgeAnnotationItem(topCandidate).getCategory() != "B")
	    seqBuilder.addEdge(topCandidate);
        if(topCandidate == end)
	    break;
    }
    
    return seqBuilder.build();
}

std::map<int, int> getCharWordTokenMap(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end,
    std::string langCode)
{
    int tokenNo = 0;
    std::map<int, int> charTokenMap;
    
    LayerTagMask tokenMask = lattice.getLayerTagManager().getMask(__TOKEN_TAG__);
    Lattice::EdgeSequence tokens = lattice.getPath(start, tokenMask);
    Lattice::EdgeSequence::Iterator tokensIt(lattice, tokens);
    
    while(tokensIt.hasNext()) {
	Lattice::EdgeDescriptor token = tokensIt.next();
	if(lattice.getEdgeAnnotationItem(token).getCategory() != "B") {
	    charTokenMap[lattice.getEdgeBeginIndex(token)] = tokenNo;
	    charTokenMap[lattice.getEdgeEndIndex(token)] = ++tokenNo;
	}
	if(token == end)
	    break;
    }
    
    return charTokenMap;
}


Lattice::EdgeSequence getSubTreeSymbols(Lattice::EdgeDescriptor node,
                                        Lattice& lattice,
                                        std::string langCode)
{
    Lattice::EdgeSequence::Builder seqBuilder(lattice);
        
    std::queue<Lattice::EdgeDescriptor> queue;
    queue.push(node);
    
    while(!queue.empty()) {
        Lattice::EdgeDescriptor currentEdge = queue.front();
        queue.pop();
        
        LayerTagCollection edgeTags = lattice.getEdgeLayerTags(currentEdge);
        std::list<Lattice::Partition> partitions = lattice.getEdgePartitions(currentEdge);
        
        if(partitions.size()) {
            Lattice::Partition partition = partitions.front();
            Lattice::Partition::Iterator pIt(lattice, partition);
            while(pIt.hasNext()) {
                Lattice::EdgeDescriptor subEdge = pIt.next();
                
                LayerTagCollection subEdgeTags = lattice.getEdgeLayerTags(subEdge);
                if (lattice.getLayerTagManager().isThere(__PARSE_TAG__, subEdgeTags)
                    || lattice.getLayerTagManager().isThere(__POS_TAG__, subEdgeTags))
                {
                    seqBuilder.addEdge(subEdge);
                    queue.push(subEdge);   
                }
                else if(lattice.getLayerTagManager().isThere(__TOKEN_TAG__, subEdgeTags)) {
                    seqBuilder.addEdge(subEdge);
                }
                else {
                    queue.push(subEdge); 	
                }
            }
        }
    }    
    return seqBuilder.build();
}

void getChildSymbolsRec(Lattice::EdgeDescriptor node,
                        Lattice& lattice,
                        std::string langCode,
			Lattice::EdgeSequence::Builder& seqBuilder)
{
    LayerTagCollection edgeTags = lattice.getEdgeLayerTags(node);
    std::list<Lattice::Partition> partitions = lattice.getEdgePartitions(node);
    
    if(partitions.size()) {
	Lattice::Partition partition = partitions.front();
	Lattice::Partition::Iterator pIt(lattice, partition);
	while(pIt.hasNext()) {
	    Lattice::EdgeDescriptor subNode = pIt.next();
	    
	    LayerTagCollection subNodeTags = lattice.getEdgeLayerTags(subNode);
	    if (lattice.getLayerTagManager().isThere(__PARSE_TAG__, subNodeTags)
		|| lattice.getLayerTagManager().isThere(__POS_TAG__, subNodeTags)
	        || lattice.getLayerTagManager().isThere(__TOKEN_TAG__, subNodeTags)) {
		seqBuilder.addEdge(subNode);
	    }
	    else {
		getChildSymbolsRec(subNode, lattice, langCode, seqBuilder); 	
	    }
	}
    }
}

Lattice::EdgeSequence getChildSymbols(Lattice::EdgeDescriptor node,
                                        Lattice& lattice,
                                        std::string langCode)
{
    Lattice::EdgeSequence::Builder seqBuilder(lattice);
    getChildSymbolsRec(node, lattice, langCode, seqBuilder);
    return seqBuilder.build();
}

Lattice::EdgeSequence getTreeSymbols(
    Lattice &lattice,
    Lattice::VertexDescriptor start,
    Lattice::VertexDescriptor end,
    std::string langCode)
{
    Lattice::EdgeSequence::Builder seqBuilder(lattice);
    
    Lattice::EdgeSequence topSymbols = getTopParseSequence(lattice, start, end, langCode);
    Lattice::EdgeSequence::Iterator topSymbolsIt(lattice, topSymbols);
    
    while(topSymbolsIt.hasNext()) {
        Lattice::EdgeDescriptor topEdge = topSymbolsIt.next();
        seqBuilder.addEdge(topEdge);
        Lattice::EdgeSequence subSymbols = getSubTreeSymbols(topEdge, lattice, langCode);
        Lattice::EdgeSequence::Iterator subSymbolsIt(lattice, subSymbols);
        while(subSymbolsIt.hasNext()) {
            Lattice::EdgeDescriptor subEdge = subSymbolsIt.next();
            seqBuilder.addEdge(subEdge);
        }
    }
    
    return seqBuilder.build();
}
