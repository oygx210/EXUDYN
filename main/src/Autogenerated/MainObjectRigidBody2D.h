/** ***********************************************************************************************
* @class        MainObjectRigidBody2DParameters
* @brief        Parameter class for MainObjectRigidBody2D
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
#include "Autogenerated/CObjectRigidBody2D.h"

#include "Autogenerated/VisuObjectRigidBody2D.h"

//! AUTO: Parameters for class MainObjectRigidBody2DParameters
class MainObjectRigidBody2DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectRigidBody2D
* @brief        A 2D rigid body which is attached to a rigid body 2D node. The body obtains coordinates, position, velocity, etc. from the underlying 2D node
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

//! AUTO: MainObjectRigidBody2D
class MainObjectRigidBody2D: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectRigidBody2D* cObjectRigidBody2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectRigidBody2D* visualizationObjectRigidBody2D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectRigidBody2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectRigidBody2D* GetCObjectRigidBody2D() { return cObjectRigidBody2D; }
    //! AUTO: Get const pointer to computational class
    const CObjectRigidBody2D* GetCObjectRigidBody2D() const { return cObjectRigidBody2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectRigidBody2D(CObjectRigidBody2D* pCObjectRigidBody2D) { cObjectRigidBody2D = pCObjectRigidBody2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectRigidBody2D* GetVisualizationObjectRigidBody2D() { return visualizationObjectRigidBody2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectRigidBody2D* GetVisualizationObjectRigidBody2D() const { return visualizationObjectRigidBody2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectRigidBody2D(VisualizationObjectRigidBody2D* pVisualizationObjectRigidBody2D) { visualizationObjectRigidBody2D = pVisualizationObjectRigidBody2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectRigidBody2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectRigidBody2D = (CObjectRigidBody2D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectRigidBody2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectRigidBody2D = (VisualizationObjectRigidBody2D*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "RigidBody2D";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return (Node::Type)(Node::Position2D + Node::Orientation2D);
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectRigidBody2D->GetParameters().physicsMass = py::cast<Real>(d["physicsMass"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectRigidBody2D->GetParameters().physicsInertia = py::cast<Real>(d["physicsInertia"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectRigidBody2D->GetParameters().nodeNumber = py::cast<Index>(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectRigidBody2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VgraphicsData")) { PyWriteBodyGraphicsData(d, "VgraphicsData", visualizationObjectRigidBody2D->GetGraphicsData()); /*! AUTO: convert dict to BodyGraphicsData*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["physicsMass"] = (Real)cObjectRigidBody2D->GetParameters().physicsMass; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsInertia"] = (Real)cObjectRigidBody2D->GetParameters().physicsInertia; //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumber"] = (Index)cObjectRigidBody2D->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectRigidBody2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VgraphicsData"] = PyGetBodyGraphicsDataDictionary(visualizationObjectRigidBody2D->GetGraphicsData()); //! AUTO: generate dictionary with special function
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { return py::cast((Real)cObjectRigidBody2D->GetParameters().physicsMass);} //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { return py::cast((Real)cObjectRigidBody2D->GetParameters().physicsInertia);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((Index)cObjectRigidBody2D->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectRigidBody2D->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectRigidBody2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { cObjectRigidBody2D->GetParameters().physicsMass = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { cObjectRigidBody2D->GetParameters().physicsInertia = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cObjectRigidBody2D->GetParameters().nodeNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectRigidBody2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectRigidBody2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};


