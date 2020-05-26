/** ***********************************************************************************************
* @class        VisualizationMarkerObjectODE2Coordinates
* @brief        A Marker attached to all coordinates of an object (usually body), e.g. to apply special constraints or loads on all coordinates. The measured coordinates INCLUDE reference + current coordinates.
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-05-24  23:29:04 (last modfied)
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

class VisualizationMarkerObjectODE2Coordinates: public VisualizationMarker // AUTO: 
{
protected: // AUTO: 

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    VisualizationMarkerObjectODE2Coordinates()
    {
        show = true;
    };

    // AUTO: access functions
    //! AUTO:  Update visualizationSystem -> graphicsData for item; index shows item Number in CData
    virtual void UpdateGraphics(const VisualizationSettings& visualizationSettings, VisualizationSystem* vSystem, Index itemNumber) override
    {
        
    }

};


