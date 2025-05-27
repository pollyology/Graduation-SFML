#include "Matrices.h"

namespace Matrices
{
    Matrix::Matrix(int _rows, int _cols)
    {
        rows = _rows;
        cols = _cols;
        a.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            a[i].resize(cols, 0);
        }
    }

	// Matrix addition
	// c = a + b
    Matrix operator+(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        {
            throw runtime_error("Error: dimensions must agree");
        }

        Matrix c(a.getRows(), a.getCols());

        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                c(i, j) = a(i, j) + b(i, j);
            }
        }
        return c;
    }

	// Matrix multiplication
	// c = a * b 
    Matrix operator*(const Matrix& a, const Matrix& b)
    {
        if (a.getCols() != b.getRows())
        {
            throw runtime_error("Error: dimensions must agree");
        }
     
        Matrix c(a.getRows(), b.getCols());

        // Use triple for loop (i, j, k) for equation: C[i][j] = A[i][k] * B[k][j]
        // i = rows of A; j = cols of B; k = cols of A / rows of B

        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < b.getCols(); j++)
            {
                for (int k = 0; k < a.getCols(); k++)
                {
                    c(i, j) += a(i, k) * b(k, j);
                }
            }
        }
        return c;
    }

	// Matrix comparison (equality)
	// a == b
    bool operator==(const Matrix& a, const Matrix& b)
    {
        if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        {
            return false;
        }

        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < b.getCols(); j++)
            {
                if (abs(a(i, j) - b(i, j)) > 0.001)
                {
                    return false;
                }
            }
        }
        return true;
    }

	// Matrix comparison (inequality)
	// a != b
    bool operator!=(const Matrix& a, const Matrix& b)
    {
        return !(a == b);
    }

	// Output matrix: separate columns by ' ' and rows by '\n'
    ostream& operator<<(ostream& os, const Matrices::Matrix& a)
    {
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int j = 0; j < a.getCols(); j++)
            {
                os << setw(10) << a(i, j) << " ";
            }
            os << endl;
        }
        return os;
    }

//	+===================================+
//	|		MATRIX CONSTRUCTORS			|	
//	+===================================+

///	2D rotation matrix
///	usage:  A = R * A rotates A theta radians counter-clockwise
RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
{
	a.at(0).at(0) = cos(theta);	   // (0,0) -> cos(theta)
	a.at(0).at(1) = -sin(theta);  // (0,1) -> -sin(theta)
	a.at(1).at(0) = sin(theta);  // (1,0) -> sin(theta)
	a.at(1).at(1) = cos(theta); // (1,1) -> cos(theta)
}

///	2D scaling matrix
///	usage:  A = S * A expands or contracts A by the specified scaling factor
ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
{
	// Hardcode scalar values in 2x2 matrix
	a.at(0).at(0) = scale;	
	a.at(1).at(1) = scale;
}

///2D Translation matrix
///usage:  A = T + A will shift all coordinates of A by (xShift, yShift)
TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			a.at(i).at(j) += (i == 0) ? xShift : yShift; // Hardcode x and y to row 0 and 1
		}
	}
}

}