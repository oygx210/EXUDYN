/** ***********************************************************************************************
* @class        MainMarkerSuperElementRigidParameters
* @brief        Parameter class for MainMarkerSuperElementRigid
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
#include "Autogenerated/CMarkerSuperElementRigid.h"

#include "Autogenerated/VisuMarkerSuperElementRigid.h"

//! AUTO: Parameters for class MainMarkerSuperElementRigidParameters
class MainMarkerSuperElementRigidParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainMarkerSuperElementRigid
* @brief        A position and orientation (rigid-body) marker attached to a SuperElement, such as ObjectFFRF, ObjectGenericODE2 and ObjectFFRFreducedOrder (for which it may be inefficient). The marker acts on the mesh nodes, not on the underlying nodes of the object. Note that in contrast to the MarkerSuperElementPosition, this marker needs a set of interface nodes which are not aligned at one line, such that the can represent rigid body motion. Note that definitions of marker positions are slightly different from MarkerSuperElementPosition.
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

//! AUTO: MainMarkerSuperElementRigid
class MainMarkerSuperElementRigid: public MainMarker // AUTO: 
{
protected: // AUTO: 
    CMarkerSuperElementRigid* cMarkerSuperElementRigid; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationMarkerSuperElementRigid* visualizationMarkerSuperElementRigid; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainMarkerSuperElementRigid()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CMarkerSuperElementRigid* GetCMarkerSuperElementRigid() { return cMarkerSuperElementRigid; }
    //! AUTO: Get const pointer to computational class
    const CMarkerSuperElementRigid* GetCMarkerSuperElementRigid() const { return cMarkerSuperElementRigid; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCMarkerSuperElementRigid(CMarkerSuperElementRigid* pCMarkerSuperElementRigid) { cMarkerSuperElementRigid = pCMarkerSuperElementRigid; }

    //! AUTO: Get pointer to visualization class
    VisualizationMarkerSuperElementRigid* GetVisualizationMarkerSuperElementRigid() { return visualizationMarkerSuperElementRigid; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationMarkerSuperElementRigid* GetVisualizationMarkerSuperElementRigid() const { return visualizationMarkerSuperElementRigid; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationMarkerSuperElementRigid(VisualizationMarkerSuperElementRigid* pVisualizationMarkerSuperElementRigid) { visualizationMarkerSuperElementRigid = pVisualizationMarkerSuperElementRigid; }

    //! AUTO: Get const pointer to computational base class object
    virtual CMarker* GetCMarker() const { return cMarkerSuperElementRigid; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCMarker(CMarker* pCMarker) { cMarkerSuperElementRigid = (CMarkerSuperElementRigid*)pCMarker; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationMarker* GetVisualizationMarker() const { return visualizationMarkerSuperElementRigid; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationMarker(VisualizationMarker* pVisualizationMarker) { visualizationMarkerSuperElementRigid = (VisualizationMarkerSuperElementRigid*)pVisualizationMarker; }

    //! AUTO:  Get type name of marker (without keyword 'Marker'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "SuperElementRigid";
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cMarkerSuperElementRigid->GetParameters().bodyNumber = py::cast<Index>(d["bodyNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetVector3DSafely(d, "referencePosition", cMarkerSuperElementRigid->GetParameters().referencePosition); /*! AUTO:  safely cast to C++ type*/
        cMarkerSuperElementRigid->GetParameters().meshNodeNumbers = py::cast<std::vector<Index>>(d["meshNodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cMarkerSuperElementRigid->GetParameters().weightingFactors = py::cast<std::vector<Real>>(d["weightingFactors"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationMarkerSuperElementRigid->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VshowMarkerNodes")) { visualizationMarkerSuperElementRigid->GetShowMarkerNodes() = py::cast<bool>(d["VshowMarkerNodes"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["markerType"] = (std::string)GetTypeName();
        d["bodyNumber"] = (Index)cMarkerSuperElementRigid->GetParameters().bodyNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["referencePosition"] = (std::vector<Real>)cMarkerSuperElementRigid->GetParameters().referencePosition; //! AUTO: cast variables into python (not needed for standard types) 
        d["meshNodeNumbers"] = (std::vector<Index>)cMarkerSuperElementRigid->GetParameters().meshNodeNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["weightingFactors"] = (std::vector<Real>)cMarkerSuperElementRigid->GetParameters().weightingFactors; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationMarkerSuperElementRigid->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VshowMarkerNodes"] = (bool)visualizationMarkerSuperElementRigid->GetShowMarkerNodes(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { return py::cast((Index)cMarkerSuperElementRigid->GetParameters().bodyNumber);} //! AUTO: get parameter
        else if (parameterName.compare("referencePosition") == 0) { return py::cast((std::vector<Real>)cMarkerSuperElementRigid->GetParameters().referencePosition);} //! AUTO: get parameter
        else if (parameterName.compare("meshNodeNumbers") == 0) { return py::cast((std::vector<Index>)cMarkerSuperElementRigid->GetParameters().meshNodeNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("weightingFactors") == 0) { return py::cast((std::vector<Real>)cMarkerSuperElementRigid->GetParameters().weightingFactors);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationMarkerSuperElementRigid->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VshowMarkerNodes") == 0) { return py::cast((bool)visualizationMarkerSuperElementRigid->GetShowMarkerNodes());} //! AUTO: get parameter
        else  {PyError(STDstring("MarkerSuperElementRigid::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { cMarkerSuperElementRigid->GetParameters().bodyNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referencePosition") == 0) { EPyUtils::SetVector3DSafely(value, cMarkerSuperElementRigid->GetParameters().referencePosition); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("meshNodeNumbers") == 0) { cMarkerSuperElementRigid->GetParameters().meshNodeNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("weightingFactors") == 0) { cMarkerSuperElementRigid->GetParameters().weightingFactors = py::cast<std::vector<Real>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationMarkerSuperElementRigid->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VshowMarkerNodes") == 0) { visualizationMarkerSuperElementRigid->GetShowMarkerNodes() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("MarkerSuperElementRigid::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


