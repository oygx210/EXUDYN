/** ***********************************************************************************************
* @brief		Includes for basic linalg: Vectors(Slim-/Const-/Resizable-/LinkedData), Matrizes (Const-/Resizable-/LinkedData) and Arrays; 
*				also defines advanced templates for matrix/vector operations
*
* @author		Gerstmayr Johannes
* @date			1997-05-15 (generated)
* @date			2019-05-15 (last modified)
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @todo         Vector::operators, check if data[cnt++] works and is faster
*
************************************************************************************************ */

#pragma once

//BasicLinalg provides consistent includes for BasicDefinitions, arrays, vectors and matrices
#include <initializer_list> //for initializer_list in constructor
#include <ostream>
#include <stdlib.h> // for initialization with std::vector
#include <vector>
#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h" //defines Real
#include "Utilities/BasicFunctions.h"

#include "Utilities/ResizableArray.h"	//includes SlimArray.h and BasicFunctions.h
#include "Linalg/LinkedDataVector.h"	//includes SlimVector.h and Vector.h
#include "Linalg/ResizableVector.h"	//includes SlimVector.h and Vector.h
#include "Linalg/ConstSizeVector.h"	
#include "Linalg/ConstSizeMatrix.h"		//includes Matrix.h
#include "Linalg/ResizableMatrix.h"	

typedef ConstSizeMatrix<9> Matrix3D; //will be changed to SlimMatrix<...>
typedef ConstSizeMatrix<4> Matrix2D; //will be changed to SlimMatrix<...>

typedef ConstSizeMatrixBase<float,9> Matrix3DF; //will be changed to SlimMatrix<...>
typedef ConstSizeMatrixBase<float,4> Matrix2DF; //will be changed to SlimMatrix<...>

//put this into special .h file:

namespace EXUmath {

	inline static const Matrix3DF unitMatrix3DF(3, 3, { 1.f,0.f,0.f, 0.f,1.f,0.f, 0.f,0.f,1.f });
	inline static const Matrix3D unitMatrix3D(3, 3, { 1.,0.,0., 0.,1.,0., 0.,0.,1. });

	//convert 3x3 Matrix to std::array<std::array<Real, matrixColumns>, matrixRows>; used mainly for pybind conversion
	template<Index rows, Index columns, class T>
	inline std::array<std::array<T, rows>, columns> MatrixToStdArrayTemplate(const MatrixBase<T>& matrix)
	{
		std::array<std::array<T, columns>, rows> stdarray;
		for (Index i = 0; i < rows; i++) {
			for (Index j = 0; j < columns; j++)
			{
				stdarray[i][j] = matrix(i, j);
			}
		}
		return stdarray;
	}

	inline std::array<std::array<Real, 3>, 3> MatrixToStdArray33(const Matrix& matrix) {
		return MatrixToStdArrayTemplate<3, 3, Real>(matrix);
	}

	inline std::array<std::array<float, 3>, 3> Matrix3DFToStdArray33(const Matrix3DF& matrix) {
		return MatrixToStdArrayTemplate<3, 3, float>(matrix);
	}

	//convert Vector to std::array<std::array<Real, matrixColumns>, matrixRows>; used mainly for pybind conversion
	template<Index rows, Index columns, class T>
	inline std::array<std::array<T, rows>, columns> VectorToStdArrayArrayTemplate(const SlimVectorBase<T,(Index)(rows*columns)>& vector)
	{
		std::array<std::array<T, columns>, rows> stdarray;
		for (Index i = 0; i < rows; i++) {
			for (Index j = 0; j < columns; j++)
			{
				stdarray[i][j] = vector[i*columns + j];
			}
		}
		return stdarray;
	}

	//inline std::array<std::array<Real, 3>, 3> SlimVector9ToStdArray33(const SlimVector<9>& vector) {
	//	return VectorToStdArrayArrayTemplate<3, 3, Real>(vector);
	//}
	inline std::array<std::array<float, 3>, 3> SlimVectorF9ToStdArray33F(const Float9& vector) {
		return VectorToStdArrayArrayTemplate<3, 3, float>(vector);
	}
	inline std::array<std::array<float, 4>, 4> SlimVectorF16ToStdArray44F(const Float16& vector) {
		return VectorToStdArrayArrayTemplate<4, 4, float>(vector);
	}


