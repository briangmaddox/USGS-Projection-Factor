#ifndef GEOTIFFPROJECTIONFACTORY_H_
#define GEOTIFFPROJECTIONFACTORY_H_


#include "ProjectionFactory.h"
#include "ImageLib/GeoTIFFImageIFile.h"
#include "ImageLib/SPCSMapper.h"
#include "ProjectionLib/UTMProjection.h"
#include "ProjectionLib/StatePlaneProjection.h"
#include "ProjectionLib/AlbersConicProjection.h"
#include "ProjectionLib/LambertConformalConicProjection.h"
#include "ProjectionLib/MercatorProjection.h"
#include "ProjectionLib/PolarStereographicProjection.h"
#include "ProjectionLib/PolyconicProjection.h"
#include "ProjectionLib/EquidistantConicProjection.h"
#include "ProjectionLib/TransverseMercatorProjection.h"
#include "ProjectionLib/StereographicProjection.h"
#include "ProjectionLib/LambertAzimuthalProjection.h"
#include "ProjectionLib/AzimuthalEquidistantProjection.h"
#include "ProjectionLib/GnomonicProjection.h"
#include "ProjectionLib/OrthographicProjection.h"
#include "ProjectionLib/SinusoidalProjection.h"
#include "ProjectionLib/EquirectangularProjection.h"
#include "ProjectionLib/MillerCylindricalProjection.h"
#include "ProjectionLib/VanDerGrintenProjection.h"
#include "ProjectionLib/RobinsonProjection.h"
#include "ProjectionLib/AlaskaConformalProjection.h"
#include "ProjectionLib/HotineObliqueMercatorProjection.h"
#include "ProjectionLib/GeographicProjection.h"


namespace ProjFactory
{

/** define some common error messages */
#define PROJGEOTIFF_NOINFILE   "GeoTIFFProjectionFactory: no input file"
#define PROJGEOTIFF_NOMODEL    "GeoTIFFProjectionFactory: no model type"
#define PROJGEOTIFF_NODATUM    "GeoTIFFProjectionFactory: no datum"
#define PROJGEOTIFF_NOANGULAR  "GeoTIFFProjectionFactory: no angular units"
#define PROJGEOTIFF_NORASTER   "GeoTIFFProjectionFactory: raster type is wrong"
#define PROJGEOTIFF_NOPCSTYPE  "GeoTIFFProjectionFactory: no PCS type"
#define PROJGEOTIFF_NOPROJTYPE "GeoTIFFProjectionFactory: no projection type"
#define PROJGEOTIFF_NOUNITS    "GeoTIFFProjectionFactory: no linear units"
#define PROJGEOTIFF_UNKOWNPROJ "GeoTIFFProjectionFactory: unkown projection"
#define PROJGEOTIFF_NOMEM      "GeoTIFFProjectionFactory: no memory"


/**  This class creates projections from
     geotiff files. */
class GeoTIFFProjectionFactory: public ProjectionFactory
{
public:
  /** Main constructor for the class. */
  GeoTIFFProjectionFactory();

  /** Main destructor for the class. */
  virtual ~GeoTIFFProjectionFactory();


  /** Create function creates a projection
      from a file */
  virtual ProjLib::Projection * create(std::string filename)
    throw (ProjFactoryException);

protected:

  /** UTM/State plane parser for geotiff */
  ProjLib::Projection * parseUTMSPCS(USGSImageLib::GeoTIFFImageIFile * ingeo)
    throw(ProjFactoryException);

  /** Get a ProjLib datumtype from Geotiff code */
  ProjLib::DATUM getDatumFromGeo(unsigned short ingeocode) 
    throw(ProjFactoryException);
  
  /** Get a ProjLib unittype from Geotiff code */
  ProjLib::UNIT getUnitFromGeo(unsigned short ingeocode)
    throw(ProjFactoryException);
  
  /** Convert to Packed DMS from degrees */
  double ConvertToDMS(double degrees) throw();

};

}//namespace

#endif
