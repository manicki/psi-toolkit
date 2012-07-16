#include "grules.h"


GRule::~GRule()
{
  delete expr;
  delete extra_expr;
  delete right_symbols;
  delete left_symbol;
  delete recipe;
}

GRuleList::~GRuleList()
{
    vector<GRule*>::iterator iter = begin();
    for( ; iter != end(); ++iter)
    {
    delete (*iter);
    }
}

// ==================================================================

GRuleRightSymbolsSeq::~GRuleRightSymbolsSeq()
{
    vector<GRuleRightSymbolAltsSeq*>::iterator iter = begin();
    for( ; iter != end(); ++iter)
    {
    delete (*iter);
    }
}


// ==================================================================
GRuleRightSymbolAltsSeq::~GRuleRightSymbolAltsSeq()
{
    delete atom;

    vector<GRuleRightSymbolsSeq*>::iterator iter = alts.begin();
    for( ; iter != alts.end(); ++iter)
    delete (*iter);
}

GRuleTreeRecipe::~GRuleTreeRecipe()
{
  if(this->path)
  {
    GRuleTreeRecipePath::iterator i=this->path->begin();

    while(i!=this->path->end())
    {
      delete (*i);
      ++i;
    }
  }

  if(this->subrecipes)
  {
    list<GRuleTreeRecipe*>::iterator i=this->subrecipes->begin();

    while(i!=this->subrecipes->end())
    {
      delete (*i);
      ++i;
    }
  }

  delete category;
  delete label;
  delete symbol;
  delete path;
  delete subrecipes;
}


// ==================================================================