	//! apply a 3D rigid-body transformation with a source vector, a destination vector (may be same) and a rotation matrix; transformedPosition = positionOffset + rotation*localPosition
	template<class TMatrix, class TVector>
	inline void RigidBodyTransformation(const TMatrix& rotation, const TVector& positionOffset, const TVector& localPosition, TVector& transformedPosition)
	{
		CHECKandTHROW(rotation.NumberOfRows() == 3 && rotation.NumberOfColumns() == 3 
			&& positionOffset.NumberOfItems() == 3 && localPosition.NumberOfItems() == 3 && transformedPosition.NumberOfItems() == 3, "RigidBodyTransformation: index mismatch");
		
		TVector temp;
		MultMatrixVectorTemplate<TMatrix, TVector, TVector>(rotation, localPosition, temp);
		transformedPosition = positionOffset;
		transformedPosition += temp;
	}

	//! compute orthogonal basis vectors (normal1, normal2) for given vector0 (non-unique solution!); if vector0 == [0,0,0], then any normal basis is returned
	inline void ComputeOrthogonalBasis(Vector3D vector0, Vector3D& normal1, Vector3D& normal2)
	{
		Real L0 = vector0.GetL2Norm();
		if (L0 == 0.) { normal1.SetVector({ 1.,0.,0. }); normal2.SetVector({ 0.,1.,0. }); } //any solution will suffice
		vector0 *= 1. / L0;

		if (fabs(vector0[0]) > 0.5 && fabs(vector0[1]) < 0.1 && fabs(vector0[2]) < 0.1) { normal1.SetVector({ 0., 1., 0. }); }
		else { normal1.SetVector({ 1., 0., 0. }); }

		Real h = normal1 * vector0;
		normal1 -= h * vector0;
		normal1.Normalize();
		normal2 = vector0.CrossProduct(normal1);
	}

	//! Project normal into normal plane of vector (Gram-Schmidt orthogonalization); works for any vector typehaving scalar*vector and vector*vector operator
	template<class TVector>
	inline void GramSchmidtOrthogonalization(const TVector vector, TVector& normal)
	{
		Real h = (normal * vector) / (vector*vector);
		normal -= h * vector;
	}

	//! Use GramSchmidtOrthogonalization(...) and normalize the resulting 'normal' vector; vector must have a .Normalization() function
	template<class TVector>
	inline void GramSchmidtOrthogonalizationNormalized(const TVector vector, TVector& normal)
	{
		GramSchmidtOrthogonalization(vector, normal);
		normal.Normalize();
	}



	//numerical integration in interval [-1,1]; int(1) = 2
	static const SlimVector<1> gaussRuleOrder1Points({ 0. });
	static const SlimVector<1> gaussRuleOrder1Weights({ 2. });
	static const SlimVector<2> gaussRuleOrder3Points({ -sqrt(1. / 3.), sqrt(1. / 3.) });
	static const SlimVector<2> gaussRuleOrder3Weights({ 1., 1. });
	static const SlimVector<3> gaussRuleOrder5Points({ -sqrt(3. / 5.), 0., sqrt(3. / 5.) });
	static const SlimVector<3> gaussRuleOrder5Weights({ 5. / 9., 8. / 9., 5. / 9. });
	static const SlimVector<4> gaussRuleOrder7Points({ -sqrt(3. / 7. + sqrt(120.) / 35.), -sqrt(3. / 7. - sqrt(120.) / 35.), sqrt(3. / 7. - sqrt(120.) / 35.), sqrt(3. / 7. + sqrt(120.) / 35.) });
	static const SlimVector<4> gaussRuleOrder7Weights({ 1. / 2. - 5. / (3.*sqrt(120.)), 1. / 2. + 5. / (3.*sqrt(120.)), 1. / 2. + 5. / (3.*sqrt(120.)), 1. / 2. - 5. / (3.*sqrt(120.)) });
	static const SlimVector<5> gaussRuleOrder9Points({ -0.906179845938664, -0.5384693101056831, 0., 0.5384693101056831, 0.906179845938664 });
	static const SlimVector<5> gaussRuleOrder9Weights({ 0.23692688505618914, 0.47862867049936636, 0.5688888888888889, 0.47862867049936636, 0.23692688505618914 });

