/** ***********************************************************************************************
* @brief		Implementation of class VisualizationSystem
* @details		Details:
 				- a visualization system, containing data and functions for visualization
*
* @author		Gerstmayr Johannes
* @date			2019-05-24 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: missing
* 				
*
* *** Example code ***
*
************************************************************************************************ */
#pragma once

//#include "System/CSystem.h"						//included in VisualizationSystem.h
//#include "Autogenerated/VisualizationSettings.h"  //included in VisualizationSystem.h
//#include "Graphics/GraphicsData.h"				//included in VisualizationSystem.h
//#include "Graphics/VisualizationSystem.h"
#include "Graphics/VisualizationSystemContainer.h"  //includes everything needed

#ifdef USE_GLFW_GRAPHICS
#include "Graphics/GlfwClient.h" //in order to link to graphics engine
#endif


void VisualizationSystem::Reset()
{
	graphicsData.GetVisualizationCounter() = 0;
	graphicsData.FlushData();
	graphicsData.GetUpdateGraphicsDataNow() = false;

	GetVisualizationSystemData().Reset();
}

void VisualizationSystem::LinkToSystemData(CSystemData* systemDataInit)
{
	systemData = systemDataInit;
}

void VisualizationSystem::LinkPostProcessData(PostProcessData* postProcessDataInit)
{
	postProcessData = postProcessDataInit;
}

//! Renderer reports to simulation that simulation shall be interrupted
void VisualizationSystem::StopSimulation()
{
	postProcessData->stopSimulation = true;
}

