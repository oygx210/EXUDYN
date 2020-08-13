/** ***********************************************************************************************
* @class        MainLoadMassProportionalParameters
* @brief        Parameter class for MainLoadMassProportional
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-07-20  12:33:24 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef MAINLOADMASSPROPORTIONALPARAMETERS__H
#define MAINLOADMASSPROPORTIONALPARAMETERS__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CLoadMassProportional.h"

#include "Autogenerated/VisuLoadMassProportional.h"

//! AUTO: Parameters for class MainLoadMassProportionalParameters
class MainLoadMassProportionalParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainLoadMassProportional
* @brief        Load attached to BodyMass-based marker, applying a 3D vector load (e.g. the vector [0,-g,0] is used to apply gravitational loading of size g in negative y-direction).
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

//! AUTO: MainLoadMassProportional
class MainLoadMassProportional: public MainLoad // AUTO: 
{
protected: // AUTO: 
    CLoadMassProportional* cLoadMassProportional; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationLoadMassProportional* visualizationLoadMassProportional; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainLoadMassProportional()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CLoadMassProportional* GetCLoadMassProportional() { return cLoadMassProportional; }
    //! AUTO: Get const pointer to computational class
    const CLoadMassProportional* GetCLoadMassProportional() const { return cLoadMassProportional; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCLoadMassProportional(CLoadMassProportional* pCLoadMassProportional) { cLoadMassProportional = pCLoadMassProportional; }

    //! AUTO: Get pointer to visualization class
    VisualizationLoadMassProportional* GetVisualizationLoadMassProportional() { return visualizationLoadMassProportional; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationLoadMassProportional* GetVisualizationLoadMassProportional() const { return visualizationLoadMassProportional; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationLoadMassProportional(VisualizationLoadMassProportional* pVisualizationLoadMassProportional) { visualizationLoadMassProportional = pVisualizationLoadMassProportional; }

    //! AUTO: Get const pointer to computational base class object
    virtual CLoad* GetCLoad() const { return cLoadMassProportional; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCLoad(CLoad* pCLoad) { cLoadMassProportional = (CLoadMassProportional*)pCLoad; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationLoad* GetVisualizationLoad() const { return visualizationLoadMassProportional; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationLoad(VisualizationLoad* pVisualizationLoad) { visualizationLoadMassProportional = (VisualizationLoadMassProportional*)pVisualizationLoad; }

    //! AUTO:  Get type name of load (without keyword 'Load'...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "MassProportional";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cLoadMassProportional->GetParameters().markerNumber = py::cast<Index>(d["markerNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetVector3DSafely(d, "loadVector", cLoadMassProportional->GetParameters().loadVector); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "loadVectorUserFunction")) { if (EPyUtils::CheckForValidFunction(d["loadVectorUserFunction"])) { cLoadMassProportional->GetParameters().loadVectorUserFunction = py::cast<std::function<StdVector(Real,StdVector3D)>>((py::function)d["loadVectorUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationLoadMassProportional->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["loadType"] = (std::string)GetTypeName();
        d["markerNumber"] = (Index)cLoadMassProportional->GetParameters().markerNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["loadVector"] = (std::vector<Real>)cLoadMassProportional->GetParameters().loadVector; //! AUTO: cast variables into python (not needed for standard types) 
        if (cLoadMassProportional->GetParameters().loadVectorUserFunction)
            {d["loadVectorUserFunction"] = (std::function<StdVector(Real,StdVector3D)>)cLoadMassProportional->GetParameters().loadVectorUserFunction;}
        else
            {d["loadVectorUserFunction"] = 0;}
 //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationLoadMassProportional->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { return py::cast((Index)cLoadMassProportional->GetParameters().markerNumber);} //! AUTO: get parameter
        else if (parameterName.compare("loadVector") == 0) { return py::cast((std::vector<Real>)cLoadMassProportional->GetParameters().loadVector);} //! AUTO: get parameter
        else if (parameterName.compare("loadVectorUserFunction") == 0) { return py::cast((std::function<StdVector(Real,StdVector3D)>)cLoadMassProportional->GetParameters().loadVectorUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationLoadMassProportional->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("LoadMassProportional::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("markerNumber") == 0) { cLoadMassProportional->GetParameters().markerNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("loadVector") == 0) { EPyUtils::SetVector3DSafely(value, cLoadMassProportional->GetParameters().loadVector); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("loadVectorUserFunction") == 0) { cLoadMassProportional->GetParameters().loadVectorUserFunction = py::cast<std::function<StdVector(Real,StdVector3D)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationLoadMassProportional->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("LoadMassProportional::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};



#endif //#ifdef include once...
