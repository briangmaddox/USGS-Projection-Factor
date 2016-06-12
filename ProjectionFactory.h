#ifndef PROJECTIONFACTORY_H_
#define PROJECTIONFACTORY_H_

#include <new>
#include <list>
#include <string>
#include "ProjectionLib/Projection.h"
#include "ProjFactoryException.h"

namespace ProjFactory
{


/** This class is the base factory for 
    creating projections. */
class ProjectionFactory 
{

public:

  /** Main constructor for the class */
  ProjectionFactory();

  /** Main Destructor for the class */
  virtual ~ProjectionFactory();

  /** Create function creates a projection
      from a file */
  virtual ProjLib::Projection * create(std::string  filename)
    throw (ProjFactoryException) = 0;

  /** flush function flushes all projection */
  void flush() throw();
  
  /** delete function deletes the projection */
  bool remove( ProjLib::Projection * indelete) throw();

protected:

  /** The list to store created projections */
  std::list<ProjLib::Projection *> projlist;

};

}//namespace

#endif
    
