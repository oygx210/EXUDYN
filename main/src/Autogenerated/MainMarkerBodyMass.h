/** ***********************************************************************************************
* @class        MainMarkerBodyMassParameters
* @brief        Parameter class for MainMarkerBodyMass
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-07-20  12:33:23 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINMARKERBODYMASSPARAMETERS__H
#define MAINMARKERBODYMASSPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CMarkerBodyMass.h"

#include "Autogenerated/VisuMarkerBodyMass.h"

//! AUTO: Parameters for class MainMarkerBodyMassParameters
class MainMarkerBodyMassParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainMarkerBodyMass
* @brief        A marker attached to the body mass; use this marker to apply a body-load (e.g. gravitational force).
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

//! AUTO: MainMarkerBodyMass
class MainMarkerBodyMass: public MainMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerBodyMass* cMarkerBodyMass; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationMarkerBodyMass* visualizationMarkerBodyMass; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainMarkerBodyMass()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CMarkerBodyMass* GetCMarkerBodyMass() { return cMarkerBodyMass; }
    //! AUTO: Get const pointer to computational class
    const CMarkerBodyMass* GetCMarkerBodyMass() const { return cMarkerBodyMass; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCMarkerBodyMass(CMarkerBodyMass* pCMarkerBodyMass) { cMarkerBodyMass = pCMarkerBodyMass; }

    //! AUTO: Get pointer to visualization class
    VisualizationMarkerBodyMass* GetVisualizationMarkerBodyMass() { return visualizationMarkerBodyMass; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationMarkerBodyMass* GetVisualizationMarkerBodyMass() const { return visualizationMarkerBodyMass; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationMarkerBodyMass(VisualizationMarkerBodyMass* pVisualizationMarkerBodyMass) { visualizationMarkerBodyMass = pVisualizationMarkerBodyMass; }

    //! AUTO: Get const pointer to computational base class object
    virtual CMarker* GetCMarker() const { return cMarkerBodyMass; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCMarker(CMarker* pCMarker) { cMarkerBodyMass = (CMarkerBodyMass*)pCMarker; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationMarker* GetVisualizationMarker() const { return visualizationMarkerBodyMass; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationMarker(VisualizationMarker* pVisualizationMarker) { visualizationMarkerBodyMass = (VisualizationMarkerBodyMass*)pVisualizationMarker; }

    //! AUTO:  Get type name of marker (without keyword 'Marker'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "BodyMass";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cMarkerBodyMass->GetParameters().bodyNumber = py::cast<Index>(d["bodyNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationMarkerBodyMass->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["markerType"] = (std::string)GetTypeName();
        d["bodyNumber"] = (Index)cMarkerBodyMass->GetParameters().bodyNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationMarkerBodyMass->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { return py::cast((Index)cMarkerBodyMass->GetParameters().bodyNumber);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationMarkerBodyMass->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("MarkerBodyMass::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { cMarkerBodyMass->GetParameters().bodyNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationMarkerBodyMass->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("MarkerBodyMass::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
