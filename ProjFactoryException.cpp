#ifndef PROJFACTORYEXCEPTION_CPP_
#define PROJFACTORYEXCEPTION_CPP_


#include "ProjFactoryException.h"

using namespace ProjFactory;

//**********************************************************
ProjFactoryException::ProjFactoryException()
{
  message = PROJFACTEXCEP;     //set default message
}

//**********************************************************
ProjFactoryException::ProjFactoryException(const std::string & inMessage)
{
  message = inMessage;
}

//**********************************************************
ProjFactoryException::~ProjFactoryException()
{}

//**********************************************************
std::string ProjFactoryException::getMessage() const throw()
{
  return message;
}

//*********************************************************
void ProjFactoryException::
setMessage(const std::string & inMessage) throw()
{
  message = inMessage;
}

#endif
