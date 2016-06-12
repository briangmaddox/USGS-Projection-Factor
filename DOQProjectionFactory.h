#ifndef DOQPROJECTIONFACTORY_H_
#define DOQPROJECTIONFACTORY_H_

#include "ProjectionFactory.h"
#include "ImageLib/DOQImageIFile.h"
#include "ProjectionLib/UTMProjection.h"
#include "ProjectionLib/StatePlaneProjection.h"

namespace ProjFactory
{

/** Define some error messages that can be used with the class */
#define PROJDOQ_NOINPUT "DOQProjectionFactory: Unable to open input file"
#define PROJDOQ_NOMEM   "DOQProjectionFactory: no memory"
  
class DOQProjectionFactory : public ProjectionFactory
{
public:
  /** Main constructor for the class */
  DOQProjectionFactory();
  
  /** Class destructor */
  ~DOQProjectionFactory();

  
  /** Create function creates a projection
      from a file */
  virtual ProjLib::Projection * create(std::string  filename)
    throw (ProjFactoryException);


};

}//namespace

#endif
