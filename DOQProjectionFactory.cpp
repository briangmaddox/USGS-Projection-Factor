#ifndef DOQPROJECTIONFACTORY_CPP_
#define DOQPROJECTIONFACTORY_CPP_

#include "DOQProjectionFactory.h"


using namespace ProjFactory;

//************************************************************
DOQProjectionFactory::DOQProjectionFactory() : ProjectionFactory()
{}

//************************************************************
DOQProjectionFactory::~DOQProjectionFactory()
{}

  
//***************************************************************
ProjLib::Projection * DOQProjectionFactory::
create(std::string  filename)
  throw (ProjFactoryException)
{
  ProjLib::DATUM datum;
  std::string sdatum, coordsystem;
  ProjLib::Projection * proj = NULL;
  int zone;
  USGSImageLib::DOQImageIFile * indoq = NULL;
  try
  {
    
    if (!(indoq = new (std::nothrow) USGSImageLib::DOQImageIFile
          (filename)))
      throw std::bad_alloc();
    
    if (!indoq->good())
      throw ProjFactoryException(PROJDOQ_NOINPUT);


    indoq->getCoordinateZone(zone);
    indoq->getHorizontalDatum(sdatum);
    indoq->getHorizontalCoordinateSystem(coordsystem);
    
    //now check the datum 
    if ( (sdatum == std::string("NAD83")) || 
         (sdatum == std::string("nad83")))
      datum = ProjLib::NAD83;
    else
      datum = ProjLib::NAD27;
    
    if ( (coordsystem == std::string("UTM")) || 
         (coordsystem == std::string("utm")))
    {
      
      //It is probably a bad thing to assume that it is in meters
      if (!(proj = new (std::nothrow) ProjLib::UTMProjection(zone, 
                                                    datum, ProjLib::METERS)))
        throw std::bad_alloc();
    }
    else
    {
      //State plane case
      //set the param files
      if (datum == ProjLib::NAD83)
        ProjLib::StatePlaneProjection::setNAD83ParameterFilename
          (std::string("./nad83sp"));
      
      if (datum == ProjLib::NAD27)
        ProjLib::StatePlaneProjection::setNAD27ParameterFilename
          (std::string("./nad27sp"));
      
      if (!(proj = new (std::nothrow) ProjLib::StatePlaneProjection
            (zone, datum, ProjLib::US_FEET)))
        throw std::bad_alloc();
      
    }
    projlist.push_back(proj);
    return proj;
  }
  catch(std::bad_alloc & badal)
  {
    if (proj)
      delete proj;

    throw ProjFactoryException(PROJDOQ_NOMEM);
  }
  catch(ProjFactoryException & tempexp)
  {
    if (proj)
      delete proj;
    throw tempexp;
  }
  catch(...)
  {
    if (proj)
      delete proj;

    throw ProjFactoryException();
  }
}

#endif

