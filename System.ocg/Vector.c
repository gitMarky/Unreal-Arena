
global func Vector2D(int x, int y)
{
	var vector = {};
	vector.x = x;
	vector.y = y;
	return vector;
}

global func Vector3D(int x, int y, int z)
{
	var vector = {};
	vector.x = x;
	vector.y = y;
	vector.z = z;
	return vector;
}


/**
 Addition of two vectors.
 @param a Vector
 @param b Vector to add
 @return array vector {@c a + b}
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
 Subtraction of two vectors.
 @param a Vector
 @param b Vector to subtract
 @return array vector {@c a - b}
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
 @par a vector
 @par n number
 @return array A vector where each element is multiplied by n 
*/
global func Vec_Mul(proplist a, int n)
{
	AssertNotNil(a, "vector 'a'");
	
	var vector = {};
	
	for (var i in GetProperties(a))
	{
		vector[i] = n * a[i];
	}

	return vector;
}

/**
 Division of a vector and a number.
 @par a vector
 @par n number
 @return array A vector where each element is divided by n 
*/
global func Vec_Div(proplist a, int n)
{
	AssertNotNil(a, "vector 'a'");
	
	var vector = {};
	
	for (var i in GetProperties(a))
	{
		vector[i] = a[i] / n;
	}

	return vector;
}

/**
 Dot product of two vectors.
 @par x vector 1
 @par y vector 2
 @return Sum of a_i * b_i, for i = 0,...,n
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
 Length of a vector
 @par a Vector
 @return The length of the vector: the square root of the dot product
*/
global func Vec_Length(proplist a)
{
	return Sqrt(Vec_Dot(a, a));
}

global func Vec_Dim(proplist a)
{
	AssertNotNil(a, "vector 'a'");

	return GetLength(GetProperties(a));
}

/**
 Angle between two vectors.
 @param a A two-dimensional vector.
 @param b A two-dimensional vector.
 @param return angle between a and b
*/
global func Vec_Angle(proplist a, proplist b)
{
	AssertVectorOperation(a, b);
	AssertVectorDimension(a, 2);

	return Angle(a.x, a.y, b.x, b.y);
}

/**
 Normalizes a vector with precision.
 @par a vector
 @par precision factor for the resultion length
 @return the normalize vector with length 1*precision
*/
global func Vec_Normalize(proplist a, int precision)
{
	if (!precision) precision = 1;
	return Vec_Div(Vec_Mul(a, precision), Vec_Length(a));
}


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
}

global func AssertVectorDimension(proplist a, int dimension)
{
	var dim = Vec_Dim(a); 
	if (dim != dimension)
	{
		FatalError(Format("The function expects a vector of length %d, got length %d", dimension, dim));
	}
}

global func AssertNotNil(parameter, string message)
{
	if (parameter == nil)
	{
		if (!message) message = "parameter";
		
		FatalError(Format("Expected %s that is not nil", message));
	}
}