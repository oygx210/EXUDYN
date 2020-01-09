/** ***********************************************************************************************
* @class        CLoad
* @brief        A general class to handle loads which are applied to markers
*
* @author       Gerstmayr Johannes
* @date         2018-05-18 (generated)
* @date         2019-05-02 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

class CLoad //
{
protected: //
  //Index markerNumber;                             //!< pointer to CMarker
  //moved to CLoadForceVector.parameters! Vector loadVector;                              //!< general load vector (e.g. force or torque)

public: //
  //! default constructor with parameter initialization
  //CLoad()
  //{
  //  markerNumber = 0;
  //  //loadVector = Vector({0.,0.,0.});
  //};

  // access functions
  //! clone object; specifically for copying instances of derived class, for automatic memory management e.g. in ObjectContainer
  virtual CLoad* GetClone() const { return new CLoad(*this); }
  
  //! Write (Reference) access to: marker index (defined in specialized class)
  virtual void SetMarkerNumber(Index markerNumberInit) { release_assert(0 && "ERROR: illegal call to CLoad::SetMarkerNumber(...)"); }
  //! Read (Reference) access to: marker index (defined in specialized class)
  virtual Index GetMarkerNumber() const { release_assert(0 && "ERROR: illegal call to CLoad::GetMarkerNumber() const"); return 0; }

  //! determine type of load in order to decide according action in assembly
  virtual LoadType GetType() const { release_assert(0 && "ERROR: illegal call to CLoad::GetType"); return LoadType::None; }

  const static Index VectorSize = 3; //dimensionality of vector loads
  //! determine type of load in order to decide according action in assembly
  virtual bool IsVector() const { release_assert(0 && "ERROR: illegal call to CLoad::IsVector"); return true; }

  //! Write (Reference) access to: general load vector (e.g. force or torque); used if LoadType::IsVector = 1
  //DELETE: should not be needed: virtual Vector3D& GetLoadVector() { release_assert(0 && "ERROR: illegal call to CLoad::GetLoadVector"); Vector3D* v = new Vector3D(0.); return *v; }
  //! Read (Reference) access to: general load vector (e.g. force or torque) as a function of time; used if LoadType::IsVector = 1
  virtual Vector3D GetLoadVector(Real t) const { release_assert(0 && "ERROR: illegal call to CLoad::GetLoadVector(Real t) const"); Vector3D* v = new Vector3D(0.); return *v;
  }

  //! Write (Reference) access to: scalar load value (e.g. object/node coordinate); used if LoadType::IsVector = 0
  //DELETE: should not be needed: virtual Real& GetLoadValue() { release_assert(0 && "ERROR: illegal call to CLoad::GetLoadValue"); Real* v = new Real(0.); return *v; }
  //! Read (Reference) access to: scalar load value (e.g. object/node coordinate) as a function of time; used if LoadType::IsVector = 0
  virtual Real GetLoadValue(Real t) const { release_assert(0 && "ERROR: illegal call to CLoad::GetLoadValue(Real t) const");  Real* v = new Real(0.); return *v; }

  virtual Marker::Type GetRequestedMarkerType() const { release_assert(0 && "ERROR: illegal call to CLoad::RequestedMarkerType"); return Marker::None; }

  virtual void Print(std::ostream& os) const
  {
	  os << "CLoad";
	  os << "  markerNumber = " << GetMarkerNumber() << "\n";
	  //os << "  loadVector = " << loadVector << "\n";
	  os << "\n";
  }

  friend std::ostream& operator<<(std::ostream& os, const CLoad& object)
  {
	  object.Print(os);
	  return os;
  }
};

