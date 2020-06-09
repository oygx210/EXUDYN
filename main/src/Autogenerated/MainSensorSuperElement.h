/** ***********************************************************************************************
* @class        MainSensorSuperElementParameters
* @brief        Parameter class for MainSensorSuperElement
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
#include "Autogenerated/CSensorSuperElement.h"

#include "Autogenerated/VisuSensorSuperElement.h"

//! AUTO: Parameters for class MainSensorSuperElementParameters
class MainSensorSuperElementParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainSensorSuperElement
* @brief        A sensor attached to a SuperElement-object with mesh node number. As a difference to other ObjectSensors, the SuperElement sensor has a mesh node number at which the sensor is attached to. The sensor measures OutputVariableSuperElement and outputs values into a file, showing per line [time, sensorValue[0], sensorValue[1], ...]. A user function can be attached to postprocess sensor values accordingly.
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

//! AUTO: MainSensorSuperElement
class MainSensorSuperElement: public MainSensor // AUTO: 
{
protected: // AUTO: 
    CSensorSuperElement* cSensorSuperElement; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationSensorSuperElement* visualizationSensorSuperElement; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainSensorSuperElement()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CSensorSuperElement* GetCSensorSuperElement() { return cSensorSuperElement; }
    //! AUTO: Get const pointer to computational class
    const CSensorSuperElement* GetCSensorSuperElement() const { return cSensorSuperElement; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCSensorSuperElement(CSensorSuperElement* pCSensorSuperElement) { cSensorSuperElement = pCSensorSuperElement; }

    //! AUTO: Get pointer to visualization class
    VisualizationSensorSuperElement* GetVisualizationSensorSuperElement() { return visualizationSensorSuperElement; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationSensorSuperElement* GetVisualizationSensorSuperElement() const { return visualizationSensorSuperElement; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationSensorSuperElement(VisualizationSensorSuperElement* pVisualizationSensorSuperElement) { visualizationSensorSuperElement = pVisualizationSensorSuperElement; }

    //! AUTO: Get const pointer to computational base class object
    virtual CSensor* GetCSensor() const { return cSensorSuperElement; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCSensor(CSensor* pCSensor) { cSensorSuperElement = (CSensorSuperElement*)pCSensor; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationSensor* GetVisualizationSensor() const { return visualizationSensorSuperElement; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationSensor(VisualizationSensor* pVisualizationSensor) { visualizationSensorSuperElement = (VisualizationSensorSuperElement*)pVisualizationSensor; }

    //! AUTO:  Get type name of sensor (without keyword 'Sensor'...!)
    virtual const char* GetTypeName() const override
    {
        return "SuperElement";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cSensorSuperElement->GetParameters().bodyNumber = py::cast<Index>(d["bodyNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cSensorSuperElement->GetParameters().meshNodeNumber = py::cast<Index>(d["meshNodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cSensorSuperElement->GetParameters().writeToFile = py::cast<bool>(d["writeToFile"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "fileName", cSensorSuperElement->GetParameters().fileName); /*! AUTO:  safely cast to C++ type*/
        cSensorSuperElement->GetParameters().outputVariableType = (OutputVariableType)py::cast<Index>(d["outputVariableType"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationSensorSuperElement->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["sensorType"] = (std::string)GetTypeName();
        d["bodyNumber"] = (Index)cSensorSuperElement->GetParameters().bodyNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["meshNodeNumber"] = (Index)cSensorSuperElement->GetParameters().meshNodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["writeToFile"] = (bool)cSensorSuperElement->GetParameters().writeToFile; //! AUTO: cast variables into python (not needed for standard types) 
        d["fileName"] = (std::string)cSensorSuperElement->GetParameters().fileName; //! AUTO: cast variables into python (not needed for standard types) 
        d["outputVariableType"] = (OutputVariableType)cSensorSuperElement->GetParameters().outputVariableType; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationSensorSuperElement->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { return py::cast((Index)cSensorSuperElement->GetParameters().bodyNumber);} //! AUTO: get parameter
        else if (parameterName.compare("meshNodeNumber") == 0) { return py::cast((Index)cSensorSuperElement->GetParameters().meshNodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("writeToFile") == 0) { return py::cast((bool)cSensorSuperElement->GetParameters().writeToFile);} //! AUTO: get parameter
        else if (parameterName.compare("fileName") == 0) { return py::cast((std::string)cSensorSuperElement->GetParameters().fileName);} //! AUTO: get parameter
        else if (parameterName.compare("outputVariableType") == 0) { return py::cast((OutputVariableType)cSensorSuperElement->GetParameters().outputVariableType);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationSensorSuperElement->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("SensorSuperElement::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("bodyNumber") == 0) { cSensorSuperElement->GetParameters().bodyNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("meshNodeNumber") == 0) { cSensorSuperElement->GetParameters().meshNodeNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("writeToFile") == 0) { cSensorSuperElement->GetParameters().writeToFile = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("fileName") == 0) { EPyUtils::SetStringSafely(value, cSensorSuperElement->GetParameters().fileName); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("outputVariableType") == 0) { cSensorSuperElement->GetParameters().outputVariableType = py::cast<OutputVariableType>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationSensorSuperElement->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("SensorSuperElement::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


