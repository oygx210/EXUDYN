/** ***********************************************************************************************
* @class        MainMarkerNodeRotationCoordinateParameters
* @brief        Parameter class for MainMarkerNodeRotationCoordinate
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-06-01  20:10:12 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CMarkerNodeRotationCoordinate.h"

#include "Autogenerated/VisuMarkerNodeRotationCoordinate.h"

//! AUTO: Parameters for class MainMarkerNodeRotationCoordinateParameters
class MainMarkerNodeRotationCoordinateParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainMarkerNodeRotationCoordinate
* @brief        A node-Marker attached to a a node containing rotation; the Marker measures a rotation coordinate (Tait-Bryan angles) or angular velocities on the velocity level
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

//! AUTO: MainMarkerNodeRotationCoordinate
class MainMarkerNodeRotationCoordinate: public MainMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerNodeRotationCoordinate* cMarkerNodeRotationCoordinate; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationMarkerNodeRotationCoordinate* visualizationMarkerNodeRotationCoordinate; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainMarkerNodeRotationCoordinate()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CMarkerNodeRotationCoordinate* GetCMarkerNodeRotationCoordinate() { return cMarkerNodeRotationCoordinate; }
    //! AUTO: Get const pointer to computational class
    const CMarkerNodeRotationCoordinate* GetCMarkerNodeRotationCoordinate() const { return cMarkerNodeRotationCoordinate; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCMarkerNodeRotationCoordinate(CMarkerNodeRotationCoordinate* pCMarkerNodeRotationCoordinate) { cMarkerNodeRotationCoordinate = pCMarkerNodeRotationCoordinate; }

    //! AUTO: Get pointer to visualization class
    VisualizationMarkerNodeRotationCoordinate* GetVisualizationMarkerNodeRotationCoordinate() { return visualizationMarkerNodeRotationCoordinate; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationMarkerNodeRotationCoordinate* GetVisualizationMarkerNodeRotationCoordinate() const { return visualizationMarkerNodeRotationCoordinate; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationMarkerNodeRotationCoordinate(VisualizationMarkerNodeRotationCoordinate* pVisualizationMarkerNodeRotationCoordinate) { visualizationMarkerNodeRotationCoordinate = pVisualizationMarkerNodeRotationCoordinate; }

    //! AUTO: Get const pointer to computational base class object
    virtual CMarker* GetCMarker() const { return cMarkerNodeRotationCoordinate; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCMarker(CMarker* pCMarker) { cMarkerNodeRotationCoordinate = (CMarkerNodeRotationCoordinate*)pCMarker; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationMarker* GetVisualizationMarker() const { return visualizationMarkerNodeRotationCoordinate; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationMarker(VisualizationMarker* pVisualizationMarker) { visualizationMarkerNodeRotationCoordinate = (VisualizationMarkerNodeRotationCoordinate*)pVisualizationMarker; }

    //! AUTO:  Get type name of marker (without keyword 'Marker'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "NodeRotationCoordinate";
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cMarkerNodeRotationCoordinate->GetParameters().nodeNumber = py::cast<Index>(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cMarkerNodeRotationCoordinate->GetParameters().rotationCoordinate = py::cast<Index>(d["rotationCoordinate"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationMarkerNodeRotationCoordinate->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["markerType"] = (std::string)GetTypeName();
        d["nodeNumber"] = (Index)cMarkerNodeRotationCoordinate->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["rotationCoordinate"] = (Index)cMarkerNodeRotationCoordinate->GetParameters().rotationCoordinate; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationMarkerNodeRotationCoordinate->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((Index)cMarkerNodeRotationCoordinate->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("rotationCoordinate") == 0) { return py::cast((Index)cMarkerNodeRotationCoordinate->GetParameters().rotationCoordinate);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationMarkerNodeRotationCoordinate->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("MarkerNodeRotationCoordinate::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cMarkerNodeRotationCoordinate->GetParameters().nodeNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("rotationCoordinate") == 0) { cMarkerNodeRotationCoordinate->GetParameters().rotationCoordinate = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationMarkerNodeRotationCoordinate->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("MarkerNodeRotationCoordinate::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


