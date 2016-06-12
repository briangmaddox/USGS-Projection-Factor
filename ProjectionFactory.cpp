#ifndef PROJECTIONFACTORY_CPP_
#define PROJECTIONFACTORY_CPP_

#include "ProjectionFactory.h"

using namespace ProjFactory;

//***************************************************
ProjectionFactory::ProjectionFactory()
{
}

//***************************************************
ProjectionFactory::~ProjectionFactory()
{
  flush();           //flush the factory
}

//**************************************************
void ProjectionFactory::flush() throw()
{
  unsigned int counter;
  ProjLib::Projection * Temp = NULL;
  //remove all projections
  for (counter = 0; counter < projlist.size(); counter++)
  {
    Temp = projlist.front();
    projlist.pop_front();
    delete Temp;
  }
}
 
//****************************************************
bool ProjectionFactory::remove( ProjLib::Projection * indelete) throw()
{
  std::list<ProjLib::Projection *>::iterator it;
  //find and delete the projection
  for (it = projlist.begin(); it != projlist.end(); it++)
  {
    if ((*it) == indelete)
    {
      delete indelete;
      projlist.erase(it);
      return true;
    }
  }
  
  return false;
}


#endif
