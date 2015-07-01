
/**
 Addition of two vectors.
 @param a Vector
 @param b Vector to add
 @return array vector {@c a + b}
*/
global func Vec_Add(array a, array b)
{
	AssertVectorOperation(a, b);
	
	var vector = [];
	
	for (var i = 0; i < GetLength(a); i++)
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
global func Vec_Sub(array a, array b)
{
	AssertVectorOperation(a, b);
	
	var vector = [];
	
	for (var i = 0; i < GetLength(a); i++)
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
global func Vec_Mul(array a, int n)
{
	AssertNotNil(a, "vector 'a'");
	
	var vector = [];
	
	for (var i = 0; i < GetLength(a); i++)
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
global func Vec_Div(array a, int n)
{
	AssertNotNil(a, "vector 'a'");
	
	var vector = [];
	
	for (var i = 0; i < GetLength(a); i++)
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
global func Vec_Dot(array a, array b)
{
	AssertVectorOperation(a, b);
	
	var sum = 0;
	
	for (var i = 0; i < GetLength(a); i++)
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
global func Vec_Length(array a)
{
	return Sqrt(Vec_Dot(a, a));
}

/**
 Angle between two vectors.
 @param a A two-dimensional vector.
 @param b A two-dimensional vector.
 @param return angle between a and b
*/
global func Vec_Angle(array a, array b)
{
	AssertVectorOperation(a, b);
	
	if (GetLength(a) != 2)
	{
		FatalError("The function expects a vector of length 2, got length %d", GetLength(a));
	}
	
	return Angle(a[0], a[1], b[0], b[1]);
}

/*
 Normalizes a vector with precision.
 @par a vector
 @par precision factor for the resultion length
 @return the normalize vector with length 1*precision
*/
global func Vec_Normalize(array a, int precision)
{
	if (!precision) precision = 1;
	return Vec_Div(Vec_Mul(a, precision), Vec_Length(a));
}


global func AssertVectorOperation(array x, array y)
{
	AssertNotNil(x, "vector 'x'");
	AssertNotNil(y, "vector 'y'");
	
	if (GetLength(x) != GetLength(y))
	{
		FatalError(Format("This method only works with vectors of the same length. Vector 'x' has %d elements, vector 'y' has %d elements", GetLength(x), GetLength(y)));
	}
}

global func AssertNotNil(parameter, message)
{
	if (parameter == nil)
	{
		if (!message) message = "parameter";
		
		FatalError(Format("Expected %d that is not nil", message));
	}
}