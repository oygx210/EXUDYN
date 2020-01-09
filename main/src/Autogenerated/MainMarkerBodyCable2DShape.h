/** ***********************************************************************************************
* @class        MainMarkerBodyCable2DShapeParameters
* @brief        Parameter class for MainMarkerBodyCable2DShape
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2019-11-12  21:47:28 (last modfied)
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

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CMarkerBodyCable2DShape.h"

#include "Autogenerated/VisuMarkerBodyCable2DShape.h"

//! AUTO: Parameters for class MainMarkerBodyCable2DShapeParameters
class MainMarkerBodyCable2DShapeParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainMarkerBodyCable2DShape
* @brief        A special Marker attached to a 2D ANCF beam finite element with cubic interpolation and 8 coordinates.
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
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

//! AUTO: MainMarkerBodyCable2DShape
class MainMarkerBodyCable2DShape: public MainMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerBodyCable2DShape* cMarkerBodyCable2DShape; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationMarkerBodyCable2DShape* visualizationMarkerBodyCable2DShape; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainMarkerBodyCable2DShape()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CMarkerBodyCable2DShape* GetCMarkerBodyCable2DShape() { return cMarkerBodyCable2DShape; }
    //! AUTO: Get const pointer to computational class
    const CMarkerBodyCable2DShape* GetCMarkerBodyCable2DShape() const { return cMarkerBodyCable2DShape; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCMarkerBodyCable2DShape(CMarkerBodyCable2DShape* pCMarkerBodyCable2DShape) { cMarkerBodyCable2DShape = pCMarkerBodyCable2DShape; }

    //! AUTO: Get pointer to visualization class
    VisualizationMarkerBodyCable2DShape* GetVisualizationMarkerBodyCable2DShape() { return visualizationMarkerBodyCable2DShape; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationMarkerBodyCable2DShape* GetVisualizationMarkerBodyCable2DShape() const { return visualizationMarkerBodyCable2DShape; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationMarkerBodyCable2DShape(VisualizationMarkerBodyCable2DShape* pVisualizationMarkerBodyCable2DShape) { visualizationMarkerBodyCable2DShape = pVisualizationMarkerBodyCable2DShape; }

    //! AUTO: Get const pointer to computational base class object
    virtual CMarker* GetCMarker() const { return cMarkerBodyCable2DShape; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCMarker(CMarker* pCMarker) { cMarkerBodyCable2DShape = (CMarkerBodyCable2DShape*)pCMarker; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationMarker* GetVisualizationMarker() const { return visualizationMarkerBodyCable2DShape; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationMarker(VisualizationMarker* pVisualizationMarker) { visualizationMarkerBodyCable2DShape = (VisualizationMarkerBodyCable2DShape*)pVisualizationMarker; }

    //! AUTO:  Get type name of marker (without keyword "Marker"...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "Cable2DShape";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cMarkerBodyCable2DShape->GetParameters().bodyNumber = py::cast<Index>(d["bodyNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cMarkerBodyCable2DShape->GetParameters().numberOfSegments = py::cast<Index>(d["numberOfSegments"]); /* AUTO:  read out dictionary and cast to C++ type*/
        HPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (HPyUtils::DictItemExists(d, "Vshow")) { visualizationMarkerBodyCable2DShape->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["markerType"] = (std::string)GetTypeName();
        d["bodyNumber"] = (Index)cMarkerBodyCable2DShape->GetParameters().bodyNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["numberOfSegments"] = (Index)cMarkerBodyCable2DShape->GetParameters().numberOfSegments; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationMarkerBodyCable2DShape->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { return py::cast((Index)cMarkerBodyCable2DShape->GetParameters().bodyNumber);} //! AUTO: get parameter
        else if (parameterName.compare("numberOfSegments") == 0) { return py::cast((Index)cMarkerBodyCable2DShape->GetParameters().numberOfSegments);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationMarkerBodyCable2DShape->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("MarkerBodyCable2DShape::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { HPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { cMarkerBodyCable2DShape->GetParameters().bodyNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("numberOfSegments") == 0) { cMarkerBodyCable2DShape->GetParameters().numberOfSegments = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationMarkerBodyCable2DShape->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("MarkerBodyCable2DShape::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


