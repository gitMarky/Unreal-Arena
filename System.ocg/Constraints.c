static const CONSTRAINT_BoundBy = 0;

/**
 Constraint: minimal value and maximal value.
 @par min The minimal value. If {@c nil} is passed, then this constraint is ignored.
 @par max The maximal value. If {@c nil} is passed, then this constraint is ignored.
 @return array An array with the constraint function.
 */
global func CF_BoundBy(int min, int max)
{
	return [CONSTRAINT_BoundBy, min, max];
}