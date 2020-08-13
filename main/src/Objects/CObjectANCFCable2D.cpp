/** ***********************************************************************************************
* @brief        CObjectANCFCable2D implementation
*
* @author       Gerstmayr Johannes
* @date         2019-06-17 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Objects/CObjectANCFCable2DBase.h"
#include "Autogenerated/CObjectANCFCable2D.h"

//for CallFunction(...)
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainObjectANCFCable2D.h"

#include<ostream> //for autodiff
#include "../ngs-core-master/autodiff.hpp"

typedef ngstd::AutoDiff<16, Real> DReal16;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ANCFCable2D BASE class

//! get compressed shape function vector
//MATLAB: [(2 * x ^ 3) / L ^ 3 - (3 * x ^ 2) / L ^ 2 + 1, x - (2 * x ^ 2) / L + x ^ 3 / L ^ 2, (3 * x ^ 2) / L ^ 2 - (2 * x ^ 3) / L ^ 3, x ^ 3 / L ^ 2 - x ^ 2 / L]
Vector4D CObjectANCFCable2DBase::ComputeShapeFunctions(Real x, Real L)
{
	//Real L = GetLength();
	Real x0 = x / L;
	return Vector4D({ 1. - 3. * EXUstd::Square(x0) + 2. * EXUstd::Cube(x0),
					 x - 2. * EXUstd::Square(x) / L + x * EXUstd::Square(x0),
					 3. * EXUstd::Square(x0) - 2. * EXUstd::Cube(x0),
					 -x * (x0) + x * EXUstd::Square(x0) });
}

//! get derivative of compressed shape function vector for slopes
//MATLAB: [(6 * x ^ 2) / L ^ 3 - (6 * x) / L ^ 2, (3 * x ^ 2) / L ^ 2 - (4 * x) / L + 1, (6 * x) / L ^ 2 - (6 * x ^ 2) / L ^ 3, (3 * x ^ 2) / L ^ 2 - (2 * x) / L]
Vector4D CObjectANCFCable2DBase::ComputeShapeFunctions_x(Real x, Real L)
{
	//Real L = GetLength();
	Real L2 = L*L;
	Real L3 = L2*L;
	return Vector4D({ - 6. * x / L2+ 6. * EXUstd::Square(x)/L3,
					 1 - 4. * x / L + 3.* EXUstd::Square(x)/L2,
					 6. * x / L2 - 6. * EXUstd::Square(x)/L3,
					 -2.*x / L + 3.*EXUstd::Square(x)/L2 });
}

//! get second derivative of compressed shape function vector for slopes
//MATLAB: [(12 * x) / L ^ 3 - 6 / L ^ 2, (6 * x) / L ^ 2 - 4 / L, 6 / L ^ 2 - (12 * x) / L ^ 3, (6 * x) / L ^ 2 - 2 / L]
Vector4D CObjectANCFCable2DBase::ComputeShapeFunctions_xx(Real x, Real L)
{
	//Real L = GetLength();
	Real L2 = L * L;
	Real L3 = L2 * L;
	return Vector4D({ (12. * x) / L3 - 6. / L2, 
					 (6. * x) / L2 - 4. / L, 
					 6. / L2 - (12. * x) / L3,
					 (6. * x) / L2 - 2. / L });
}

//! map element coordinates (position or veloctiy level) given by nodal vectors q0 and q1 onto compressed shape function vector to compute position, etc.
Vector2D CObjectANCFCable2DBase::MapCoordinates(const Vector4D& SV, const LinkedDataVector& q0, const LinkedDataVector& q1)
{
	Vector2D v(0.);
	v[0] += SV[0] * q0[0];
	v[1] += SV[0] * q0[1];
	v[0] += SV[1] * q0[2];
	v[1] += SV[1] * q0[3];

	v[0] += SV[2] * q1[0];
	v[1] += SV[2] * q1[1];
	v[0] += SV[3] * q1[2];
	v[1] += SV[3] * q1[3];

	return v;
}

void CObjectANCFCable2DBase::ComputeCurrentNodeCoordinates(ConstSizeVector<4>& qNode0, ConstSizeVector<4>& qNode1) const
{
	qNode0.CopyFrom(((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector()); //displacement coordinates node 0
	qNode1.CopyFrom(((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector()); //displacement coordinates node 1

	qNode0 += ((CNodeODE2*)GetCNode(0))->GetReferenceCoordinateVector(); //reference coordinates + displacements
	qNode1 += ((CNodeODE2*)GetCNode(1))->GetReferenceCoordinateVector();
}

void CObjectANCFCable2DBase::ComputeCurrentNodeVelocities(ConstSizeVector<4>& qNode0, ConstSizeVector<4>& qNode1) const
{
	qNode0.CopyFrom(((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_t()); //velocity coordinates node 0
	qNode1.CopyFrom(((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector_t()); //velocity coordinates node 1
}

void CObjectANCFCable2DBase::ComputeCurrentObjectCoordinates(ConstSizeVector<8>& qANCF) const
{
	const int ns = 4; //number of shape functions
	LinkedDataVector qNode0(qANCF, 0, ns);		//link node values to element vector
	LinkedDataVector qNode1(qANCF, ns, ns);		//link node values to element vector

	qNode0 = ((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector(); //displacement coordinates node 0
	qNode1 = ((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector(); //displacement coordinates node 1

	qNode0 += ((CNodeODE2*)GetCNode(0))->GetReferenceCoordinateVector(); //reference coordinates + displacements
	qNode1 += ((CNodeODE2*)GetCNode(1))->GetReferenceCoordinateVector();
}

void CObjectANCFCable2DBase::ComputeCurrentObjectVelocities(ConstSizeVector<8>& qANCF_t) const
{
	const int ns = 4; //number of shape functions
	LinkedDataVector qNode0(qANCF_t, 0, ns);		//link node values to element vector
	LinkedDataVector qNode1(qANCF_t, ns, ns);		//link node values to element vector

	qNode0 = ((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_t(); //displacement coordinates node 0
	qNode1 = ((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector_t(); //displacement coordinates node 1
}

//! Computational function: compute mass matrix
void CObjectANCFCable2DBase::ComputeMassMatrix(Matrix& massMatrix) const
{
	if (massMatrixComputed)
	{
		massMatrix.CopyFrom(precomputedMassMatrix); //just assignement; ConstSizeMatrix is directly assigned to Matrix (no double copy)
	}
	else
	{
		massMatrix.SetScalarMatrix(nODE2Coordinates, 0.); //set 8x8 matrix
		Real L = GetLength();
		Real rhoA = GetMassPerLength();
		const Index ns = 4;   //number of shape functions

		Index cnt = 0;
		Real a = 0; //integration interval [a,b]
		Real b = L;
		for (auto item : EXUmath::gaussRuleOrder7Points)
		{
			Real x = 0.5*(b - a)*item + 0.5*(b + a);
			Vector4D SV = ComputeShapeFunctions(x, L);
			Vector4D SVint = SV;
			SVint *= rhoA * (0.5*(b - a)*EXUmath::gaussRuleOrder7Weights[cnt++]);

			for (Index i = 0; i < ns; i++)
			{
				for (Index j = 0; j < ns; j++)
				{
					massMatrix(i * 2, j * 2)		 += SV[i] * SVint[j];
					massMatrix(i * 2 + 1, j * 2 + 1) += SV[i] * SVint[j];
				}
			}
		}
		precomputedMassMatrix.CopyFrom(massMatrix); //assignement operator would cause double copy!
		massMatrixComputed = true;

	}

	//pout << "Mass=" << massMatrix << "\n";
		
}

//! Computational function: compute right-hand-side (RHS) of second order ordinary differential equations (ODE) to "ode2rhs"
void CObjectANCFCable2DBase::ComputeODE2RHS(Vector& ode2Rhs) const
{
	ode2Rhs.SetNumberOfItems(nODE2Coordinates);
	ode2Rhs.SetAll(0.);
	//compute work of elastic forces:

	const Index dim = 2;  //2D finite element
	const Index ns = 4;   //number of shape functions

	Real L = GetLength();
	Real EA, EI, axialStrain0, curvature0, bendingDamping, axialDamping;
	GetMaterialParameters(EI, EA, bendingDamping, axialDamping, axialStrain0, curvature0);

	Index cnt;
	Real a = 0; //integration interval [a,b]
	Real b = L;

	ConstSizeVector<ns> q0; 
	ConstSizeVector<ns> q1;
	ComputeCurrentNodeCoordinates(q0, q1);

	ConstSizeVector<ns> q0_t;
	ConstSizeVector<ns> q1_t;
	if (axialDamping != 0. || bendingDamping != 0.)
	{
		ComputeCurrentNodeVelocities(q0_t, q1_t);
	}


	ConstSizeVector<ns*dim> elasticForces;

	//numerical integration:
	//high accuracy: axialStrain = order9, curvature = order5
	//low accuracy : axialStrain = order7, curvature = order3 (lower order not possible, becomes unstable or very inaccurate ...

	const Index maxIntegrationPoints = 5;
	ConstSizeVector<maxIntegrationPoints> integrationPoints;
	ConstSizeVector<maxIntegrationPoints> integrationWeights;

	if (UseReducedOrderIntegration())
	{
		integrationPoints.CopyFrom(EXUmath::gaussRuleOrder7Points); //copy is slower, but cannot link to variable size ==> LinkedDataVector ...
		integrationWeights.CopyFrom(EXUmath::gaussRuleOrder7Weights);
	} else
	{
		integrationPoints.CopyFrom(EXUmath::gaussRuleOrder9Points); //copy is slower, but cannot link to variable size ==> LinkedDataVector ...
		integrationWeights.CopyFrom(EXUmath::gaussRuleOrder9Weights);
	}

	//axial strain:
	cnt = 0;
	for (auto item : integrationPoints)
	{
		Real x = 0.5*(b - a)*item + 0.5*(b + a);
		Vector4D SVx = ComputeShapeFunctions_x(x, L);
		Real integrationFactor = (0.5*(b - a)*integrationWeights[cnt++]);

		Vector2D rx = MapCoordinates(SVx, q0, q1);

		Real rxNorm2 = rx.GetL2NormSquared();
		Real rxNorm = sqrt(rxNorm2);
		Real axialStrain = rxNorm - 1.; // axial strain
		Real axialStrain_t = 0.; //rate of axial strain

		if (axialDamping != 0.)
		{
			Vector2D rx_t = MapCoordinates(SVx, q0_t, q1_t);
			axialStrain_t = (rx * rx_t) / rxNorm; //rate of axial strain
		}

		for (Index i = 0; i < dim; i++)
		{
			for (Index j = 0; j < ns; j++)
			{
				elasticForces[j*dim + i] = 1. / rxNorm * SVx[j] * rx[i];
			}
		}
		//elasticForces *= integrationFactor * GetParameters().physicsAxialStiffness * (axialStrain - GetParameters().physicsReferenceAxialStrain);
		elasticForces *= integrationFactor * (EA * (axialStrain - axialStrain0) + axialDamping * axialStrain_t);

		ode2Rhs += elasticForces;  //add to element elastic forces
	}

	//++++++++++++++++++++++++++++++
	//curvature:
	if (UseReducedOrderIntegration())
	{
		integrationPoints.CopyFrom(EXUmath::gaussRuleOrder3Points);
		integrationWeights.CopyFrom(EXUmath::gaussRuleOrder3Weights);
	}
	else
	{
		integrationPoints.CopyFrom(EXUmath::gaussRuleOrder5Points);
		integrationWeights.CopyFrom(EXUmath::gaussRuleOrder5Weights);
	}

	cnt = 0;
	for (auto item : integrationPoints)
	{
		Real x = 0.5*(b - a)*item + 0.5*(b + a);
		Vector4D SVx = ComputeShapeFunctions_x(x, L);
		Vector4D SVxx = ComputeShapeFunctions_xx(x, L);
		Real integrationFactor = (0.5*(b - a)*integrationWeights[cnt++]);

		Vector2D rx = MapCoordinates(SVx, q0, q1);
		Vector2D rxx = MapCoordinates(SVxx, q0, q1);

		Real rxNorm2 = rx.GetL2NormSquared();				//g
		//Real rxNorm = sqrt(rxNorm2);				
		Real rxCrossRxx = rx.CrossProduct2D(rxx);			//f
		Real curvature = rxCrossRxx / rxNorm2;				//kappa = (rx x rxx)/rx^2       //WRONG (Andreas/Matlab?): kappa = sqrt(cross2d(rp, rpp) ^ 2) / (rp'*rp);        // material measure of curvature

		Real inv2RxNorm2 = 1. / (rxNorm2*rxNorm2);			//g2inv
		Real tempF = 2. * rxCrossRxx*inv2RxNorm2;			//fn; f ... fraction numerator
		Real tempG = rxNorm2 * inv2RxNorm2;					//gn; g ... fraction denominator
		Real df;

		Real curvature_t = 0.; //rate of curvature
		if (bendingDamping != 0.)
		{
			Vector2D rx_t = MapCoordinates(SVx, q0_t, q1_t);
			Vector2D rxx_t = MapCoordinates(SVxx, q0_t, q1_t);

			Real rxCrossRxx_t = rx_t.CrossProduct2D(rxx) + rx.CrossProduct2D(rxx_t);	//f_t
			Real rxNorm2_t = 2.*(rx*rx_t);												//g_t

			curvature_t = (rxCrossRxx_t * rxNorm2 - rxCrossRxx * rxNorm2_t) / EXUstd::Square(rxNorm2); //rate of bending strain; (f_t*g - f*g_t)/g^2
		}

		for (Index i = 0; i < dim; i++)
		{
			for (Index j = 0; j < ns; j++)
			{
				switch (i) {
				case 0:
				{
					df = SVx[j]*rxx.Y() - SVxx[j]*rx.Y(); break; 
				}
				case 1:
				{
					df = -SVx[j]*rxx.X() + SVxx[j]*rx.X(); break; 
				}
				default:;
				}
				Real dg = rx[i]*SVx[j]; //derivative of denominator
				elasticForces[j*dim + i] = df * tempG - tempF * dg;
			}
		}
		//elasticForces *= integrationFactor * GetParameters().physicsBendingStiffness * (curvature - GetParameters().physicsReferenceCurvature);
		elasticForces *= integrationFactor * (EI * (curvature - curvature0) + bendingDamping * curvature_t );

		ode2Rhs += elasticForces;  //add to element elastic forces
	}


}


//! Computational function: compute right-hand-side (RHS) of second order ordinary differential equations (ODE) to "ode2rhs"
template<class TReal>
void CObjectANCFCable2DBase::ComputeODE2RHStemplate(VectorBase<TReal>& ode2Rhs, const ConstSizeVectorBase<TReal, nODE2Coordinates>& qANCF, const ConstSizeVectorBase<TReal, nODE2Coordinates>& qANCF_t) const
{
	ode2Rhs.SetNumberOfItems(nODE2Coordinates);
	ode2Rhs.SetAll(0.);
	//compute work of elastic forces:

	const Index dim = 2;  //2D finite element
	const Index ns = 4;   //number of shape functions

	Real L = GetLength();
	Real EA, EI, axialStrain0, curvature0, bendingDamping, axialDamping;
	GetMaterialParameters(EI, EA, bendingDamping, axialDamping, axialStrain0, curvature0);

	Index cnt;
	Real a = 0; //integration interval [a,b]
	Real b = L;


	ConstSizeVectorBase<TReal, ns*dim> elasticForces;

	//numerical integration:
	//high accuracy: axialStrain = order9, curvature = order5
	//low accuracy : axialStrain = order7, curvature = order3 (lower order not possible, becomes unstable or very inaccurate ...

	const Index maxIntegrationPoints = 5;
	ConstSizeVector<maxIntegrationPoints> integrationPoints;
	ConstSizeVector<maxIntegrationPoints> integrationWeights;

	if (UseReducedOrderIntegration())
	{
		integrationPoints.CopyFrom(EXUmath::gaussRuleOrder7Points); //copy is slower, but cannot link to variable size ==> LinkedDataVector ...
		integrationWeights.CopyFrom(EXUmath::gaussRuleOrder7Weights);
	}
	else
	{
		integrationPoints.CopyFrom(EXUmath::gaussRuleOrder9Points); //copy is slower, but cannot link to variable size ==> LinkedDataVector ...
		integrationWeights.CopyFrom(EXUmath::gaussRuleOrder9Weights);
	}

	//axial strain:
	cnt = 0;
	for (auto item : integrationPoints)
	{
		Real x = 0.5*(b - a)*item + 0.5*(b + a);
		Vector4D SVx = ComputeShapeFunctions_x(x, L);
		Real integrationFactor = (0.5*(b - a)*integrationWeights[cnt++]);

		SlimVectorBase<TReal, 2> rx = MapCoordinates<TReal>(SVx, qANCF);

		TReal rxNorm2 = rx.GetL2NormSquared();
		TReal rxNorm = sqrt(rxNorm2);
		TReal axialStrain = rxNorm - 1.; // axial strain
		TReal axialStrain_t = 0.; //rate of axial strain

		if (axialDamping != 0.)
		{
			SlimVectorBase<TReal, 2> rx_t = MapCoordinates<TReal>(SVx, qANCF_t);
			axialStrain_t = (rx * rx_t) / rxNorm; //rate of axial strain
		}

		for (Index i = 0; i < dim; i++)
		{
			for (Index j = 0; j < ns; j++)
			{
				elasticForces[j*dim + i] = 1. / rxNorm * SVx[j] * rx[i];
			}
		}
		//elasticForces *= integrationFactor * GetParameters().physicsAxialStiffness * (axialStrain - GetParameters().physicsReferenceAxialStrain);
		elasticForces *= integrationFactor * (EA * (axialStrain - axialStrain0) + axialDamping * axialStrain_t);

		ode2Rhs += elasticForces;  //add to element elastic forces
	}

	//++++++++++++++++++++++++++++++
	//curvature:
	if (UseReducedOrderIntegration())
	{
		integrationPoints.CopyFrom(EXUmath::gaussRuleOrder3Points);
		integrationWeights.CopyFrom(EXUmath::gaussRuleOrder3Weights);
	}
	else
	{
		integrationPoints.CopyFrom(EXUmath::gaussRuleOrder5Points);
		integrationWeights.CopyFrom(EXUmath::gaussRuleOrder5Weights);
	}

	cnt = 0;
	for (auto item : integrationPoints)
	{
		Real x = 0.5*(b - a)*item + 0.5*(b + a);
		Vector4D SVx = ComputeShapeFunctions_x(x, L);
		Vector4D SVxx = ComputeShapeFunctions_xx(x, L);
		Real integrationFactor = (0.5*(b - a)*integrationWeights[cnt++]);

		SlimVectorBase<TReal, 2> rx = MapCoordinates<TReal>(SVx, qANCF);
		SlimVectorBase<TReal, 2> rxx = MapCoordinates<TReal>(SVxx, qANCF);

		TReal rxNorm2 = rx.GetL2NormSquared();				//g
		//TReal rxNorm = sqrt(rxNorm2);
		TReal rxCrossRxx = rx.CrossProduct2D(rxx);			//f
		TReal curvature = rxCrossRxx / rxNorm2;				//kappa = (rx x rxx)/rx^2       //WRONG (Andreas/Matlab?): kappa = sqrt(cross2d(rp, rpp) ^ 2) / (rp'*rp);        // material measure of curvature

		TReal inv2RxNorm2 = 1. / (rxNorm2*rxNorm2);			//g2inv
		TReal tempF = 2. * rxCrossRxx*inv2RxNorm2;			//fn; f ... fraction numerator
		TReal tempG = rxNorm2 * inv2RxNorm2;					//gn; g ... fraction denominator
		TReal df;

		TReal curvature_t = 0.; //rate of curvature
		if (bendingDamping != 0.)
		{
			SlimVectorBase<TReal, 2> rx_t = MapCoordinates<TReal>(SVx, qANCF_t);
			SlimVectorBase<TReal, 2> rxx_t = MapCoordinates<TReal>(SVxx, qANCF_t);

			TReal rxCrossRxx_t = rx_t.CrossProduct2D(rxx) + rx.CrossProduct2D(rxx_t);	//f_t
			TReal rxNorm2_t = 2.*(rx*rx_t);												//g_t

			curvature_t = (rxCrossRxx_t * rxNorm2 - rxCrossRxx * rxNorm2_t) / EXUstd::Square(rxNorm2); //rate of bending strain; (f_t*g - f*g_t)/g^2
		}

		for (Index i = 0; i < dim; i++)
		{
			for (Index j = 0; j < ns; j++)
			{
				switch (i) {
				case 0:
				{
					df = SVx[j] * rxx.Y() - SVxx[j] * rx.Y(); break;
				}
				case 1:
				{
					df = -SVx[j] * rxx.X() + SVxx[j] * rx.X(); break;
				}
				default:;
				}
				TReal dg = rx[i] * SVx[j]; //derivative of denominator
				elasticForces[j*dim + i] = df * tempG - tempF * dg;
			}
		}
		//elasticForces *= integrationFactor * GetParameters().physicsBendingStiffness * (curvature - GetParameters().physicsReferenceCurvature);
		elasticForces *= integrationFactor * (EI * (curvature - curvature0) + bendingDamping * curvature_t);

		ode2Rhs += elasticForces;  //add to element elastic forces
	}


}


void CObjectANCFCable2DBase::ComputeJacobianODE2_ODE2(ResizableMatrix& jacobian, ResizableMatrix& jacobian_ODE2_t) const
{
	const Index ns = 4;   //number of shape functions
	ConstSizeVector<2*ns> qANCF0;
	ConstSizeVector<2*ns> qANCF0_t;
	ConstSizeVectorBase<DReal16, 2*ns> qANCF;
	ConstSizeVectorBase<DReal16, 2*ns> qANCF_t;
	ComputeCurrentObjectCoordinates(qANCF0);
	ComputeCurrentObjectVelocities(qANCF0_t);
	for (Index i = 0; i < 2 * ns; i++)
	{
		qANCF[i] = qANCF0[i];
		qANCF_t[i] = qANCF0_t[i];
		qANCF[i].DValue((int)i) = 1; //mark that this is the corresponding derivative
		qANCF_t[i].DValue((int)(i+2*ns)) = 1; //mark that this is the corresponding derivative; velocity derivatives are in second block
	}
	ConstSizeVectorBase<DReal16, 2 * ns> ode2RHS;

	ComputeODE2RHStemplate<DReal16>(ode2RHS, qANCF, qANCF_t);
	//now copy autodifferentiated result:
	for (Index i = 0; i < 2 * ns; i++)
	{
		for (Index j = 0; j < 2 * ns; j++)
		{
			jacobian(i, j) = ode2RHS[i].DValue((int)j);
			jacobian_ODE2_t(i, j) = ode2RHS[i].DValue((int)(j+2*ns));
		}
	}
}


//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectANCFCable2DBase::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + (Index)AccessFunctionType::AngularVelocity_qt + (Index)AccessFunctionType::DisplacementMassIntegral_q);
}

//! Flags to determine, which output variables are available (displacment, velocity, stress, ...)
OutputVariableType CObjectANCFCable2DBase::GetOutputVariableTypes() const
{
	return (OutputVariableType)((Index)OutputVariableType::Position + (Index)OutputVariableType::Velocity);
}

//! provide Jacobian at localPosition in "value" according to object access
void CObjectANCFCable2DBase::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	Real L = GetLength();

	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt:
	{
		//const Index dim = 2;  //2D finite element
		//const Index ns = 4;   //number of shape functions

		Real x = localPosition[0]; //only x-coordinate
		Vector4D SV = ComputeShapeFunctions(x, L);
		value.SetNumberOfRowsAndColumns(3, 8); //3D velocity, 8 coordinates qt
		//pout << "inside ..." << localPosition << "\n";

		if (localPosition[1] == 0)
		{
			value.SetAll(0.);
			value(0, 0) = SV[0];
			value(1, 1) = SV[0];
			value(0, 2) = SV[1];
			value(1, 3) = SV[1];
			value(0, 4) = SV[2];
			value(1, 5) = SV[2];
			value(0, 6) = SV[3];
			value(1, 7) = SV[3];
		}
		else
		{
			//pout << "inside ..." << "\n";
			//value.SetAll(0.);
			Real y = localPosition[1];
			Vector4D SV_x = ComputeShapeFunctions_x(x, L);
			Vector2D r_x = ComputeSlopeVector(x, ConfigurationType::Current);
			Real norm = r_x.GetL2Norm();
			Real normInv = 0;
			Vector2D n({ -r_x[1], r_x[0] });
			if (norm != 0.)
			{
				normInv = 1. / norm;
			}
			else
			{
				CHECKandTHROWstring("CObjectANCFCable2DBase::GetPosition(...): slope vector has length 0!");
			}
			n *= normInv;
			//p = r(localPosition[0]) + localPosition[1] * n; n=1/sqrt(rx^T*rx)*[-rx[1],rx[0]]
			//dp/dq = S + (ry^T*S_x)/(rx^T*rx) (3/2) * n + 1/sqrt(rx^T*rx) * S_x^perpendicular
			Real norm3 = norm * norm * norm; //could be SIMPLIFIED, because n also contains 1/norm ....

			//pout << "  slope=" << r_x << ", norm=" << norm << ", n=" << n << "\n";
			//
			for (Index i = 0; i < 4; i++)
			{
				Vector2D Svec[2]; //SV_x
				Svec[0] = Vector2D({ SV_x[i],0 });
				Svec[1] = Vector2D({ 0, SV_x[i] });
				Vector2D SvecP[2];
				SvecP[0] = Vector2D({ 0., SV_x[i]});
				SvecP[1] = Vector2D({ -SV_x[i], 0. });

				for (Index j = 0; j < 2; j++)
				{
					Real u = -y*(r_x*Svec[j]) / norm3;
					value(0, i * 2 + j) = u * n[0] + y*normInv * SvecP[j][0];
					value(1, i * 2 + j) = u * n[1] + y*normInv * SvecP[j][1];

					value(j, i * 2 + j) += SV[i];
				}

			}
		}

		break;
	}
	case AccessFunctionType::AngularVelocity_qt:
	{
		//const Index dim = 2;  //2D finite element
		const Index ns = 4;   //number of shape functions

		Real xLoc = localPosition[0]; //only x-coordinate
		Vector2D slope = ComputeSlopeVector(xLoc, ConfigurationType::Current);
		Real x = slope[0]; //x-slopex
		Real y = slope[1]; //y-slopex

		Vector4D SVx = ComputeShapeFunctions_x(xLoc, L);
		Real fact0 = -y / (x*x + y * y);
		Real fact1 = x / (x*x + y * y);

		value.SetNumberOfRowsAndColumns(3, 8);
		value.SetAll(0.); //last row not necessary to set to zero ... 
		for (Index i = 0; i < ns; i++)
		{
			value(2, i*2) = SVx[i] * fact0; //last row of jacobian
			value(2, i*2 + 1) = SVx[i] * fact1;
		}

		break;
	}
	case AccessFunctionType::DisplacementMassIntegral_q:
	{
		//const Index dim = 2;  //2D finite element
		//const Index ns = 4;   //number of shape functions

		value.SetNumberOfRowsAndColumns(3, 8); //3D velocity, 8 coordinates qt
		value.SetAll(0.);

		Real L = GetLength();
		Real rhoA = GetMassPerLength();

		Index cnt = 0;
		Real a = 0; //integration interval [a,b]
		Real b = L;

		Vector4D SV({0.,0.,0.,0.});

		for (auto item : EXUmath::gaussRuleOrder3Points)
		{
			Real x = 0.5*(b - a)*item + 0.5*(b + a);
			Vector4D SVloc = ComputeShapeFunctions(x, L);
			SVloc *= rhoA * (0.5*(b - a)*EXUmath::gaussRuleOrder3Weights[cnt++]);
			SV += SVloc;
		}

		value(0, 0) = SV[0];
		value(1, 1) = SV[0];
		value(0, 2) = SV[1];
		value(1, 3) = SV[1];
		value(0, 4) = SV[2];
		value(1, 5) = SV[2];
		value(0, 6) = SV[3];
		value(1, 7) = SV[3];
		break;
	}
	default:
		SysError("CObjectANCFCable2D:GetAccessFunctionBody illegal accessType");
	}
}

//! provide according output variable in "value"
void CObjectANCFCable2DBase::GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value) const
{
	//outputVariables = "{
	//'Position':'global position vector of local axis (1) and cross section (2) position', 
	//'Velocity':'global velocity vector of local axis (1) and cross section (2) position', 
	//'Director1':'(axial) slope vector of local axis position', 
	//'Strain':'axial strain (scalar)', 
	//'Curvature':'axial strain (scalar)', 
	//'Force':'(local) section normal force (scalar)', 
	//'Torque':'(local) bending moment (scalar)'}"
	switch (variableType)
	{
	case OutputVariableType::Position:	value.CopyFrom(GetPosition(localPosition, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetPosition(localPosition, configuration) - GetPosition(localPosition, ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity:	value.CopyFrom(GetVelocity(localPosition, configuration)); break;
	case OutputVariableType::Director1: {
		Vector2D rx = ComputeSlopeVector(localPosition[0], configuration);
		value.SetVector({rx[0], rx[1], 0.});
		break; }
	case OutputVariableType::Strain:	value.SetVector({ ComputeAxialStrain(localPosition[0], configuration) }); break;
	case OutputVariableType::Curvature:	value.SetVector({ ComputeCurvature(localPosition[0], configuration) }); break;
	case OutputVariableType::Force: {
		Real physicsBendingStiffness, physicsAxialStiffness, bendingDamping, axialDamping, physicsReferenceAxialStrain, physicsReferenceCurvature;
		GetMaterialParameters(physicsBendingStiffness, physicsAxialStiffness, bendingDamping, axialDamping, physicsReferenceAxialStrain, physicsReferenceCurvature);
		
		Real force = physicsAxialStiffness * (ComputeAxialStrain(localPosition[0], configuration) - physicsReferenceAxialStrain);
		if (axialDamping != 0) { force += axialDamping * ComputeAxialStrain_t(localPosition[0], configuration); }

		value.SetVector({ force }); break;
	}
	case OutputVariableType::Torque: {
		Real physicsBendingStiffness, physicsAxialStiffness, physicsReferenceAxialStrain, physicsReferenceCurvature, bendingDamping, axialDamping;
		GetMaterialParameters(physicsBendingStiffness, physicsAxialStiffness, bendingDamping, axialDamping, physicsReferenceAxialStrain, physicsReferenceCurvature);
		
		Real torque = physicsBendingStiffness * (ComputeCurvature(localPosition[0], configuration) - physicsReferenceCurvature);
		if (bendingDamping != 0) { torque += bendingDamping * ComputeCurvature_t(localPosition[0], configuration); }
		value.SetVector({ torque }); break;
	}
	default:
		SysError("CObjectANCFCable2D::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectANCFCable2DBase::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector4D SV = ComputeShapeFunctions(x, GetLength());
	
	Vector2D v = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference)
	{
		v += MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}
	//include off-axis position:
	if (localPosition[1] != 0.)
	{
		Vector2D r_x = ComputeSlopeVector(x, configuration);
		Real norm = r_x.GetL2Norm();
		if (norm != 0.)
		{
			r_x *= 1. / norm;
			Vector2D n({ -r_x[1], r_x[0] });
			v += localPosition[1] * n;
		} else
		{ CHECKandTHROWstring("CObjectANCFCable2DBase::GetPosition(...): slope vector has length 0!"); }

	}

	return Vector3D({v[0],v[1],0.});

}

//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectANCFCable2DBase::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector4D SV = ComputeShapeFunctions(x, GetLength());

	Vector2D v = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector_t(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector_t(configuration));
	//Vector2D v = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_t(), ((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector_t());

	return Vector3D({ v[0],v[1],0. });
}

//! return the (global) position of "localPosition" according to configuration type
Vector3D CObjectANCFCable2DBase::GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration) const
{
	Real x = localPosition[0]; //only x-coordinate
	Vector4D SV = ComputeShapeFunctions(x, GetLength());

	Vector2D v(0.);
	if (configuration != ConfigurationType::Reference)
	{
		v = MapCoordinates(SV, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	}
	return Vector3D({ v[0],v[1],0. });
}

Matrix3D CObjectANCFCable2DBase::GetRotationMatrix(const Vector3D& localPosition, ConfigurationType configuration) const
{
	//return ((CNodeODE2*)GetCNode(0))->GetRotationMatrix(configuration);

	//compute slope vector:
	Real x = localPosition[0]; //only x-coordinate
	Vector2D vRef = ComputeSlopeVector(x, configuration);

	//compute angle from slope vector
	Real phi = atan2(vRef[1], vRef[0]); //slope vector [0,1] is defined as zero angle
	return Matrix3D(3, 3, { cos(phi),-sin(phi),0.,  sin(phi),cos(phi),0., 0.,0.,1. });
}

//! AUTO:  return configuration dependent velocity of node; returns always a 3D Vector
Vector3D CObjectANCFCable2DBase::GetAngularVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	CHECKandTHROW(configuration == ConfigurationType::Current, "CObjectANCFCable2D::GetAngularVelocity: call only possible for current configuration");
	//return ((CNodeODE2*)GetCNode(0))->GetAngularVelocity(configuration);

	//for details see GetAngularVelocity in Point2DSlope1

	Real xLoc = localPosition[0]; //only x-coordinate
	Vector2D slope = ComputeSlopeVector(xLoc, configuration);
	Real x = slope[0]; //x-slopex
	Real y = slope[1]; //y-slopex

	Vector4D SVx = ComputeShapeFunctions_x(xLoc, GetLength());
	Vector2D slope_t = MapCoordinates(SVx, ((CNodeODE2*)GetCNode(0))->GetCurrentCoordinateVector_t(), ((CNodeODE2*)GetCNode(1))->GetCurrentCoordinateVector_t());

	//compare this function to GetRotationMatrix(...)
	return Vector3D({ 0., 0., (-y * slope_t[0] + x * slope_t[1]) / (x*x + y * y) });
}

Vector2D CObjectANCFCable2DBase::ComputeSlopeVector(Real x, ConfigurationType configuration) const
{
	Vector4D SVx = ComputeShapeFunctions_x(x, GetLength());

	Vector2D slope = MapCoordinates(SVx, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference) //add reference configuration to any current, initial, visualization coordinates (except reference configuration!)
	{
		slope += MapCoordinates(SVx, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}

	return slope;

}

Vector2D CObjectANCFCable2DBase::ComputeSlopeVector_x(Real x, ConfigurationType configuration) const
{
	Vector4D SVxx = ComputeShapeFunctions_xx(x, GetLength());

	Vector2D slope_x = MapCoordinates(SVxx, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(configuration));
	if (configuration != ConfigurationType::Reference) //add reference configuration to any current, initial, visualization coordinates (except reference configuration!)
	{
		slope_x += MapCoordinates(SVxx, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Reference), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector(ConfigurationType::Reference));
	}

	return slope_x;

}

//!  compute the axial strain at a certain axial position, for given configuration
Real CObjectANCFCable2DBase::ComputeAxialStrain(Real x, ConfigurationType configuration) const 
{
	Vector2D rx = ComputeSlopeVector(x, configuration);

	Real rxNorm2 = rx.GetL2NormSquared();
	Real rxNorm = sqrt(rxNorm2);
	return rxNorm - 1.; // axial strain
}

//!  compute the (bending) curvature at a certain axial position, for given configuration
Real CObjectANCFCable2DBase::ComputeCurvature(Real x, ConfigurationType configuration) const
{
	Vector2D rx = ComputeSlopeVector(x, configuration);
	Vector2D rxx = ComputeSlopeVector_x(x, configuration);

	Real rxNorm2 = rx.GetL2NormSquared(); //computation see ComputeODE2RHS(...)
	//Real rxNorm = sqrt(rxNorm2);
	Real rxCrossRxx = rx.CrossProduct2D(rxx);
	return rxCrossRxx / rxNorm2; //curvature
}

Vector2D CObjectANCFCable2DBase::ComputeSlopeVector_t(Real x, ConfigurationType configuration) const
{
	Vector4D SVx = ComputeShapeFunctions_x(x, GetLength());

	return MapCoordinates(SVx, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector_t(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector_t(configuration));
}

Vector2D CObjectANCFCable2DBase::ComputeSlopeVector_xt(Real x, ConfigurationType configuration) const
{
	Vector4D SVxx = ComputeShapeFunctions_xx(x, GetLength());

	return MapCoordinates(SVxx, ((CNodeODE2*)GetCNode(0))->GetCoordinateVector_t(configuration), ((CNodeODE2*)GetCNode(1))->GetCoordinateVector_t(configuration));
}

//!  compute the axial strain at a certain axial position, for given configuration
Real CObjectANCFCable2DBase::ComputeAxialStrain_t(Real x, ConfigurationType configuration) const
{
	Vector2D rx = ComputeSlopeVector(x, configuration);
	Vector2D rx_t = ComputeSlopeVector_t(x, configuration);
	Real rxNorm2 = rx.GetL2NormSquared();
	Real rxNorm = sqrt(rxNorm2);

	return (rx * rx_t) / rxNorm; //rate of axial strain
}


//!  compute the (bending) curvature at a certain axial position, for given configuration
Real CObjectANCFCable2DBase::ComputeCurvature_t(Real x, ConfigurationType configuration) const
{
	Vector2D rx = ComputeSlopeVector(x, configuration);
	Vector2D rxx = ComputeSlopeVector_x(x, configuration);

	Vector2D rx_t = ComputeSlopeVector_t(x, configuration);
	Vector2D rxx_t = ComputeSlopeVector_xt(x, configuration);

	//apply differentiation formula: (f/g)' = (f'g - fg') / g^2
	Real g = rx.GetL2NormSquared();				//g
	Real f = rx.CrossProduct2D(rxx);			//f
	Real f_t = rx_t.CrossProduct2D(rxx) + rx.CrossProduct2D(rxx_t);
	Real g_t = 2 * (rx_t * rx);

	return (f_t*g - f * g_t) / EXUstd::Square(g);
}








//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ANCFCable2D MAIN part
py::object MainObjectANCFCable2D::CallFunction(STDstring functionName, py::dict args) const
{
	PyError(STDstring("MainObjectMassPoint::CallFunction called with invalid functionName '" + functionName + "'"));
	return py::int_(EXUstd::InvalidIndex);
}

bool MainObjectANCFCable2D::CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const
{
	CObjectANCFCable2D* cObject = (CObjectANCFCable2D*)GetCObject();

	for (Index i = 0; i < 2; i++) {
		if (std::strcmp(mainSystem.GetMainSystemData().GetMainNode(cObject->GetNodeNumber(i)).GetTypeName(), "Point2DSlope1") != 0)
		{
			errorString = "ObjectANCFCable2D: node " + EXUstd::ToString(i) + " must be of type 'Point2DSlope1'";
			return false;
		}
	}
	return true;
}




