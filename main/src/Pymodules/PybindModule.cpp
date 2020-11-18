/** ***********************************************************************************************
* @brief		Implementation file for linking to python; includes several automatically generated header files inside of module (kind of hack)
* @details		Details:
				- main module linked to python (python interface)
*
* @author		Stefan Holzinger / Gerstmayr Johannes
* @date			2019-03-29 
* @pre			...
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

//++++++++++++++++++++++++++
//for signal, catch CTRL-C in console:
//#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <array>
#include <vector>
#include <signal.h>
//++++++++++++++++++++++++++

// pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>       //interface to numpy
#include <pybind11/buffer_info.h> //passing reference to matrix to numpy
#include <pybind11/embed.h>       //scoped interpreter
//does not work globally: #include <pybind11/iostream.h> //used to redirect cout:  py::scoped_ostream_redirect output;
#include <pybind11/cast.h> //for arguments
#include <pybind11/functional.h> //for function handling ... otherwise gives a python error (no compilation error in C++ !)
namespace py = pybind11;
using namespace pybind11::literals; //brings in the '_a' literals; e.g. for short arguments definition

// includes needed for glfw test example
#define NOMINMAX //needs to be placed on top (before windows.h)! Otherwise std::min/max will cause error msg!
#include <cmath>

#include "Linalg/BasicLinalg.h"

// glfw testclass. This  includes the glfw test example
#include "Graphics/GlfwClient.h"

#include <iostream>
#include <ostream>
#include <fstream>

#include "System/versionCpp.h"

#include "Linalg/LinearSolver.h"

#include "Linalg/Geometry.h"

#include "Main/MainSystem.h"

#include "Pymodules/PybindUtilities.h"
#include "Pymodules/PyMatrixContainer.h"
#include "Autogenerated/MainObjectMassPoint.h"
#include "Autogenerated/MainNodePoint.h"

#include "Solver/TimeIntegrationSolver.h"
#include "Autogenerated/SystemContainer.h"

#include "Main/MainSystemContainer.h"
#include "System/ItemIndices.h"

#include "Solver/CSolverImplicitSecondOrder.h"
#include "Solver/CSolverStatic.h"
#include "Solver/MainSolverBase.h"
#include "Autogenerated/MainSolver.h" //pybind access to solvers
#include "Autogenerated/SimulationSettings.h" 
#include "Autogenerated/DictionariesGetSet.h"


#include "Tests/UnitTestBase.h" //for unit tests

#include "Pymodules/PybindTests.h"

//#include "../../include/ngs-core-master/autodiff.hpp" //for testing

#ifdef USE_GLFW_GRAPHICS
	extern GlfwRenderer glfw;
#endif


#ifdef __EXUDYN_RUNTIME_CHECKS__
extern Index array_new_counts;		//global counter of item allocations; is increased every time a new is called
extern Index array_delete_counts;	//global counter of item deallocations; is increased every time a delete is called
extern Index vector_new_counts;	//global counter of item allocations; is increased every time a new is called
extern Index vector_delete_counts; //global counter of item deallocations; is increased every time a delete is called
extern Index matrix_new_counts;	//global counter of item allocations; is increased every time a new is called
extern Index matrix_delete_counts; //global counter of item deallocations; is increased every time a delete is called
#endif


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//functions linked to exudyn
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//! retrieve current version as m.attr is not passed trough package
py::str PyGetVersionString()
{
	STDstring str = STDstring(EXUstd::exudynVersion);
#ifndef EXUDYN_RELEASE
	str += "(pre-release)";
#pragma message("====================================")
#pragma message("EXUDYN not compiled in release mode!")
#pragma message("====================================")
#endif
	return str;
}

//! Definition of Invalid Index; to be used in Python to check whether a function returned a valid index (e.g. AddObject(...))
Index GetInvalidIndex() { return EXUstd::InvalidIndex; }

//! set flag to write (true) or not write to console; default = true
void PySetWriteToConsole(bool flag) { outputBuffer.SetWriteToConsole(flag); }

//! set flag to write (true) or not write to console; default = false
void PySetWriteToFile(STDstring filename, bool flagWriteToFile, bool flagAppend)
{
	outputBuffer.SetWriteToFile(filename, flagWriteToFile, flagAppend);
} 

////redirect printing via exudyn, such that all output can be streamed to file ...
//void PyPrint(py::object pyObject)
//{
//	pout << py::str(pyObject);
//	//py::print(pyObject);
//}
//
//! print function with line feed
void PyPrint(py::args args)
{
	for (auto item: args)
	{
		pout << item << " ";
	}
	pout << "\n";
}
//! add some delay (in milliSeconds) to printing to console, in order to let Spyder process the output; default = 0
void PySetPrintDelayMilliSeconds(Index delayMilliSeconds)
{
	outputBuffer.SetDelayMilliSeconds(delayMilliSeconds);
}


//Print some (Debug) infos: linalg, threads, computational efficiency, etc.
py::list PythonInfoStat()
{
	py::list list;
#ifdef __EXUDYN_RUNTIME_CHECKS__
	pout << "Linalg stats:\n";
	pout << "  array_new_counts:   " <<  array_new_counts    << "\n";
	pout << "  array_delete_counts:" <<  array_delete_counts << "\n";
	pout << "  vector_new_counts:   " << vector_new_counts   << "\n";
	pout << "  vector_delete_counts:" << vector_delete_counts << "\n";
	pout << "  matrix_new_counts:   " << matrix_new_counts   << "\n";
	pout << "  matrix_delete_counts:" << matrix_delete_counts << "\n";
	list.append(array_new_counts);
	list.append(array_delete_counts);
	list.append(vector_new_counts);
	list.append(vector_delete_counts);
	list.append(matrix_new_counts);
	list.append(matrix_delete_counts);
#else
	pout << "Linalg stats deactivated (needs re-compile)\n";
#endif
	return list;


}

//! Set the precision for floating point numbers written to console; this is reset after a simulation is started by according simulation settings
void PySetOutputPrecision(Index precision)
{
	std::cout.precision(precision);
	pout.precision(precision);
}

extern bool linalgPrintUsePythonFormat; //!< true: use python format for output of vectors and matrices; false: use matlab format

//! true: use python format for output of vectors and matrices; false: use matlab format
void PySetLinalgOutputFormatPython(bool flagPythonFormat)
{
	linalgPrintUsePythonFormat = flagPythonFormat;
}

//! start glfw renderer; return true if successful
bool PyStartOpenGLRenderer(bool verbose=true)
{
#ifdef USE_GLFW_GRAPHICS
	return glfwRenderer.SetupRenderer(verbose);
#else
	PyWarning("GLFW_Graphics deactivated");
	return false;
#endif
}

//! start glfw renderer; return true if successful
void PyStopOpenGLRenderer()
{
#ifdef USE_GLFW_GRAPHICS
	glfwRenderer.StopRenderer();
#else
	PyWarning("GLFW_Graphics deactivated");
#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) 
{
	if (signum == SIGINT)
	{
		//std::cout << "Caught signal " << signum << "\n";
		//std::cout << "Process interrupted by user (CTRL+C)!\n";
		pout << "Process interrupted by user (CTRL+C)!\n";
		// Terminate program
		exit(signum);
	}
}

//enable CTRL-C in DOS windows
class MySignal 
{
public:
	MySignal()
	{
		// Register signal and signal handler
		signal(SIGINT, signal_callback_handler);
	}
};

MySignal registerSignal;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//test functions, will be eliminated from time to time


void PyTest()
{
	//add testing here
	Vector v1({ 1,2,3 });
	Vector v2({ 8,7,6 });
	Matrix m(3, 2, { 1,2,3, 4,5,6 });
	Vector r({ 0,1 });

	EXUmath::MultMatrixVectorAdd(m, v1, r);
	pout << "r1=" << r << "\n";				//=> (14, 33)
	EXUmath::MultMatrixVectorAdd(m, v2, r); 
	pout << "r2=" << r << "\n";				//=> (54,136)
}


py::object GetVector()
{
	Vector v({ 42.1234567890123456,43,44 }); //double precision maintained in NumPy array in python
	return py::array_t<Real>(v.NumberOfItems(), v.GetDataPointer()); //copy array (could also be referenced!)
}

//py::object GetMatrix()
py::array_t<Real> GetMatrix()
{
	ResizableMatrix m(2, 3, { 12.5,13,14,  15,16,17 }); //double precision maintained in NumPy array in python

	return py::array_t<Real>(std::vector<std::ptrdiff_t>{(int)m.NumberOfRows(), (int)m.NumberOfColumns()}, m.GetDataPointer()); //copy array (could also be referenced!)
}

void SeeMatrix(py::array_t<Real> pyArray)
{
	pout << "ndim=" << pyArray.ndim() << "\n";
	pout << "size=" << pyArray.size() << "\n";
	if (pyArray.ndim() == 2)
	{
		auto mat = pyArray.unchecked<2>();
		Index nrows = mat.shape(0);
		Index ncols = mat.shape(1);

		Matrix m(nrows, ncols);
		for (Index i = 0; i < nrows; i++)
		{
			for (Index j = 0; j < ncols; j++)
			{
				m(i, j) = mat(i, j);
			}
		}
		pout << "Matrix m=" << m << "\n";
	}
}
//end test functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#ifdef __FAST_EXUDYN_LINALG
PYBIND11_MODULE(exudynCPP, m) {
	m.doc() = "EXUDYN binding Python<->C++\n This is the 'fast' version without range/memory/whatsoever checks and uses /fp:fast compiler options!\n -> usage:\nSC=exu.SystemContainer()\nmbs=SC.AddSystem()\n see theDoc.pdf for tutorials, interface description and further information"; // module docstring
#else
PYBIND11_MODULE(exudynCPP, m) {
	m.doc() = "EXUDYN binding Python<->C++\n -> usage:\nSC=exu.SystemContainer()\nmbs=SC.AddSystem()\n see theDoc.pdf for tutorials, interface description and further information"; // module docstring
#endif
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//variables linked to exudyn
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	py::dict exudynVariables; //!< global dictionary which can be used by the user to store local variables
	py::dict exudynSystemVariables; //!< global dictionary which is used by system functions to store local variables

	m.def("GetVersionString", &PyGetVersionString, "get version as string as a function which is passed trough exudyn package");

	//m.def("PrintLF", &PyPrintLF, "this allows printing via exudyn with similar syntax as in python print(...) except for keyword arguments: print('test=',42); allows to redirect all output to file given by SetWriteToFile(...)");
	
	//m.def("PyThrowTest", &PyThrowTest, "PyThrowTest");

	//m.def("GetVector", &GetVector, "GetVector");
	//m.def("GetMatrix", &GetMatrix, "GetMatrix");
	//m.def("SeeMatrix", &SeeMatrix, "SeeMatrix");
	//m.def("Test", &PyTest, "internal test");
	//m.def("SetTestFunction2", &PySetTestFunction2, "Set the test function");
	//m.def("EvaluateTestFunction", &PyEvaluateTestFunction, "Evaluate test function");

#ifdef _MYDEBUG //only in debug mode!
	m.def("CreateTestSystem", &CreateTestSystem, "Create test system with systemNumber and optional arguments; will create the system depending on arg1 and arg2", py::arg("systemNumber")=0, py::arg("arg0") = 0, py::arg("arg1") = 0);
#endif

	py::enum_<AccessFunctionType>(m, "AccessFunctionType")
		.value("_None", AccessFunctionType::_None)
		.value("TranslationalVelocity_qt", AccessFunctionType::TranslationalVelocity_qt)
		.value("AngularVelocity_qt", AccessFunctionType::AngularVelocity_qt)
		.value("Coordinate_q", AccessFunctionType::Coordinate_q)
		.value("DisplacementLineIntegral_q", AccessFunctionType::DisplacementLineIntegral_q)
		.value("DisplacementSurfaceIntegral_q", AccessFunctionType::DisplacementSurfaceIntegral_q)
		.value("DisplacementVolumeIntegral_q", AccessFunctionType::DisplacementVolumeIntegral_q)
		.value("DisplacementMassIntegral_q", AccessFunctionType::DisplacementMassIntegral_q)
		.value("DisplacementSurfaceNormalIntegral_q", AccessFunctionType::DisplacementSurfaceNormalIntegral_q)
		//.value("Rotv1v2v3", AccessFunctionType::Rotv1v2v3_q)
		//.value("EndOfEnumList", AccessFunctionType::EndOfEnumList)
		.export_values();

	py::enum_<CObjectType>(m, "ObjectType")
		.value("_None", CObjectType::_None)
		.value("Ground", CObjectType::Ground)
		.value("Constraint", CObjectType::Constraint)
		.value("Connector", CObjectType::Connector)
		.value("Body", CObjectType::Body)
		.value("SingleNoded", CObjectType::SingleNoded)
		.value("MultiNoded", CObjectType::MultiNoded)
		.value("FiniteElement", CObjectType::FiniteElement)
		.value("SuperElement", CObjectType::SuperElement)
		.value("EndOfEnumList", CObjectType::EndOfEnumList)
		.export_values();

	py::enum_<Node::Type>(m, "NodeType")
		.value("_None", Node::_None)
		.value("Ground", Node::Ground)
		.value("Position2D", Node::Position2D)
		.value("Orientation2D", Node::Orientation2D)
		.value("Point2DSlope1", Node::Point2DSlope1)
		.value("Position", Node::Position)
		.value("Orientation", Node::Orientation)
		.value("RigidBody", Node::RigidBody)
		.value("RotationEulerParameters", Node::RotationEulerParameters)
		.value("RotationRxyz", Node::RotationRxyz)
		.value("RotationRotationVector", Node::RotationRotationVector)
		.value("RotationLieGroup", Node::RotationLieGroup)
		.value("GenericODE2", Node::GenericODE2)
		.value("GenericData", Node::GenericData)
		.export_values();

	//py::enum_<CNodeType>(m, "CNodeType")
	//	.value("_None", CNodeType::_None)
	//	.value("Point", CNodeType::Point)
	//	.value("RigidBody", CNodeType::RigidBody)
	//	.value("Temperature", CNodeType::Temperature)
	//	.value("General", CNodeType::General)
	//	.export_values();

	py::enum_<CNodeGroup>(m, "NodeGroup")
		.value("_None", CNodeGroup::_None)
		.value("ODE1variables", CNodeGroup::ODE1variables)
		.value("ODE2variables", CNodeGroup::ODE2variables)
		.value("AEvariables", CNodeGroup::AEvariables)
		.value("DataVariables", CNodeGroup::DataVariables)
		.export_values();

	//py::enum_<LinearSolverType>(m, "LinearSolverType")
	//	.value("_None", LinearSolverType::_None)
	//	.value("EXUdense", LinearSolverType::EXUdense)
	//	.value("EigenSparse", LinearSolverType::EigenSparse)
	//	.export_values();
	

	//dirty include: autogenerated pybind functions added here!
	#include "Autogenerated/Pybind_modules.h"

#ifdef PERFORM_UNIT_TESTS
	m.def("UnitTests", &RunUnitTests, "Run unit tests and return number of fails ...");
#endif
	//++++++++++++++++++++++++++++++++++++++++

	#include "Autogenerated/pybind_manual_classes.h"

	//+++++++++++++++++++++++++++++++++++++++++++
	//item indices:
	py::class_<NodeIndex>(m, "NodeIndex", "NodeIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &NodeIndex::index)
		.def("GetTypeString", &NodeIndex::GetTypeString,
			"get type string for identification in python")
		.def("GetIndex", &NodeIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &NodeIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const NodeIndex &item) {
		return item.GetIndex();
		}, "return the integer representation of the index")
		.def("__repr__", [](const NodeIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
		;

	py::class_<ObjectIndex>(m, "ObjectIndex", "ObjectIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &ObjectIndex::index)
		.def("GetTypeString", &ObjectIndex::GetTypeString,
			"get type string for identification in python")
		.def("GetIndex", &ObjectIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &ObjectIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const ObjectIndex &item) {
		return item.GetIndex();
		}, "return the integer representation of the index")
			.def("__repr__", [](const ObjectIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
			;

	py::class_<MarkerIndex>(m, "MarkerIndex", "MarkerIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &MarkerIndex::index)
		.def("GetTypeString", &MarkerIndex::GetTypeString,
			"get type string for identification in python")
		.def("GetIndex", &MarkerIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &MarkerIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const MarkerIndex &item) {
		return item.GetIndex();
		}, "return the integer representation of the index")
			.def("__repr__", [](const MarkerIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
		;

	py::class_<LoadIndex>(m, "LoadIndex", "LoadIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &LoadIndex::index)
		.def("GetTypeString", &LoadIndex::GetTypeString,
			"get type string for identification in python")
		.def("GetIndex", &LoadIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &LoadIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const LoadIndex &item) {
		return item.GetIndex();
		}, "return the integer representation of the index")
			.def("__repr__", [](const LoadIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
		;

	py::class_<SensorIndex>(m, "SensorIndex", "SensorIndex: index which may only be used for nodes")
		.def(py::init<>())
		.def(py::init<Index>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//private: .def_readwrite("index", &SensorIndex::index)
		.def("GetTypeString", &SensorIndex::GetTypeString,
			"get type string for identification in python")
		.def("GetIndex", &SensorIndex::GetIndex,
			"get index converted to index / int")
		.def("SetIndex", &SensorIndex::SetIndex,
			"set index with index / int")
		.def("__int__", [](const SensorIndex &item) {
		return item.GetIndex();
		}, "return the integer representation of the index")
		.def("__repr__", [](const SensorIndex &item) {
			return STDstring(EXUstd::ToString(item.GetIndex()));
		}, "return the string representation of the index, which can be, e.g., printed")
		;

	////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//NOT needed, should always convert to std::vector<NodeIndex>
	//py::class_<ArrayNodeIndex>(m, "ArrayNodeIndex", "ArrayNodeIndex: array of indices which may only be used for nodes")
	//	.def(py::init<>())
	//	.def(py::init<std::vector<Index>>())
	//	.def(py::init<std::vector<NodeIndex>>())
	//	//+++++++++++++++++++++++++++++++++++++++++++
	//	.def("GetTypeString", &ArrayNodeIndex::GetTypeString,
	//		"get type string for identification in python")
	//	.def("GetArrayIndex", &ArrayNodeIndex::GetArrayIndex,
	//		"get index converted to index / int")
	//	.def("SetArrayIndex", &ArrayNodeIndex::SetArrayIndex,
	//		"set index with index / int")
	//	//.def("__list__", [](const ArrayNodeIndex &item) {
	//	//	return item.GetArrayIndex();
	//	//}, "return the integer representation of the index array")
	//	.def("__repr__", [](const ArrayNodeIndex &item) {
	//		return STDstring(EXUstd::ToString(ArrayIndex(item.GetArrayIndex())));
	//	}, "return the string representation of the index array, which can be, e.g., printed")
	//	;


		//+++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++
		py::class_<PyMatrixContainer>(m, "MatrixContainer", "MatrixContainer: holds a dense or sparse matrix")
		.def(py::init<>())
		//+++++++++++++++++++++++++++++++++++++++++++
		.def("SetWithDenseMatrix", &PyMatrixContainer::SetWithDenseMatrix, 
			"set with dense numpy array; array (=matrix) contains values and size information", py::arg("pyArray"), py::arg("useDenseMatrix") = true)
		.def("SetWithSparseMatrixCSR", &PyMatrixContainer::SetWithSparseMatrixCSR, 
			"set with sparse CSR matrix format: numpy array contains in every row [row, col, value]; numberOfRows and numberOfColumns given extra", 
			py::arg("numberOfRowsInit"), py::arg("numberOfColumnsInit"), py::arg("pyArrayCSR"), py::arg("useDenseMatrix") = false)
		.def("GetPythonObject", &PyMatrixContainer::GetPythonObject, "convert MatrixContainer to numpy array (dense) or dictionary (sparse): containing #rows, #columns, numpy matrix with triplets")
		;
	//+++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++
	//+++++++++++++++++++++++++++++++++++++++++++
	//MAINSYSTEMCONTAINER
	py::class_<MainSystemContainer>(m, "SystemContainer", "SystemContainer: Contains a set of (multibody) systems")
		.def(py::init<>())
		//+++++++++++++++++++++++++++++++++++++++++++
		//System functions:
		.def("AddSystem", &MainSystemContainer::AddMainSystem, "add a new computational system", py::return_value_policy::reference)
		//.def("AddSystem", (Index(MainSystem::*)(STDstring)) &MainSystemContainer::AddMainSystem, "add a new computational system", py::return_value_policy::reference)
		.def("Reset", &MainSystemContainer::Reset, "delete all systems and reset SystemContainer (including graphics)") //WRONG?: , py::return_value_policy::reference)

		.def("NumberOfSystems", [](const MainSystemContainer& msc) {msc.GetMainSystems().NumberOfItems(); })

		.def("GetSystem", &MainSystemContainer::GetMainSystem, "Get main system i from system container")

		.def_property("visualizationSettings", &MainSystemContainer::PyGetVisualizationSettings, &MainSystemContainer::PySetVisualizationSettings)

		.def("WaitForRenderEngineStopFlag", &MainSystemContainer::WaitForRenderEngineStopFlag, "Wait for user to stop render engine (CTRL+Q)")

		.def("RenderEngineZoomAll", &MainSystemContainer::PyZoomAll, "Send zoom all signal, which will perform zoom all at next redraw request")
		
		.def("RedrawAndSaveImage", &MainSystemContainer::RedrawAndSaveImage, "Redraw openGL scene and save image (command waits until process is finished)")

		.def("GetRenderState", &MainSystemContainer::PyGetRenderState, "Get dictionary with current render state (openGL zoom, modelview, etc.)")

		.def("SetRenderState", &MainSystemContainer::PySetRenderState, "Set current render state (openGL zoom, modelview, etc.) with given dictionary; usually, this dictionary has been obtained with GetRenderState")

		//+++++++++++++++++++++++++++++++++++++++++++
		//Solver functions:
		.def("TimeIntegrationSolve", [](MainSystemContainer& msc, MainSystem& ms, STDstring solverName, const SimulationSettings& simulationSettings) {
		pout.precision(simulationSettings.outputPrecision);
		try
		{
			if (solverName == "RungeKutta1")
			{
				msc.GetSolvers().GetSolverRK1().SolveSystem(*(ms.GetCSystem()), simulationSettings);
			}
			else if (solverName == "GeneralizedAlphaOldSolver")
			{
				msc.GetSolvers().GetSolverGeneralizedAlpha().SolveSystem(*(ms.GetCSystem()), simulationSettings);
			}
			else if (solverName == "GeneralizedAlpha")
			{
				CSolverImplicitSecondOrderTimeInt solverSO;
				solverSO.SolveSystem(*(ms.GetCSystem()), simulationSettings);
			}
			else {
				PyError(STDstring("SystemContainer::TimeIntegrationSolve: invalid solverName '") + solverName + "'; options are: RungeKutta1 or GeneralizedAlpha");
			}
		}
		catch (const EXUexception& ex)
		{
			SysError("EXUDYN raised internal error in TimeIntegrationSolve:\n" + STDstring(ex.what()) + "\n");
		}
		catch (...) //any other exception
		{
			SysError("Unexpected exception during TimeIntegrationSolve!\n");
		}

		}, "Solve system in time with a specific time integrator")

		.def("StaticSolveOldSolver", [](MainSystemContainer& msc, MainSystem& ms, const SimulationSettings& simulationSettings) {
			pout.precision(simulationSettings.outputPrecision);

			msc.GetSolvers().GetSolverStatic().SolveSystem(*(ms.GetCSystem()), simulationSettings); // , jacobian);
		}, "OLD SOLVER: Statically solve system (linear/nonlinear)")

		.def("StaticSolve", [](MainSystemContainer& msc, MainSystem& ms, const SimulationSettings& simulationSettings) {
			pout.precision(simulationSettings.outputPrecision);

			try
			{
				CSolverStatic solverStatic;
				solverStatic.SolveSystem(*(ms.GetCSystem()), simulationSettings);
			}
			catch (const EXUexception& ex)
			{
				SysError("EXUDYN raised internal error in StaticSolve:\n" + STDstring(ex.what()) + "\n");
			}
			catch (...) //any other exception
			{
				SysError("Unexpected exception during StaticSolve!\n");
			}

		}, "Statically solve system (linear/nonlinear)")

		.def("__repr__", [](const MainSystemContainer &item) {
			STDstring str = "SystemContainer:\n";

			for (Index i = 0; i < item.GetMainSystems().NumberOfItems(); i++)
			{
				str += "System " + EXUstd::ToString(i) + " <systemData:\n";
				str += item.GetMainSystems()[i]->GetMainSystemData().PyInfoSummary() + ">\n";
			}
			return str;
		}, "return the representation of the systemContainer, which can be, e.g., printed")

		;

//does not work, use internal exudyn version:
//#ifdef VERSION_INFO
//		#pragma message( "Using exudyn version " VERSION_INFO )
//		m.attr("__version__") = VERSION_INFO;
//#else
	m.attr("__version__") = EXUstd::exudynVersion;
//#endif
}