	//numerically integrate a function in interval [a,b]
	inline auto NumIntegrate = [](Real(*function)(Real), auto& points, auto& weights, Real a, Real b)
	{ 
		Index cnt = 0; Real value = 0.;  
		for (auto item : points) { Real x = 0.5*(b - a)*item + 0.5*(b + a); value += 0.5*(b - a)*weights[cnt++]*function(x); }
	};

	//auto function = [](Real x) {return x*x; }; //define a function 

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//ResizableMatrix/ResizableVector
	inline void MultMatrixVector(const ResizableMatrix& matrix, Vector3D& vector, ResizableVector& result) {
		MultMatrixVectorTemplate<ResizableMatrix, Vector3D, ResizableVector>(matrix, vector, result);
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Matrix x Vector specialization for different situations:
	inline void MultMatrixVector(const Matrix& matrix, const Vector& vector, Vector& result) {
		MultMatrixVectorTemplate<Matrix, Vector, Vector>(matrix, vector, result);
	}

	inline void MultMatrixVector(const Matrix& matrix, const SlimVector<3>& vector, Vector& result) {
		MultMatrixVectorTemplate<Matrix, SlimVector<3>, Vector>(matrix, vector, result);
	}

	//result as SlimVector not possible (SetNumberOfItems not implemented)
	//inline void MultMatrixVector(const Matrix& matrix, const SlimVector<3>& vector, SlimVector<3>& result) {
	//	MultMatrixVectorTemplate<Matrix, SlimVector<3>, SlimVector<3>>(matrix, vector, result);
	//}

	//for rotation matrix * vector multiplication
	inline void MultMatrixVector(const Matrix& matrix, const ConstSizeVector<3>& vector, Vector& result) {
		MultMatrixVectorTemplate<Matrix, ConstSizeVector<3>, Vector>(matrix, vector, result);
	}
	//for rotation matrix * vector multiplication
	inline void MultMatrixVector(const Matrix& matrix, const ConstSizeVector<3>& vector, ConstSizeVector<3>& result) {
		MultMatrixVectorTemplate<Matrix, ConstSizeVector<3>, ConstSizeVector<3>>(matrix, vector, result);
	}

	//for Euler parameter multiplications with G-matrix:
	inline void MultMatrixVector(const ConstSizeMatrix<12>& matrix, const ConstSizeVector<4>& vector, Vector& result) {
		MultMatrixVectorTemplate<ConstSizeMatrix<12>, ConstSizeVector<4>, Vector>(matrix, vector, result);
	}

	//for Euler parameter multiplications with G-matrix:
	inline void MultMatrixVector(const ConstSizeMatrix<12>& matrix, const ConstSizeVector<4>& vector, ConstSizeVector<3>& result) {
		MultMatrixVectorTemplate<ConstSizeMatrix<12>, ConstSizeVector<4>, ConstSizeVector<3>>(matrix, vector, result);
	}

	//for Euler parameter multiplications with G-matrix:
	inline void MultMatrixVector(const ConstSizeMatrix<12>& matrix, const ConstSizeVector<4>& vector, Vector3D& result) {
		MultMatrixVectorTemplate<ConstSizeMatrix<12>, ConstSizeVector<4>, Vector3D>(matrix, vector, result);
	}

	//for Euler parameter multiplications with G-matrix:
	inline void MultMatrixVector(const ConstSizeMatrix<12>& matrix, const LinkedDataVector& vector, Vector3D& result) {
		MultMatrixVectorTemplate<ConstSizeMatrix<12>, LinkedDataVector, Vector3D>(matrix, vector, result);
	}

	//for Euler parameter multiplications with G^T-matrix:
	inline void MultMatrixVector(const ConstSizeMatrix<12>& matrix, const Vector3D& vector, ConstSizeVector<4>& result) {
		MultMatrixVectorTemplate<ConstSizeMatrix<12>, Vector3D, ConstSizeVector<4>>(matrix, vector, result);
	}

	//for rotation matrix * vector multiplication
	inline void MultMatrixVector(const Matrix3D& matrix, Vector3D& vector, Vector& result) {
		MultMatrixVectorTemplate<Matrix3D, Vector3D, Vector>(matrix, vector, result);
	}
	inline void MultMatrixVector(const Matrix3D& matrix, Vector3D& vector, Vector3D& result) {
		MultMatrixVectorTemplate<Matrix3D, Vector3D, Vector3D>(matrix, vector, result);
	}
	inline void MultMatrixVector(const Matrix3D& matrix, const Vector& vector, Vector3D& result) {
		MultMatrixVectorTemplate<Matrix3D, Vector, Vector3D>(matrix, vector, result);
	}
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//transposed version of Matrix x Vector:
	inline void MultMatrixTransposedVector(const Matrix& matrix, const Vector& vector, Vector& result) {
		MultMatrixTransposedVectorTemplate<Matrix, Vector, Vector>(matrix, vector, result);
	}

	inline void MultMatrixTransposedVector(const Matrix& matrix, const SlimVector<3>& vector, Vector& result) {
		MultMatrixTransposedVectorTemplate<Matrix, SlimVector<3>, Vector>(matrix, vector, result);
	}

	inline void MultMatrixTransposedVector(const Matrix& matrix, const SlimVector<1>& vector, Vector& result) {
		MultMatrixTransposedVectorTemplate<Matrix, SlimVector<1>, Vector>(matrix, vector, result);
	}

	inline void MultMatrixTransposedVector(const Matrix& matrix, const ConstSizeVector<3>& vector, Vector& result) {
		MultMatrixTransposedVectorTemplate<Matrix, ConstSizeVector<3>, Vector>(matrix, vector, result);
	}

	inline void MultMatrixTransposedVector(const Matrix& matrix, const ConstSizeVector<3>& vector, ConstSizeVector<3>& result) {
		MultMatrixTransposedVectorTemplate<Matrix, ConstSizeVector<3>, ConstSizeVector<3>>(matrix, vector, result);
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//! Matrix * Matrix multiplication templates; this should also work with operator* in ConstSizeMatrix<9>
	inline void MultMatrixMatrix(const ConstSizeMatrix<9>& m1, const ConstSizeMatrix<9>& m2, ConstSizeMatrix<9>& result) {
		MultMatrixMatrixTemplate<ConstSizeMatrix<9>, ConstSizeMatrix<9>, ConstSizeMatrix<9>>(m1, m2, result);
	}

	//! Matrix * Matrix multiplication templates for multiplication with rigid body G-matrices
	inline void MultMatrixMatrix(const ConstSizeMatrix<12>& m1, const ConstSizeMatrix<9>& m2, ConstSizeMatrix<12>& result) {
		MultMatrixMatrixTemplate<ConstSizeMatrix<12>, ConstSizeMatrix<9>, ConstSizeMatrix<12>>(m1, m2, result);
	}

	//! Matrix * Matrix multiplication templates for multiplication with rigid body G-matrices
	inline void MultMatrixMatrix(const ConstSizeMatrix<9>& m1, const ConstSizeMatrix<12>& m2, ConstSizeMatrix<12>& result) {
		MultMatrixMatrixTemplate<ConstSizeMatrix<9>, ConstSizeMatrix<12>, ConstSizeMatrix<12>>(m1, m2, result);
	}

	//! Matrix * Matrix multiplication templates e.g. for computation of inertia tensor
	inline void MultMatrixMatrix(const ConstSizeMatrix<12>& m1, const ConstSizeMatrix<12>& m2, ConstSizeMatrix<16>& result) {
		MultMatrixMatrixTemplate<ConstSizeMatrix<12>, ConstSizeMatrix<12>, ConstSizeMatrix<16>>(m1, m2, result);
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//transposed(Matrix)*Matrix:

	//! transposed(Matrix) * Matrix multiplication templates for multiplication with rigid body G-matrices
	inline void MultMatrixTransposedMatrix(const ConstSizeMatrix<12>& m1, const ConstSizeMatrix<9>& m2, ConstSizeMatrix<12>& result) {
		MultMatrixTransposedMatrixTemplate<ConstSizeMatrix<12>, ConstSizeMatrix<9>, ConstSizeMatrix<12>>(m1, m2, result);
	}


}



