#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "TransitionInfo.hpp"

using namespace poleng::bonsai;

void TransitionInfo::addMorphology(PosInfo p) {
    variants_.push_back(p);
}

std::vector<TransitionInfo> TransitionInfo::getPosTransitions() {
    std::vector<TransitionInfo> posTransitions;
    
    std::string lastPosTag;
    for(std::vector<PosInfo>::iterator it = variants_.begin(); it != variants_.end(); ++it) {
        std::string base; std::string morpho; int disamb_sh; 
        boost::tie(base, morpho, disamb_sh) = *it;
        std::vector<std::string> morphs;
        
        if(disamb_sh == 1) {
            boost::split(morphs, morpho, boost::is_any_of(":"));
            std::string posTag = morphs.at(0);
            if(posTag.compare(lastPosTag) == 0) {
                posTransitions.back().addMorphology(PosInfo(base, morpho, disamb_sh));
            }
            else {
                TransitionInfo p(*this);
                p.setType("pos");
                p.setLabel(posTag);
                p.variants_.clear();
                p.addMorphology(PosInfo(base, morpho, disamb_sh));
                posTransitions.push_back(p);
                lastPosTag = posTag;
            }
        }
        
    }
    depth_ = depth_ + 1;
    return posTransitions;
}

std::string TransitionInfo::getMorphologyString() {
    std::stringstream ss;
    
    ss << "\\[";
    if (type_ == "group")
        ss << "head=" << head_ << ";";
    ss << "id=\"" << id_ << "\"";
    for(std::vector<PosInfo>::iterator it = variants_.begin(); it != variants_.end(); ++it) {
        std::string base; std::string morpho; int disamb_sh; 
        boost::tie(base, morpho, disamb_sh) = *it;
        
        if(it != variants_.begin()) {
          ss << ",";  
        }
        ss << base << ":" << morpho;
//        ss << morpho;
        if (!disamb_sh)
            ss << "<- deleted!";
    }
    ss << "\\]";
    return ss.str();
}

std::vector<std::string> TransitionInfo::getReadableMorphology()
{
    std::vector<std::string> mphs;

    for(std::vector<PosInfo>::iterator it = variants_.begin(); it != variants_.end(); ++it) {
        std::string base; std::string morpho; int disamb_sh; 
        boost::tie(base, morpho, disamb_sh) = *it;

        if (disamb_sh)
        {
            std::string s = base + ":" + morpho;
            mphs.push_back(s);
        }
    }
    return mphs;
}


void TransitionInfo::setDesc(std::vector<int> aDesc)
{
    desc = aDesc;
    //label_desc_ = "";

    if (desc_on)
    {
    std::vector<PosInfo>::iterator it = variants_.begin();
    while (it != variants_.end())
    {
//    if (desc.size() > 0)
//    {
    PosInfo pi = *it;
    if (!boost::get<2>(pi))
    {
        it ++;
        continue;
    }
    std::string morphology = boost::get<1>(pi);

//    int count = 0;
//    int p = 0;
//    while (morphology.find(":", p) >= 0)
//    {
//        p = morphology.find(":", p) + 1;
//        count ++;
//    }
//    std::vector<std::string> values;
//    int index = morphology.find(":");
//    std::string ret = morphology.substr(0, index);
    std::string ret = "";
    std::string mret = "";
//    std::string modify = morphology.substr(index + 1, std::string::npos);
//    morphology = modify;
    int index = 0;
    int j = 0;
    std::vector<int>::iterator id = desc.begin();
    while (id != desc.end()) // przeglada tablice desc, ktora zawiera definicje opisu:
                             // wartosci w tablicy to indeksy atrybutow przypisanych do czesci, ktore maja byc przepisane w opisie
                             // np.
                             // dla wpisu w tagsecie: subst = number gender person
                             // i wpisu w definicji opisu: subst = gender
                             // indeks znajdujacy sie w tablicy desc to 2 (numerowanie od zera, przy czym 0 odpowiada czesci mowy, a wartosci dodatnie to kolejne atrybuty przypisane do danej czesci mowy)
                             // specjalne wartosci indeksow:
                             // -1 - forma bazowa
                             // -2 - forma ortograficzna

    //while (morphology.find(":", index) != std::string::npos)
    {
        if (*id < 0)
        {
            if (mret != "")
            {
                if (ret != "")
                    ret += desc_sep;
                ret += mret;
                mret = "";
            }
        }
        if (*id == -1)
        {
            if (ret != "")
                ret += desc_sep;
            ret += boost::get<0>(pi);
            id ++;
            continue;
        }
        if (*id == -2)
        {
            if (ret != "")
                ret += desc_sep;
            //ret += label_;
            ret += orth_;
            id ++;
            continue;
        }
        index = morphology.find(":");
        if (j == *id)
        {
            //if (index >= 0)
            //{
            std::string add = morphology.substr(0, index);
            if (mret != "")
                mret += ":";
            mret += add;
            //}
            id ++;
            morphology = boost::get<1>(pi);
            j = 0;
            index = 0;
            continue;
        }
        if (index >= 0)
            morphology = morphology.substr(index + 1, std::string::npos);
        else
        {
            id ++;
            continue;
        }
        j ++;
    }
    if (mret != "")
    {
        if (ret != "")
            ret += desc_sep;
        ret += mret;
    }
    if (ret == "")
        ret = label_;
//    if (ret != "")
//        ret = label_ + "#" + ret;
//    else
//        ret = label_;
    label_ = ret;
    break;
    }
    }
//    }

}

void TransitionInfo::unsetDesc()
{
    label_ = orth_;
}

//void TransitionInfo::setLabelDesc(std::string aDesc)
//{
//    label_desc_ = label_ + desc_sep + aDesc;
//}

//std::string TransitionInfo::getLabelDesc()
//{
//    if (desc_on)
//    {
//    if (label_desc_ != "")
//        return label_desc_;
//    else
//    {
//        return label_;
//    }
//    }
//    else
//        return label_;
////    PosInfo pi = variants_.front();
////    std::string morphology = get<1>(pi);
//////    std::vector<std::string> values;
//////    int index = morphology.find(":");
//////    std::string ret = morphology.substr(0, index);
////    std::string ret = "";
//////    std::string modify = morphology.substr(index + 1, std::string::npos);
//////    morphology = modify;
////    int index = 0;
////    int j = 0;
////    std::vector<int>::iterator it = desc.begin();
////    while (it != desc.end())
////    //while (morphology.find(":", index) != std::string::npos)
////    {
////        index = morphology.find(":");
////        if (j == *it)
////        {
////            std::string add = morphology.substr(0, index);
////            if (ret != "")
////                ret += ":";
////            ret += add;
////            it ++;
////            morphology = get<1>(pi);
////            j = 0;
////            index = 0;
////            continue;
////        }
////        morphology = morphology.substr(index + 1, std::string::npos);
////        j ++;
////    }
////    if (ret != "")
////        ret = label_ + "#" + ret;
////    else
////        ret = label_;
////    return ret;
//}

//void TransitionInfo::deleteMorphology(string &dBase, string &dMorpho)
//{
//    for (vector<PosInfo>::iterator it = variants_.begin(); it != variants_.end(); it ++)
//    {
//        string base, morpho;
//        int disamb_sh;
//        tie (base, morpho, disamb_sh) = *it;
//        if ((base == dBase) && (morpho == dMorpho))
//            get<2>(*it) = 0;
//    }
//}