//! OpenGL renderer sends message that graphics shall be updated ==> update graphics data
void VisualizationSystem::UpdateGraphicsData(VisualizationSystemContainer& visualizationSystemContainer)
{
	postProcessData->accessState.test_and_set(std::memory_order_acquire); //computation thread must be interrupted before further update
	postProcessData->visualizationIsRunning = true; //signal, that visualization is running

	if (postProcessData->updateCounter == postProcessData->recordImageCounter) //this is the signal that a frame shall be recorded
	{
		visualizationSystemContainer.saveImage = true;
		visualizationSystemContainer.saveImageOpenGL = true; //send immediately signal to openGL that frame needs to be recorded
		postProcessData->recordImageCounter--;				 //decrease value such that next update does not lead to image record
	}

	if (((postProcessData->postProcessDataReady && postProcessData->updateCounter > graphicsData.GetVisualizationCounter()) 
		|| visualizationSystemContainer.UpdateGraphicsDataNowInternal()) && systemData->GetCData().IsSystemConsistent())
	{
		graphicsData.GetVisualizationCounter() = postProcessData->updateCounter; //next update will only be done if postProcessData->updateCounter increases
		
		//flushdata also locks data ...
		graphicsData.FlushData(); //currently data is always recomputed; FUTURE: differ between structure update and vertex/rigid body update, etc.

		//put this after FlushData():
		graphicsData.LockData(); //avoid that data is cleared by computation/Python thread

		Index cnt;
		postProcessData->visualizationTime = systemData->GetCData().GetVisualization().GetTime(); //update time, synchronized with the state shown 

		//++++++++++++++++++++++++++++++++++++++++++++++
		//visualize nodes:
		//pout << "UpdateGraphicsData nodes1\n";
		if (visualizationSystemContainer.settings.nodes.show)
		{
			cnt = 0;
			for (auto item : vSystemData.GetVisualizationNodes())
			{
				if (item->GetShow()) { item->UpdateGraphics(visualizationSystemContainer.GetVisualizationSettings(), this, cnt); }
				cnt++; //synchronize itemNumber with item!!!
			}
		}
		//pout << "UpdateGraphicsData nodes2\n";

		//++++++++++++++++++++++++++++++++++++++++++++++
		//visualize connectors: (draw connectors before objects, to make coordinate systems visible inside of bodies
		if (visualizationSystemContainer.settings.connectors.show)
		{
			cnt = 0;
			for (auto item : vSystemData.GetVisualizationObjects())
			{
				if (item->GetShow() && item->IsConnector()) { item->UpdateGraphics(visualizationSystemContainer.GetVisualizationSettings(), this, cnt); }
				cnt++; //synchronize itemNumber with item!!!
			}
		}

		//++++++++++++++++++++++++++++++++++++++++++++++
		//visualize objects:
		if (visualizationSystemContainer.settings.bodies.show)
		{
			cnt = 0;
			for (auto item : vSystemData.GetVisualizationObjects())
			{
				if (item->GetShow() && !(item->IsConnector())) { item->UpdateGraphics(visualizationSystemContainer.GetVisualizationSettings(), this, cnt); }
				cnt++; //synchronize itemNumber with item!!!
			}
		}

		//++++++++++++++++++++++++++++++++++++++++++++++
		//recompute colors in contourplot objects (LATERON: add triangles ...!)
		if (visualizationSystemContainer.settings.contour.outputVariable != OutputVariableType::_None)
		{
			float minVal = visualizationSystemContainer.settings.contour.minValue;
			float maxVal = visualizationSystemContainer.settings.contour.maxValue;
			if (visualizationSystemContainer.settings.contour.automaticRange)
			{
				float storedMinVal = graphicsData.GetContourCurrentMinValue(); //store range, for reduceRange option
				float storedMaxVal = graphicsData.GetContourCurrentMaxValue();

				//std::cout << "compute automatic range\n";
				if (visualizationSystemContainer.settings.contour.reduceRange)
				{
					minVal = EXUstd::MAXFLOAT;
					maxVal = EXUstd::MINFLOAT;
				}
				else
				{
					minVal = storedMinVal;
					maxVal = storedMaxVal;
				}
				//normalize contour plot values
				for (auto item : graphicsData.glLines)
				{
					if (item.color1[3] == contourPlotFlag) //just check color1
					{
						minVal = EXUstd::Minimum(minVal, item.color1[0]); //contour plot value is given in RED channel
						minVal = EXUstd::Minimum(minVal, item.color2[0]);
						maxVal = EXUstd::Maximum(maxVal, item.color1[0]); //contour plot value is given in RED channel
						maxVal = EXUstd::Maximum(maxVal, item.color2[0]);
					}
				}
				for (auto item : graphicsData.glCirclesXY)
				{
					if (item.color[3] == contourPlotFlag)
					{
						minVal = EXUstd::Minimum(minVal, item.color[0]); //contour plot value is given in RED channel
						maxVal = EXUstd::Maximum(maxVal, item.color[0]); //contour plot value is given in RED channel
					}
				}
				for (auto item : graphicsData.glPoints)
				{
					if (item.color[3] == contourPlotFlag)
					{
						minVal = EXUstd::Minimum(minVal, item.color[0]); //contour plot value is given in RED channel
						maxVal = EXUstd::Maximum(maxVal, item.color[0]); //contour plot value is given in RED channel
					}
				}
				for (auto item : graphicsData.glTriangles)
				{
					if (item.colors[0][3] == contourPlotFlag) //just check color of first node
					{
						for (const Float4& color : item.colors)
						{
							minVal = EXUstd::Minimum(minVal, color[0]); //contour plot value is given in RED channel
							maxVal = EXUstd::Maximum(maxVal, color[0]); //contour plot value is given in RED channel
						}
					}
				}

				//std::cout << "range=" << minVal << ", " << maxVal << "\n";
				if (minVal == EXUstd::MAXFLOAT) { minVal = 0; } //introduce standard range if no items found!
				if (maxVal == EXUstd::MINFLOAT) { maxVal = 1; }

				if (!visualizationSystemContainer.settings.contour.reduceRange)
				{
					if (minVal > storedMinVal) { minVal = storedMinVal; }
					if (maxVal < storedMaxVal) { maxVal = storedMaxVal; }
				}
			}

			//transfer computed or set values to renderer (color bar, etc.)
			graphicsData.GetContourCurrentMinValue() = minVal;
			graphicsData.GetContourCurrentMaxValue() = maxVal;

			//now recompute color as given in the colorbar
			for (auto& item : graphicsData.glLines)
			{
				if (item.color1[3] == contourPlotFlag)
				{
					item.color1 = VisualizationSystemContainerBase::ColorBarColor(minVal, maxVal, item.color1[0]);
					item.color2 = VisualizationSystemContainerBase::ColorBarColor(minVal, maxVal, item.color2[0]);
				}
			}
			for (auto& item : graphicsData.glCirclesXY)
			{
				if (item.color[3] == contourPlotFlag)
				{
					item.color = VisualizationSystemContainerBase::ColorBarColor(minVal, maxVal, item.color[0]);
				}
			}
			for (auto& item : graphicsData.glPoints)
			{
				if (item.color[3] == contourPlotFlag)
				{
					item.color = VisualizationSystemContainerBase::ColorBarColor(minVal, maxVal, item.color[0]);
				}
			}
			for (auto& item : graphicsData.glTriangles)
			{
				if (item.colors[0][3] == contourPlotFlag) //just check color of first node
				{
					for (Float4& color : item.colors)
					{
						color = VisualizationSystemContainerBase::ColorBarColor(minVal, maxVal, color[0]);
					}
				}
			}
		}

		//++++++++++++++++++++++++++++++++++++++++++++++
		//visualize markers:
		if (visualizationSystemContainer.settings.markers.show)
		{
			cnt = 0;
			for (auto item : vSystemData.GetVisualizationMarkers())
			{
				if (item->GetShow()) { item->UpdateGraphics(visualizationSystemContainer.GetVisualizationSettings(), this, cnt); }
				cnt++; //synchronize itemNumber with item!!!
			}
		}

		//++++++++++++++++++++++++++++++++++++++++++++++
		//visualize loads:
		if (visualizationSystemContainer.settings.loads.show)
		{
			cnt = 0;
			for (auto item : vSystemData.GetVisualizationLoads())
			{
				if (item->GetShow()) { item->UpdateGraphics(visualizationSystemContainer.GetVisualizationSettings(), this, cnt); }
				cnt++; //synchronize itemNumber with item!!!
			}
		}

		//++++++++++++++++++++++++++++++++++++++++++++++
		//visualize sensors:
		if (visualizationSystemContainer.settings.sensors.show)
		{
			cnt = 0;
			for (auto item : vSystemData.GetVisualizationSensors())
			{
				if (item->GetShow()) { item->UpdateGraphics(visualizationSystemContainer.GetVisualizationSettings(), this, cnt); }
				cnt++; //synchronize itemNumber with item!!!
			}
		}

		graphicsData.ClearLock();
	}
	postProcessData->accessState.clear(std::memory_order_release); //computation thread must be interrupted before further update
}

//! any multi-line text message from computation to be shown in renderer (e.g. time, solver, ...)
std::string VisualizationSystem::GetComputationMessage()
{
	postProcessData->accessState.test_and_set(std::memory_order_acquire); //computation thread must be interrupted before further update
	
	std::string str = std::string("EXUDYN\n") + postProcessData->GetVisualizationMessage() + "\ntime = " + EXUstd::ToString(postProcessData->visualizationTime);
	postProcessData->accessState.clear(std::memory_order_release); //computation thread must be interrupted before further update

	return str;
}
