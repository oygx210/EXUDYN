/** ***********************************************************************************************
* @class        MainObjectANCFCable2DParameters
* @brief        Parameter class for MainObjectANCFCable2D
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
#include "Autogenerated/CObjectANCFCable2D.h"

#include "Autogenerated/VisuObjectANCFCable2D.h"

//! AUTO: Parameters for class MainObjectANCFCable2DParameters
class MainObjectANCFCable2DParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectANCFCable2D
* @brief        A 2D cable finite element using 2 nodes of type NodePoint2DSlope1; the element has 8 coordinates and uses cubic polynomials for position interpolation; the Bernoulli-Euler beam is capable of large deformation as it employs the material measure of curvature for the bending.
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

//! AUTO: MainObjectANCFCable2D
class MainObjectANCFCable2D: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectANCFCable2D* cObjectANCFCable2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectANCFCable2D* visualizationObjectANCFCable2D; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectANCFCable2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectANCFCable2D* GetCObjectANCFCable2D() { return cObjectANCFCable2D; }
    //! AUTO: Get const pointer to computational class
    const CObjectANCFCable2D* GetCObjectANCFCable2D() const { return cObjectANCFCable2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectANCFCable2D(CObjectANCFCable2D* pCObjectANCFCable2D) { cObjectANCFCable2D = pCObjectANCFCable2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectANCFCable2D* GetVisualizationObjectANCFCable2D() { return visualizationObjectANCFCable2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectANCFCable2D* GetVisualizationObjectANCFCable2D() const { return visualizationObjectANCFCable2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectANCFCable2D(VisualizationObjectANCFCable2D* pVisualizationObjectANCFCable2D) { visualizationObjectANCFCable2D = pVisualizationObjectANCFCable2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectANCFCable2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectANCFCable2D = (CObjectANCFCable2D*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectANCFCable2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectANCFCable2D = (VisualizationObjectANCFCable2D*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "ANCFCable2D";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return (Node::Type)(Node::Position2D + Node::Orientation2D + Node::Point2DSlope1);
    }

    //! AUTO:  Call a specific object function ==> automatically generated in future?
    virtual py::object CallFunction(STDstring functionName, py::dict args) const override;

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectANCFCable2D->GetParameters().physicsLength = py::cast<Real>(d["physicsLength"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().physicsMassPerLength = py::cast<Real>(d["physicsMassPerLength"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().physicsBendingStiffness = py::cast<Real>(d["physicsBendingStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().physicsAxialStiffness = py::cast<Real>(d["physicsAxialStiffness"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().physicsBendingDamping = py::cast<Real>(d["physicsBendingDamping"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().physicsAxialDamping = py::cast<Real>(d["physicsAxialDamping"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().physicsReferenceAxialStrain = py::cast<Real>(d["physicsReferenceAxialStrain"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().physicsReferenceCurvature = py::cast<Real>(d["physicsReferenceCurvature"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().nodeNumbers = py::cast<std::vector<Index>>(d["nodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cObjectANCFCable2D->GetParameters().useReducedOrderIntegration = py::cast<bool>(d["useReducedOrderIntegration"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectANCFCable2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawHeight")) { visualizationObjectANCFCable2D->GetDrawHeight() = py::cast<float>(d["VdrawHeight"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectANCFCable2D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["physicsLength"] = (Real)cObjectANCFCable2D->GetParameters().physicsLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsMassPerLength"] = (Real)cObjectANCFCable2D->GetParameters().physicsMassPerLength; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsBendingStiffness"] = (Real)cObjectANCFCable2D->GetParameters().physicsBendingStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsAxialStiffness"] = (Real)cObjectANCFCable2D->GetParameters().physicsAxialStiffness; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsBendingDamping"] = (Real)cObjectANCFCable2D->GetParameters().physicsBendingDamping; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsAxialDamping"] = (Real)cObjectANCFCable2D->GetParameters().physicsAxialDamping; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsReferenceAxialStrain"] = (Real)cObjectANCFCable2D->GetParameters().physicsReferenceAxialStrain; //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsReferenceCurvature"] = (Real)cObjectANCFCable2D->GetParameters().physicsReferenceCurvature; //! AUTO: cast variables into python (not needed for standard types) 
        d["nodeNumbers"] = (std::vector<Index>)cObjectANCFCable2D->GetParameters().nodeNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["useReducedOrderIntegration"] = (bool)cObjectANCFCable2D->GetParameters().useReducedOrderIntegration; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectANCFCable2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawHeight"] = (float)visualizationObjectANCFCable2D->GetDrawHeight(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectANCFCable2D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { return py::cast((Real)cObjectANCFCable2D->GetParameters().physicsLength);} //! AUTO: get parameter
        else if (parameterName.compare("physicsMassPerLength") == 0) { return py::cast((Real)cObjectANCFCable2D->GetParameters().physicsMassPerLength);} //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingStiffness") == 0) { return py::cast((Real)cObjectANCFCable2D->GetParameters().physicsBendingStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialStiffness") == 0) { return py::cast((Real)cObjectANCFCable2D->GetParameters().physicsAxialStiffness);} //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingDamping") == 0) { return py::cast((Real)cObjectANCFCable2D->GetParameters().physicsBendingDamping);} //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialDamping") == 0) { return py::cast((Real)cObjectANCFCable2D->GetParameters().physicsAxialDamping);} //! AUTO: get parameter
        else if (parameterName.compare("physicsReferenceAxialStrain") == 0) { return py::cast((Real)cObjectANCFCable2D->GetParameters().physicsReferenceAxialStrain);} //! AUTO: get parameter
        else if (parameterName.compare("physicsReferenceCurvature") == 0) { return py::cast((Real)cObjectANCFCable2D->GetParameters().physicsReferenceCurvature);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { return py::cast((std::vector<Index>)cObjectANCFCable2D->GetParameters().nodeNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("useReducedOrderIntegration") == 0) { return py::cast((bool)cObjectANCFCable2D->GetParameters().useReducedOrderIntegration);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectANCFCable2D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawHeight") == 0) { return py::cast((float)visualizationObjectANCFCable2D->GetDrawHeight());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectANCFCable2D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectANCFCable2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsLength") == 0) { cObjectANCFCable2D->GetParameters().physicsLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsMassPerLength") == 0) { cObjectANCFCable2D->GetParameters().physicsMassPerLength = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingStiffness") == 0) { cObjectANCFCable2D->GetParameters().physicsBendingStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialStiffness") == 0) { cObjectANCFCable2D->GetParameters().physicsAxialStiffness = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsBendingDamping") == 0) { cObjectANCFCable2D->GetParameters().physicsBendingDamping = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsAxialDamping") == 0) { cObjectANCFCable2D->GetParameters().physicsAxialDamping = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsReferenceAxialStrain") == 0) { cObjectANCFCable2D->GetParameters().physicsReferenceAxialStrain = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("physicsReferenceCurvature") == 0) { cObjectANCFCable2D->GetParameters().physicsReferenceCurvature = py::cast<Real>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { cObjectANCFCable2D->GetParameters().nodeNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("useReducedOrderIntegration") == 0) { cObjectANCFCable2D->GetParameters().useReducedOrderIntegration = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectANCFCable2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawHeight") == 0) { visualizationObjectANCFCable2D->GetDrawHeight() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectANCFCable2D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectANCFCable2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


