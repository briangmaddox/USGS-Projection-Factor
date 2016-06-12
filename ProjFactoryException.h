#ifndef PROJFACTORYEXCEPTION_H_
#define PROJFACTORYEXCEPTION_H_

#include <string>



namespace ProjFactory
{

/** Define a default error message */
#define PROJFACTEXCEP   "ProjFactory: Unkown error"

class ProjFactoryException
{
public:

  /** Main constructor for the class */
  ProjFactoryException();

  /** Secondary constructor for the class takes a argument */
  ProjFactoryException(const std::string & inMessage);

  /** Class destructor */
  virtual ~ProjFactoryException();

  /** Allows one to view the message */
  std::string getMessage() const throw();

  /** Allows one to set the message */
  void setMessage(const std::string & inMessage) throw();
  
protected:

  std::string message;       //the error message
};

}//namespace

#endif




