/** ***********************************************************************************************
* @class        MainObjectContactCircleCable2DParameters
* @brief        Parameter class for MainObjectContactCircleCable2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-03-04  23:29:57 (last modfied)
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
#include "Autogenerated/CObjectContactCircleCable2D.h"

#include "Autogenerated/VisuObjectContactCircleCable2D.h"

//! AUTO: Parameters for class MainObjectContactCircleCable2DParameters
class MainObjectContactCircleCable2DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectContactCircleCable2D
* @brief        A very specialized penalty-based contact condition between a 2D circle (=marker0, any Position-marker) on a body and an ANCFCable2DShape (=marker1, Marker: BodyCable2DShape), in xy-plane; a node NodeGenericData is required with the number of cordinates according to the number of contact segments; the contact gap \f$g\f$ is integrated (piecewise linear) along the cable and circle; the contact force \f$f_c\f$ is zero for \f$gap>0\f$ and otherwise computed from \f$f_c = g*contactStiffness + \dot g*contactDamping\f$; during Newton iterations, the contact force is actived only, if \f$dataCoordinate[0] <= 0\f$; dataCoordinate is set equal to gap in nonlinear iterations, but not modified in Newton iterations.
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

//! AUTO: MainObjectContactCircleCable2D
class MainObjectContactCircleCable2D: public MainObjectConnector // AUTO: 
{
protected: // AUTO: 
    CObjectContactCircleCable2D* cObjectContactCircleCable2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectContactCircleCable2D* visualizationObjectContactCircleCable2D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectContactCircleCable2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectContactCircleCable2D* GetCObjectContactCircleCable2D() { return cObjectContactCircleCable2D; }
    //! AUTO: Get const pointer to computational class
    const CObjectContactCircleCable2D* GetCObjectContactCircleCable2D() const { return cObjectContactCircleCable2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectContactCircleCable2D(CObjectContactCircleCable2D* pCObjectContactCircleCable2D) { cObjectContactCircleCable2D = pCObjectContactCircleCable2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectContactCircleCable2D* GetVisualizationObjectContactCircleCable2D() { return visualizationObjectContactCircleCable2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectContactCircleCable2D* GetVisualizationObjectContactCircleCable2D() const { return visualizationObjectContactCircleCable2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectContactCircleCable2D(VisualizationObjectContactCircleCable2D* pVisualizationObjectContactCircleCable2D) { visualizationObjectContactCircleCable2D = pVisualizationObjectContactCircleCable2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectContactCircleCable2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectContactCircleCable2D = (CObjectContactCircleCable2D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectContactCircleCable2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectContactCircleCable2D = (VisualizationObjectContactCircleCable2D*)pVisualizationObject; }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return Node::GenericData;
    }

    //! AUTO:  Get type name of node (without keyword "Object"...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ContactCircleCable2D";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectContactCircleCable2D->GetParameters().markerNumbers = py::cast<std::vector<Index>>(d["markerNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectContactCircleCable2D->GetParameters().nodeNumber = py::cast<Index>(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectContactCircleCable2D->GetParameters().numberOfContactSegments = py::cast<Index>(d["numberOfContactSegments"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectContactCircleCable2D->GetParameters().contactStiffness = py::cast<Real>(d["contactStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectContactCircleCable2D->GetParameters().contactDamping = py::cast<Real>(d["contactDamping"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectContactCircleCable2D->GetParameters().circleRadius = py::cast<Real>(d["circleRadius"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectContactCircleCable2D->GetParameters().offset = py::cast<Real>(d["offset"]); /* AUTO:  read out dictionary and cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "activeConnector")) { cObjectContactCircleCable2D->GetParameters().activeConnector = py::cast<bool>(d["activeConnector"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectContactCircleCable2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationObjectContactCircleCable2D->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectContactCircleCable2D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["markerNumbers"] = (std::vector<Index>)cObjectContactCircleCable2D->GetParameters().markerNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumber"] = (Index)cObjectContactCircleCable2D->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["numberOfContactSegments"] = (Index)cObjectContactCircleCable2D->GetParameters().numberOfContactSegments; //! AUTO: cast variables into python (not needed for standard types) 
        d["contactStiffness"] = (Real)cObjectContactCircleCable2D->GetParameters().contactStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["contactDamping"] = (Real)cObjectContactCircleCable2D->GetParameters().contactDamping; //! AUTO: cast variables into python (not needed for standard types) 
        d["circleRadius"] = (Real)cObjectContactCircleCable2D->GetParameters().circleRadius; //! AUTO: cast variables into python (not needed for standard types) 
        d["offset"] = (Real)cObjectContactCircleCable2D->GetParameters().offset; //! AUTO: cast variables into python (not needed for standard types) 
        d["activeConnector"] = (bool)cObjectContactCircleCable2D->GetParameters().activeConnector; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectContactCircleCable2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationObjectContactCircleCable2D->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectContactCircleCable2D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { return py::cast((std::vector<Index>)cObjectContactCircleCable2D->GetParameters().markerNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((Index)cObjectContactCircleCable2D->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("numberOfContactSegments") == 0) { return py::cast((Index)cObjectContactCircleCable2D->GetParameters().numberOfContactSegments);} //! AUTO: get parameter
        else if (parameterName.compare("contactStiffness") == 0) { return py::cast((Real)cObjectContactCircleCable2D->GetParameters().contactStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("contactDamping") == 0) { return py::cast((Real)cObjectContactCircleCable2D->GetParameters().contactDamping);} //! AUTO: get parameter
        else if (parameterName.compare("circleRadius") == 0) { return py::cast((Real)cObjectContactCircleCable2D->GetParameters().circleRadius);} //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { return py::cast((Real)cObjectContactCircleCable2D->GetParameters().offset);} //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { return py::cast((bool)cObjectContactCircleCable2D->GetParameters().activeConnector);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectContactCircleCable2D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationObjectContactCircleCable2D->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectContactCircleCable2D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectContactCircleCable2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumbers") == 0) { cObjectContactCircleCable2D->GetParameters().markerNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cObjectContactCircleCable2D->GetParameters().nodeNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("numberOfContactSegments") == 0) { cObjectContactCircleCable2D->GetParameters().numberOfContactSegments = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("contactStiffness") == 0) { cObjectContactCircleCable2D->GetParameters().contactStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("contactDamping") == 0) { cObjectContactCircleCable2D->GetParameters().contactDamping = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("circleRadius") == 0) { cObjectContactCircleCable2D->GetParameters().circleRadius = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("offset") == 0) { cObjectContactCircleCable2D->GetParameters().offset = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("activeConnector") == 0) { cObjectContactCircleCable2D->GetParameters().activeConnector = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectContactCircleCable2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationObjectContactCircleCable2D->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectContactCircleCable2D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectContactCircleCable2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


