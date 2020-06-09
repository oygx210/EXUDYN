/** ***********************************************************************************************
* @class        MainObjectJointSphericalParameters
* @brief        Parameter class for MainObjectJointSpherical
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
#include "Autogenerated/CObjectJointSpherical.h"

#include "Autogenerated/VisuObjectJointSpherical.h"

//! AUTO: Parameters for class MainObjectJointSphericalParameters
class MainObjectJointSphericalParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectJointSpherical
* @brief        A spherical joint, which constrains the relative translation between two position based markers.
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

//! AUTO: MainObjectJointSpherical
class MainObjectJointSpherical: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectJointSpherical* cObjectJointSpherical; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectJointSpherical* visualizationObjectJointSpherical; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectJointSpherical()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectJointSpherical* GetCObjectJointSpherical() { return cObjectJointSpherical; }
    //! AUTO: Get const pointer to computational class
    const CObjectJointSpherical* GetCObjectJointSpherical() const { return cObjectJointSpherical; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectJointSpherical(CObjectJointSpherical* pCObjectJointSpherical) { cObjectJointSpherical = pCObjectJointSpherical; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectJointSpherical* GetVisualizationObjectJointSpherical() { return visualizationObjectJointSpherical; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectJointSpherical* GetVisualizationObjectJointSpherical() const { return visualizationObjectJointSpherical; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectJointSpherical(VisualizationObjectJointSpherical* pVisualizationObjectJointSpherical) { visualizationObjectJointSpherical = pVisualizationObjectJointSpherical; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectJointSpherical; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectJointSpherical = (CObjectJointSpherical*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectJointSpherical; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectJointSpherical = (VisualizationObjectJointSpherical*)pVisualizationObject; }

    //! AUTO:  Get type name of object (without keyword 'Object'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "JointSpherical";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectJointSpherical->GetParameters().markerNumbers = py::cast<std::vector<Index>>(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "constrainedAxes")) { cObjectJointSpherical->GetParameters().constrainedAxes = py::cast<std::vector<Index>>(d["constrainedAxes"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectJointSpherical->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectJointSpherical->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VjointRadius")) { visualizationObjectJointSpherical->GetJointRadius() = py::cast<float>(d["VjointRadius"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectJointSpherical->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = (std::vector<Index>)cObjectJointSpherical->GetParameters().markerNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["constrainedAxes"] = (std::vector<Index>)cObjectJointSpherical->GetParameters().constrainedAxes; //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectJointSpherical->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectJointSpherical->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VjointRadius"] = (float)visualizationObjectJointSpherical->GetJointRadius(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectJointSpherical->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast((std::vector<Index>)cObjectJointSpherical->GetParameters().markerNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("constrainedAxes") == 0) { return py::cast((std::vector<Index>)cObjectJointSpherical->GetParameters().constrainedAxes);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectJointSpherical->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectJointSpherical->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VjointRadius") == 0) { return py::cast((float)visualizationObjectJointSpherical->GetJointRadius());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectJointSpherical->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointSpherical::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectJointSpherical->GetParameters().markerNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("constrainedAxes") == 0) { cObjectJointSpherical->GetParameters().constrainedAxes = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectJointSpherical->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectJointSpherical->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VjointRadius") == 0) { visualizationObjectJointSpherical->GetJointRadius() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectJointSpherical->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectJointSpherical::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};


