#ifndef GEOTIFFPROJECTIONFACTORY_CPP_
#define GEOTIFFPROJECTIONFACTORY_CPP_

#include "GeoTIFFProjectionFactory.h"


using namespace ProjFactory;

//**********************************************
GeoTIFFProjectionFactory::GeoTIFFProjectionFactory()
  : ProjectionFactory()
{
}

//************************************************
GeoTIFFProjectionFactory::~GeoTIFFProjectionFactory()
{
}


/** Create function creates a projection
    from a file */
ProjLib::Projection * GeoTIFFProjectionFactory::
create(std::string filename)
  throw (ProjFactoryException)
{
  unsigned short int modeltype, rastertype, projCStype,
    projtype, linunits, idatum;
  ProjLib::DATUM datum;
  ProjLib::UNIT units;
  USGSImageLib::GeoTIFFImageIFile * ingeo = NULL;
 
  //input parameters
  double StdParallel1;
  double StdParallel2;
  double NatOriginLong;
  double NatOriginLat;
  double FalseEasting;
  double FalseNorthing;
  double FalseOriginLong;
  double FalseOriginLat;
  double FalseOriginEasting;
  double FalseOriginNorthing;
  double CenterLong;
  double CenterLat;
  double CenterEasting;
  double CenterNorthing;
  double ScaleAtNatOrigin;
  double AzimuthAngle;
  double StraightVertPoleLong;
  ProjLib::Projection * proj = NULL;
  
  try
    {
      if (!(ingeo = new (std::nothrow) USGSImageLib::
            GeoTIFFImageIFile(filename, false)))
        throw std::bad_alloc();

      if (!ingeo->good())
        throw ProjFactoryException(PROJGEOTIFF_NOINFILE);

      if (!ingeo->getModelType(modeltype))
        throw ProjFactoryException(PROJGEOTIFF_NOMODEL);
      
      
      
      if (modeltype != ModelTypeProjected) //try to treat as geo
      {
        if (!ingeo->getGeoKey(GeographicTypeGeoKey,idatum))
          throw ProjFactoryException(PROJGEOTIFF_NODATUM);
        
        datum = getDatumFromGeo(idatum);
        
        if (!ingeo->getAngularUnits(linunits))
          throw ProjFactoryException(PROJGEOTIFF_NOANGULAR);
             
        units = getUnitFromGeo(linunits);
        
        if(!(proj = new ProjLib::GeographicProjection(datum, units)))
          throw std::bad_alloc();
        
        projlist.push_back(proj);     //put it 9in the list
        return proj;
      }
      
      if (!ingeo->getRasterType(rastertype))
        throw ProjFactoryException(PROJGEOTIFF_NORASTER);
      
      if (rastertype != RasterPixelIsArea) //must have raster as area
        throw ProjFactoryException(PROJGEOTIFF_NORASTER);
      
      
      if (!ingeo->getProjectedCSType(projCStype))
        throw ProjFactoryException(PROJGEOTIFF_NOPCSTYPE);
      
       if (projCStype == 32767)
       {
         if (!ingeo->getGeoKey(ProjCoordTransGeoKey, projtype))
           throw ProjFactoryException(PROJGEOTIFF_NOPROJTYPE);
         
         if (!ingeo->getGeoKey(GeographicTypeGeoKey,idatum))
           throw ProjFactoryException(PROJGEOTIFF_NODATUM);
         
         datum = getDatumFromGeo(idatum);
         
         if (!ingeo->getLinearUnits(linunits))
           throw ProjFactoryException(PROJGEOTIFF_NOUNITS);
         
         units = getUnitFromGeo(linunits);
         
         //get all the possible params
         ingeo->getGeoKey(ProjStdParallel1GeoKey, StdParallel1);
         ingeo->getGeoKey(ProjStdParallel2GeoKey, StdParallel2);
         ingeo->getGeoKey(ProjNatOriginLongGeoKey,NatOriginLong);
         ingeo->getGeoKey(ProjNatOriginLatGeoKey, NatOriginLat);
         ingeo->getGeoKey(ProjFalseNorthingGeoKey,FalseNorthing);
         ingeo->getGeoKey(ProjFalseEastingGeoKey, FalseEasting);
         ingeo->getGeoKey(ProjFalseOriginLongGeoKey, 
                          FalseOriginLong);
         ingeo->getGeoKey(ProjFalseOriginLatGeoKey,
                          FalseOriginLat);
         ingeo->getGeoKey(ProjFalseOriginEastingGeoKey, 
                          FalseOriginEasting);
         ingeo->getGeoKey(ProjFalseOriginNorthingGeoKey, 
                          FalseOriginNorthing);
         ingeo->getGeoKey(ProjCenterLongGeoKey, CenterLong);
         ingeo->getGeoKey(ProjCenterLatGeoKey, CenterLat);
         ingeo->getGeoKey(ProjCenterEastingGeoKey, 
                          CenterEasting);
         ingeo->getGeoKey(ProjCenterNorthingGeoKey, 
                          CenterNorthing);
         ingeo->getGeoKey(ProjScaleAtNatOriginGeoKey, 
                          ScaleAtNatOrigin);
         ingeo->getGeoKey(ProjAzimuthAngleGeoKey, AzimuthAngle);
         ingeo->getGeoKey(ProjStraightVertPoleLongGeoKey, 
                          StraightVertPoleLong);
         
         switch(projtype)
         {
         case CT_AlaskaConformal:
           if(!(proj =  new(std::nothrow) ProjLib::AlaskaConformalProjection
                (0.0, 0.0, FalseEasting, FalseNorthing,
                 datum, units )))
             throw std::bad_alloc();
           break;
         case CT_AlbersEqualArea:
           if(!(proj =  new (std::nothrow) ProjLib::AlbersConicProjection
                ( ConvertToDMS(StdParallel1), 
                  ConvertToDMS(StdParallel2),
                  0.0, 0.0, ConvertToDMS(CenterLong),
                  ConvertToDMS(NatOriginLat), 
                  FalseEasting, FalseNorthing, datum, units)))
             throw std::bad_alloc();
           break;
           
         case CT_AzimuthalEquidistant:
           if(!(proj = new (std::nothrow) ProjLib::
                AzimuthalEquidistantProjection
                ( ConvertToDMS(CenterLong),
                  ConvertToDMS(CenterLat),
                  FalseEasting, FalseNorthing, 0.0, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_EquidistantConic:
           if ( StdParallel1 == StdParallel2 )
           {
             if(!(proj =  new(std::nothrow) ProjLib::
                  EquidistantConicProjection
                  ( ConvertToDMS(CenterLat), 0.0, 0.0,
                    ConvertToDMS(CenterLong),
                    ConvertToDMS(NatOriginLat),
                    FalseEasting,
                    FalseNorthing,
                    datum, units)))
               throw std::bad_alloc();
             
           }
           else
           {      
             if(!(proj = new(std::nothrow) ProjLib::
                  EquidistantConicProjection
                  ( ConvertToDMS(StdParallel1),
                    ConvertToDMS(StdParallel2),
                    0.0, 0.0,
                    ConvertToDMS(CenterLong),
                    ConvertToDMS(NatOriginLat),
                    FalseEasting,
                    FalseNorthing,
                    datum, units)))
               throw std::bad_alloc();
           }
           break;
         case CT_Equirectangular:
           if(!(proj =  new(std::nothrow) ProjLib::
                EquirectangularProjection
                ( ConvertToDMS(CenterLat), 0.0, 
                  ConvertToDMS(CenterLong),
                  FalseEasting,FalseNorthing, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_Gnomonic:
           if(!(proj = new(std::nothrow) ProjLib::
                GnomonicProjection
                ( ConvertToDMS(CenterLong),
                  ConvertToDMS(CenterLat),
                  FalseEasting,FalseNorthing, 
                  0.0, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_ObliqueMercator:
           if(!(proj = new (std::nothrow) ProjLib::
                HotineObliqueMercatorProjection
                ( ScaleAtNatOrigin, AzimuthAngle,
                  0.0, 0.0, ConvertToDMS(CenterLong),
                  ConvertToDMS(CenterLat), FalseEasting,
                  FalseNorthing, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_LambertAzimEqualArea:
           if(!(proj = new(std::nothrow) ProjLib::
                LambertAzimuthalProjection
                ( ConvertToDMS(CenterLong), ConvertToDMS(CenterLat),
                  FalseEasting, FalseNorthing, 0.0, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_LambertConfConic_1SP:
         case CT_LambertConfConic_2SP:
           if(!(proj = new(std::nothrow) ProjLib::
                LambertConformalConicProjection
                ( ConvertToDMS(StdParallel1), ConvertToDMS(StdParallel2),
                  0.0, 0.0, ConvertToDMS(NatOriginLong),
                  ConvertToDMS(FalseOriginLat),
                  FalseEasting,FalseNorthing, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_Mercator:
           if(!(proj = new(std::nothrow) ProjLib::
                MercatorProjection
                ( 0.0, 0.0, ConvertToDMS(NatOriginLong),
                  ConvertToDMS(NatOriginLat),
                  CenterEasting, CenterNorthing, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_MillerCylindrical:
           if(!(proj = new (std::nothrow) ProjLib::
                MillerCylindricalProjection
                ( 0.0, ConvertToDMS(CenterLong),
                  FalseEasting, FalseNorthing,
                  datum, units)))
             throw std::bad_alloc();
           break;
         case CT_Orthographic:
           if(!(proj = new(std::nothrow) ProjLib::
                OrthographicProjection
                ( ConvertToDMS(CenterLong),
                  ConvertToDMS(CenterLat),
                  FalseEasting,FalseNorthing, 0.0, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_Polyconic:
           if(!(proj = new(std::nothrow) ProjLib::
                PolyconicProjection
                ( 0.0, 0.0, ConvertToDMS(CenterLong),
                  ConvertToDMS(CenterLat), FalseEasting,
                  FalseNorthing, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_PolarStereographic:
           if(!(proj = new(std::nothrow) ProjLib::
                PolarStereographicProjection
                (ConvertToDMS(StraightVertPoleLong),
                 ConvertToDMS(NatOriginLat), 0.0, 0.0,
                 FalseEasting,FalseNorthing, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_Robinson:
           if(!(proj = new(std::nothrow) ProjLib::
                RobinsonProjection
                ( 0.0, ConvertToDMS(CenterLong),
                  FalseEasting, FalseNorthing, datum, units)))
             throw std::bad_alloc();
           break; 
         case CT_Sinusoidal:
           if(!(proj = new(std::nothrow) ProjLib::
                SinusoidalProjection
                ( 0.0, ConvertToDMS(CenterLong),
                  FalseEasting,
                  FalseNorthing, datum, units)))
                 throw std::bad_alloc();
           break;
         case CT_Stereographic:
           if(!(proj = new(std::nothrow) ProjLib::
                StereographicProjection
                (ConvertToDMS(CenterLong),
                 ConvertToDMS(CenterLat),
                 FalseEasting, FalseNorthing,
                 0.0, datum, units)))
                 throw std::bad_alloc();
           break;
         case CT_TransverseMercator:
           if(!(proj = new(std::nothrow) ProjLib::
                TransverseMercatorProjection
                (ScaleAtNatOrigin, 0.0, 0.0,
                 ConvertToDMS(CenterLong),
                 ConvertToDMS(NatOriginLat), FalseEasting,
                 FalseNorthing, datum, units)))
             throw std::bad_alloc();
           break;
         case CT_VanDerGrinten:
           if(!(proj = new(std::nothrow) ProjLib::
                VanDerGrintenProjection
                ( ConvertToDMS(CenterLat), 0.0, 
                  ConvertToDMS(CenterLong),
                  FalseEasting,FalseNorthing,
                  datum, units)))
                 throw std::bad_alloc();
           break;
         default:
           throw ProjFactoryException(PROJGEOTIFF_UNKOWNPROJ); 
           
         }
       }
       else
       {
         proj = parseUTMSPCS(ingeo);
       }
       
       projlist.push_back(proj);
       return proj;
       
    }
  catch(ProjFactoryException & temp )
  {
    if (proj)
      delete proj;

    throw temp;
  }
  catch(std::bad_alloc & badal)
  {
    if (proj)
      delete proj;

    throw ProjFactoryException(PROJGEOTIFF_NOMEM);
  }
  catch(...)
  {
    if (proj)
      delete proj;

    throw ProjFactoryException();
  }
}


//******************************************************************
ProjLib::Projection * GeoTIFFProjectionFactory::
parseUTMSPCS(USGSImageLib::GeoTIFFImageIFile * ingeo)
  throw(ProjFactoryException)
{
  USGSImageLib::SPCSMapper stateplanemap;
  ProjLib::Projection * proj = NULL;
  ProjLib::PROJSYS projSys = ProjLib::UNKNOWN_PROJSYS;
  ProjLib::DATUM   datum = ProjLib::UNKNOWN_DATUM;
  ProjLib::UNIT    u = ProjLib::METERS;
  short unsigned int   zone = 0;
  short unsigned int pcsCode;
  
  try
  {
    if (!ingeo->getProjectedCSType(pcsCode))
      throw ProjFactoryException(PROJGEOTIFF_NOPCSTYPE);
    
    if ( pcsCode < 20137 )
      throw ProjFactoryException(PROJGEOTIFF_UNKOWNPROJ); 
    
    
    // Adindan UTM
    if ( pcsCode >= 20137 && pcsCode <= 20138 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::ADINDAN;
      zone = pcsCode % 100;
    }
    // European Datum 1950 UTM
    else if ( pcsCode >= 23028 && pcsCode <=23038 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::EUROPEAN_DATUM_1950;
      zone = pcsCode % 100;
    }
    // Indian 1954/1978 UTM
    else if ( pcsCode >= 23947 && pcsCode <= 24048 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::INDIAN;
      zone = pcsCode % 100;
    }
    // Provisional South American 1956 UTM North
    else if ( pcsCode >= 24818 && pcsCode <= 24821 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::PROVISIONAL_S_AMERICAN_1956;
      zone = pcsCode % 100;
    }
    // Provisional South American 1956 UTM South
    else if ( pcsCode >= 24818 && pcsCode <= 24821 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::PROVISIONAL_S_AMERICAN_1956;
      zone = 60 - ( pcsCode % 100 );
    }
    // NAD27 UTM North
    else if ( pcsCode >= 26703 && pcsCode <= 26722 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::NAD27;
      zone = pcsCode % 100;
    }
    // NAD27 State Plane
    else if ( pcsCode >= 26729 && pcsCode <= 26798 )
    {
      stateplanemap.initNAD27Mappings();
      if (stateplanemap.getUSGSCode( pcsCode, zone ) )
      {
        projSys = ProjLib::SPCS;
        datum = ProjLib::NAD27;
        u = ProjLib::US_FEET;
      }
    }
    // NAD83 UTM North
    else if ( pcsCode >= 26903 && pcsCode <= 26923 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::NAD83;
      zone = pcsCode % 100;
    }
    // NAD83 State Plane
    else if ( pcsCode >= 26929 && pcsCode <= 26998 )
    {
      stateplanemap.initNAD83Mappings(); //init the mappings
      if (stateplanemap.getUSGSCode( pcsCode, zone ) )
      {
        projSys = ProjLib::SPCS;
        datum = ProjLib::NAD83;
        u = ProjLib::US_FEET;
      }
    }
      // NAD27 State Plane
    else if ( pcsCode >= 32001 && pcsCode <= 32060 )
    {
      stateplanemap.initNAD27Mappings();
      if (stateplanemap.getUSGSCode( pcsCode, zone ) )
      {
        projSys = ProjLib::SPCS;
        datum = ProjLib::NAD27;
        u = ProjLib::US_FEET;
      }
    }
      // NAD83 State Plane
    else if ( pcsCode >= 32100 && pcsCode <= 32161 )
    {
      stateplanemap.initNAD83Mappings();
      if (stateplanemap.getUSGSCode( pcsCode, zone ) )
      {
        projSys = ProjLib::SPCS;
        datum = ProjLib::NAD83;
        u = ProjLib::US_FEET;
      }
    }
    // WGS72 UTM North
    else if ( pcsCode >= 32201 && pcsCode <= 32260 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::WGS_72;
      zone = pcsCode % 100;
    }
    // WGS72 UTM South
    else if ( pcsCode >= 32301 && pcsCode <= 32360 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::WGS_72;
      zone = -( pcsCode % 100 );
    }
    // WGS72BE UTM North
    else if ( pcsCode >= 32401 && pcsCode <= 32460 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::WGS_72;
      zone = pcsCode % 100;
    }
    // WGS72BE UTM South
    else if ( pcsCode >= 32501 && pcsCode <= 32560 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::WGS_72;
      zone = -( pcsCode % 100 );
    }
    // WGS84 UTM North
    else if ( pcsCode >= 32601 && pcsCode <= 32660 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::WGS_84;
      zone = pcsCode % 100;
    }
    // WGS84 UTM South
    else if ( pcsCode >= 32701 && pcsCode <= 32760 )
    {
      projSys = ProjLib::UTM;
      datum = ProjLib::WGS_84;
      zone = -( pcsCode % 100 );
    }
    
    // Create the projection if a valid projection system was found
    if ( ProjLib::UNKNOWN_PROJSYS != projSys )
    {
      switch ( projSys )
      {
      case ProjLib::UTM:
        if (!(proj = new ProjLib::UTMProjection( zone, datum, u) ))
          throw std::bad_alloc();
        break;
      case ProjLib::SPCS:
        if (!(proj = new ProjLib::StatePlaneProjection( zone, datum, u)))
          throw std::bad_alloc();
        break;
      }
      
    }
    else
      throw ProjFactoryException(PROJGEOTIFF_UNKOWNPROJ);
    
    return proj;
  }
  catch(ProjFactoryException & tempexc)
  {
    if (proj)
      delete proj;

    throw tempexc;               //just rethrow
  }
  catch(...)
  {
    if (proj)
      delete proj;
    
    throw ProjFactoryException();
  }
}

//*************************************************
ProjLib::DATUM GeoTIFFProjectionFactory::
getDatumFromGeo(unsigned short ingeocode) 
  throw(ProjFactoryException)
{
  
  switch ( ingeocode )
  {
    case GCS_Adindan:
      return ProjLib::ADINDAN; 
    case GCS_Arc_1950: 
      return ProjLib::ARC1950;
    case GCS_Arc_1960:
      return ProjLib::ARC1960;
    case GCS_Cape:
      return ProjLib::CAPE;
    case GCS_ED50:
      return ProjLib::EUROPEAN_DATUM_1950;
    case GCS_GD49:
      return ProjLib::GEODETIC_DATUM_1949;
    case GCS_Hu_Tzu_Shan:
      return ProjLib::HU_TZU_SHAN;
    case GCS_Indian_1975:
      return ProjLib::INDIAN;
    case GCS_NAD27:
      return ProjLib::NAD27;
    case GCS_NAD83:
      return ProjLib::NAD83;
    case GCS_OSGB_1936:
      return ProjLib::ORDNANCE_SURVEY_1936;
    case GCS_Pulkovo_1942:
      return ProjLib::PULKOVO_1942;
    case GCS_PSAD56:
      return ProjLib::PROVISIONAL_S_AMERICAN_1956;
    case GCS_Tokyo:
      return ProjLib::TOKYO;
    case GCS_WGS_72:
      return ProjLib::WGS_72;
    case GCS_WGS_84:
      return ProjLib::WGS_84;
    default:
      throw ProjFactoryException(PROJGEOTIFF_NODATUM);
    
    }
}

//*******************************************************
ProjLib::UNIT GeoTIFFProjectionFactory::
getUnitFromGeo(unsigned short ingeocode)
  throw(ProjFactoryException)
{
  switch ( ingeocode )
  {
  case Linear_Meter:
    return ProjLib::METERS;
  case Angular_Degree:
    return ProjLib::ARC_DEGREES;
  case Linear_Foot:
  case Linear_Foot_Modified_American:
  case Linear_Foot_Clarke:
  case Linear_Foot_Indian:
    return ProjLib::US_FEET;
  default:
    throw ProjFactoryException(PROJGEOTIFF_NOUNITS);
  }
}


//************************************************************
double GeoTIFFProjectionFactory::ConvertToDMS(double degrees) throw()
{
  double temp;
  int deg, min;
  
  int sign = 1;
  
  temp = degrees;
  if (degrees < 0.0)
  {
    sign = -1;
    temp = -temp;
  }

  //get the degrees
  deg = static_cast<int>(temp);
  temp -= deg;
  temp *= 60.0; //get minutes
  min = static_cast<int>(temp);
  temp -= min; //get seconds
  temp *= 60;
  
  temp = deg * 1000000 + min * 1000 + temp;
  return temp*sign;
}

#endif

