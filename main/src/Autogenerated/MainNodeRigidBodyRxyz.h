/** ***********************************************************************************************
* @class        MainNodeRigidBodyRxyzParameters
* @brief        Parameter class for MainNodeRigidBodyRxyz
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-03-07  00:13:14 (last modfied)
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
#include "Autogenerated/CNodeRigidBodyRxyz.h"

#include "Autogenerated/VisuNodeRigidBodyRxyz.h"

//! AUTO: Parameters for class MainNodeRigidBodyRxyzParameters
class MainNodeRigidBodyRxyzParameters // AUTO: 
{
public: // AUTO: 
    Vector6D initialCoordinates;                  //!< AUTO: initial displacement coordinates: ux,uy,uz and 3 Euler angles (xyz) relative to reference coordinates
    Vector6D initialCoordinates_t;                //!< AUTO: initial velocity coordinate: time derivatives of ux,uy,uz and of 3 Euler angles (xyz)
    //! AUTO: default constructor with parameter initialization
    MainNodeRigidBodyRxyzParameters()
    {
        initialCoordinates = Vector6D({0.,0.,0., 0.,0.,0.});
        initialCoordinates_t = Vector6D({0.,0.,0., 0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        MainNodeRigidBodyRxyz
* @brief        A 3D rigid body node based on Euler / Tait-Bryan angles for rigid bodies or beams; all coordinates lead to second order differential equations; NOTE that this node has a singularity if the second rotation parameter reaches \f$\psi_1 = (2k-1) \pi/2\f$, with \f$k \in \Ncal\f$ or \f$-k \in \Ncal\f$.
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

//! AUTO: MainNodeRigidBodyRxyz
class MainNodeRigidBodyRxyz: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodeRigidBodyRxyz* cNodeRigidBodyRxyz; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodeRigidBodyRxyz* visualizationNodeRigidBodyRxyz; //pointer to computational object (initialized in object factory) AUTO:
    MainNodeRigidBodyRxyzParameters parameters; //! AUTO: contains all parameters for MainNodeRigidBodyRxyz

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodeRigidBodyRxyz()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodeRigidBodyRxyz* GetCNodeRigidBodyRxyz() { return cNodeRigidBodyRxyz; }
    //! AUTO: Get const pointer to computational class
    const CNodeRigidBodyRxyz* GetCNodeRigidBodyRxyz() const { return cNodeRigidBodyRxyz; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodeRigidBodyRxyz(CNodeRigidBodyRxyz* pCNodeRigidBodyRxyz) { cNodeRigidBodyRxyz = pCNodeRigidBodyRxyz; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodeRigidBodyRxyz* GetVisualizationNodeRigidBodyRxyz() { return visualizationNodeRigidBodyRxyz; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodeRigidBodyRxyz* GetVisualizationNodeRigidBodyRxyz() const { return visualizationNodeRigidBodyRxyz; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodeRigidBodyRxyz(VisualizationNodeRigidBodyRxyz* pVisualizationNodeRigidBodyRxyz) { visualizationNodeRigidBodyRxyz = pVisualizationNodeRigidBodyRxyz; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodeRigidBodyRxyz; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodeRigidBodyRxyz = (CNodeRigidBodyRxyz*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodeRigidBodyRxyz; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodeRigidBodyRxyz = (VisualizationNodeRigidBodyRxyz*)pVisualizationNode; }

    //! AUTO: Write (Reference) access to parameters
    virtual MainNodeRigidBodyRxyzParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const MainNodeRigidBodyRxyzParameters& GetParameters() const { return parameters; }

    //! AUTO:  Get type name of node (without keyword "Node"...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "RigidBodyRxyz";
    }

    //! AUTO:  Call a specific node function ==> automatically generated in future
    virtual py::object CallFunction(STDstring functionName, py::dict args) const override;

    //! AUTO:  return internally stored initial coordinates (displacements) of node
    virtual LinkedDataVector GetInitialVector() const override
    {
        return parameters.initialCoordinates;
    }

    //! AUTO:  return internally stored initial coordinates (velocities) of node
    virtual LinkedDataVector GetInitialVector_t() const override
    {
        return parameters.initialCoordinates_t;
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        EPyUtils::SetVector6DSafely(d, "referenceCoordinates", cNodeRigidBodyRxyz->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "initialCoordinates")) { EPyUtils::SetVector6DSafely(d, "initialCoordinates", GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "initialVelocities")) { EPyUtils::SetVector6DSafely(d, "initialVelocities", GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationNodeRigidBodyRxyz->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VdrawSize")) { visualizationNodeRigidBodyRxyz->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationNodeRigidBodyRxyz->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = (std::vector<Real>)cNodeRigidBodyRxyz->GetParameters().referenceCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialCoordinates"] = (std::vector<Real>)GetParameters().initialCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialVelocities"] = (std::vector<Real>)GetParameters().initialCoordinates_t; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodeRigidBodyRxyz->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationNodeRigidBodyRxyz->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationNodeRigidBodyRxyz->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return py::cast((std::vector<Real>)cNodeRigidBodyRxyz->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates_t);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodeRigidBodyRxyz->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationNodeRigidBodyRxyz->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationNodeRigidBodyRxyz->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("NodeRigidBodyRxyz::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { EPyUtils::SetVector6DSafely(value, cNodeRigidBodyRxyz->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialCoordinates") == 0) { EPyUtils::SetVector6DSafely(value, GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { EPyUtils::SetVector6DSafely(value, GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodeRigidBodyRxyz->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationNodeRigidBodyRxyz->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationNodeRigidBodyRxyz->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodeRigidBodyRxyz::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


