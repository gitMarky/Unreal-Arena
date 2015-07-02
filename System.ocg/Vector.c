/**
 Library for vector operations.
 
 @author Marky
 @credits Randrian, since many functions have their origin in the rope physics library.
 */

/**
 Creates a two-dimensional vector.
 @par x The x element of the vector. Can be accessed from the proplist as 'x'.
 @par y The y element of the vector. Can be accessed from the proplist as 'y'.
 @return proplist The vector as a proplist.
 */
global func Vector2D(int x, int y)
{
	var vector = {};
	vector.x = x;
	vector.y = y;
	return vector;
}


/**
 Creates a three-dimensional vector.
 @par x The x element of the vector. Can be accessed from the proplist as 'x'.
 @par y The y element of the vector. Can be accessed from the proplist as 'y'.
 @par z The z element of the vector. Can be accessed from the proplist as 'z'.
 @return proplist The vector as a proplist.
 */
global func Vector3D(int x, int y, int z)
{
	var vector = {};
	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}

/**
 Addition of two vectors of the same dimension.
 @par a The first vector.
 @par b The second vector.
 @return proplist The vector (a_i + b_i), i=1,...,n.
*/
global func Vec_Add(proplist a, proplist b)
{
	AssertVectorOperation(a, b);
	
	var vector = {};
	
	for (var i in GetProperties(a))
	{
		vector[i] = a[i] + b[i];
	}

	return vector;
}


/**
 Subtraction of two vectors of the same dimension.
 @par a The first vector.
 @par b The second vector.
 @return proplist The vector (a_i - b_i), i=1,...,n.
*/
global func Vec_Sub(proplist a, proplist b)
{
	AssertVectorOperation(a, b);
	
	var vector = {};

	for (var i in GetProperties(a))
	{
		vector[i] = a[i] - b[i];
	}

	return vector;
}


/**
 Multiplication of a vector and a number.
 @par a The vector
 @par b The number
 @return proplist The vector (b * a_i), i=1,...,n.
*/
global func Vec_Mul(proplist a, int b)
{
	AssertNotNil(a, "vector 'a'");
	
	var vector = {};
	
	for (var i in GetProperties(a))
	{
		vector[i] = b * a[i];
	}

	return vector;
}


/**
 Division of a vector and a number.
 @par a The vector
 @par b The number
 @return proplist The vector (a_i / b), i=1,...,n.
*/
global func Vec_Div(proplist a, int b)
{
	AssertNotNil(a, "vector 'a'");
	
	var vector = {};
	
	for (var i in GetProperties(a))
	{
		vector[i] = a[i] / b;
	}

	return vector;
}


/**
 Dot product of two vectors of the same dimension.
 @par a The first vector
 @par b The second vector
 @return int Sum of a_i * b_i, for i = 1,...,n
*/
global func Vec_Dot(proplist a, proplist b)
{
	AssertVectorOperation(a, b);
	
	var sum = 0;
	
	for (var i in GetProperties(a))
	{
		sum += a[i] * b[i];
	}

	return sum;
}


/**
 Euclidean length of a vector.
 @par a The vector.
 @return int Square root of the sum of a_i * a_i, for i=1,...,n.
*/
global func Vec_Length(proplist a)
{
	return Sqrt(Vec_Dot(a, a));
}


/**
 The dimension of a vector.
 @par a The vector
 @return int The number of elements of the vector.
 */
global func Vec_Dim(proplist a)
{
	AssertNotNil(a, "vector 'a'");

	return GetLength(GetProperties(a));
}


/**
 Angle between two vectors.
 @par a A two-dimensional vector.
 @par b A two-dimensional vector.
 @return int The angle between a and b.
*/
global func Vec_Angle(proplist a, proplist b)
{
	AssertVectorOperation(a, b);
	AssertVectorDimension(a, 2);

	return Angle(a.x, a.y, b.x, b.y);
}


/**
 Normalizes a vector with precision.
 @par a The vector.
 @par precision Factor for the resultion length.
 @return proplist The normalized vector with length = 1 * precision
*/
global func Vec_Normalize(proplist a, int precision)
{
	if (!precision) precision = 1;
	return Vec_Div(Vec_Mul(a, precision), Vec_Length(a));
}


/**
 Assert that a vector operation can be done with these two vectors.
 The vectors have to be of the same dimension and their elements
 must have the same names.
 @par a The first vector, must not be nil.
 @par b The second vector, must not be nil.
 */
global func AssertVectorOperation(proplist a, proplist b)
{
	AssertNotNil(a, "vector 'a'");
	AssertNotNil(b, "vector 'b'");
	
	var length_a = GetLength(GetProperties(a));
	var length_b = GetLength(GetProperties(b));
	if (length_a != length_b)
	{
		FatalError(Format("This method only works with vectors of the same length. Vector 'a' has %d elements, vector 'b' has %d elements", length_a, length_b));
	}
	
	for (var element in GetProperties(a))
	{
		if (b[element] == nil)
		{
			FatalError(Format("All elements of vector 'a' should have a corresponding element in 'b'. Found no element '%s' in 'b'", element));
		}
	}
}


/**
 Asserts that a vector has a specific dimesion.
 @par a The vector.
 @par dimension The dimension.
 */
global func AssertVectorDimension(proplist a, int dimension)
{
	var dim = Vec_Dim(a); 
	if (dim != dimension)
	{
		FatalError(Format("The function expects a vector of length %d, got length %d", dimension, dim));
	}
}

/**
 Asserts that a parameter is not nil.
 @par parameter The parameter that is checked.
 @par message [optional] a descriptive message for the parameter that is printed in the fatal error.
 */
global func AssertNotNil(parameter, string message)
{
	if (parameter == nil)
	{
		if (!message) message = "parameter";
		
		FatalError(Format("Expected %s that is not nil", message));
	}
}