(********************************************************************
 * COPYRIGHT (C) BERNECKER + RAINER, AUSTRIA, A-5142 EGGELSBERG
 ********************************************************************
 * Library: Operator
 * File: Operator.fun
 * Created: 11.11.2003
 ********************************************************************
 * Functions and function blocks of library Operator
 ********************************************************************)
FUNCTION SIZEOF : UDINT 		(*determines the size of a variable in bytes*)
	VAR_INPUT
		in	:ANY;				(*input value*)
	END_VAR
END_FUNCTION
FUNCTION ADR : UDINT 			(*determines the address of a data point*)
	VAR_INPUT
		in	:ANY;				(*input value*)
	END_VAR
END_FUNCTION
FUNCTION SHR : ANY 				(*shifts bitwise to the right*)
	VAR_INPUT
		IN	:ANY;				(*input value*)
		N	:SINT;				(*number of bits to be shifted*)
	END_VAR
END_FUNCTION
FUNCTION ROR : ANY 				(*rotates bitwise to the right*)
	VAR_INPUT
		IN	:ANY;				(*input value*)
		N	:SINT;				(*number of bits to be rotated*)
	END_VAR
END_FUNCTION
FUNCTION ROL : ANY 				(*rotates bitwise to the left*)
	VAR_INPUT
		IN	:ANY;				(*input value*)
		N	:SINT;				(*number of bits to be rotated*)
	END_VAR
END_FUNCTION
FUNCTION SHL : ANY 				(*shifts bitwise to the left*)
	VAR_INPUT
		IN	:ANY;				(*input value*)
		N	:SINT;				(*number of bits to be shifted*)
	END_VAR
END_FUNCTION
FUNCTION AND : ANY 				(*makes a bitwise AND relation for 2 or more variables*)
	VAR_INPUT
		INx	:ANY;				(*input value*)
	END_VAR
END_FUNCTION
FUNCTION XOR : ANY 				(*makes a bitwise XOR relation for 2 or more variables*)
	VAR_INPUT
		INx	:ANY;				(*input values*)
	END_VAR
END_FUNCTION
FUNCTION OR : ANY 				(*makes a bitwise OR relation for 2 or more variables*)
	VAR_INPUT
		INx	:ANY;				(*input values*)
	END_VAR
END_FUNCTION
FUNCTION NOT : ANY 				(*makes a bitwise inversion of variables*)
	VAR_INPUT
		IN	:ANY;				(*input value*)
	END_VAR
END_FUNCTION
FUNCTION ADD : ANY 				(*adds 2 or more variables*)
	VAR_INPUT
		INx	:ANY;				(*input values*)
	END_VAR
END_FUNCTION
FUNCTION MUL : ANY_NUM 			(*multiplies 2 or more variables*)
	VAR_INPUT
		INx	:ANY_NUM;			(*input values*)
	END_VAR
END_FUNCTION
FUNCTION SUB : ANY 				(*subtracts one variable from another variable*)
	VAR_INPUT
		IN1	:ANY;				(*value 1*)
		IN2	:ANY;				(*value 2*)
	END_VAR
END_FUNCTION
FUNCTION DIV : ANY_NUM 			(*divides a variable by another variable*)
	VAR_INPUT
		IN1	:ANY_NUM;			(*dividend*)
		IN2	:ANY_NUM;			(*divisor*)
	END_VAR
END_FUNCTION
FUNCTION MOD : ANY_INT 			(*makes a remainder when dividing a variable by another variable*)
	VAR_INPUT
		IN1	:ANY_INT;			(*dividend*)
		IN2	:ANY_INT;			(*divisor*)
	END_VAR
END_FUNCTION
FUNCTION MOVE : ANY 			(*copies the contents of the input variable to the output variable*)
	VAR_INPUT
		IN	:ANY;				(*input variable*)
	END_VAR
END_FUNCTION
FUNCTION MAX : ANY 				(*determines the maximum value of two values*)
	VAR_INPUT
		IN1	:ANY;				(*input value 1*)
		IN2	:ANY;				(*input value 2*)
	END_VAR
END_FUNCTION
FUNCTION MIN : ANY 				(*determines the lowest value of two values*)
	VAR_INPUT
		IN1	:ANY;				(*input value 1*)
		IN2	:ANY;				(*input value 2*)
	END_VAR
END_FUNCTION
FUNCTION LIMIT : ANY 			(*limits a value to within maximum and minimum boundaries*)
	VAR_INPUT
		MN	:ANY;				(*minimum value*)
		IN	:ANY;				(*input value*)
		MX	:ANY;				(*maximum value*)
	END_VAR		
END_FUNCTION
FUNCTION SEL : ANY 				(*selects one value from two values*)
	VAR_INPUT
		G	:BOOL;				(*selection variable*)
		IN0	:ANY;				(*input value for FALSE*)
		IN1	:ANY;				(*input value for TRUE*)
	END_VAR
END_FUNCTION
FUNCTION MUX : ANY 				(*selects a value from several values*)
	VAR_INPUT
		K	:SINT;				(*selection variable*)
		INx	:ANY;				(*input values*)
	END_VAR
END_FUNCTION
FUNCTION GE : BOOL 				(*compares whether value 1 is greater or equal than value 2*)
	VAR_INPUT
		IN1	:ANY;				(*comparison value 1*)
		IN2	:ANY;				(*comparison value 2*)
	END_VAR
END_FUNCTION
FUNCTION EQ : BOOL 				(*compares two values whether they are equal*)
	VAR_INPUT
		IN1	:ANY;				(*comparison value 1*)
		IN2	:ANY;				(*comparison value 2*)
	END_VAR
END_FUNCTION
FUNCTION GT : BOOL 				(*compares whether value 1 is greater than value 2*)
	VAR_INPUT
		IN1	:ANY;				(*comparison value 1*)
		IN2	:ANY;				(*comparison value 2*)
	END_VAR
END_FUNCTION
FUNCTION LE : BOOL 				(*compares whether value 1 is less or equal than value 2*)
	VAR_INPUT
		IN1	:ANY;				(*comparison value 1*)
		IN2	:ANY;				(*comparison value 2*)
	END_VAR
END_FUNCTION
FUNCTION LT : BOOL 				(*compares whether value 1 is less than value 2*)
	VAR_INPUT
		IN1	:ANY;				(*comparison value 1*)
		IN2	:ANY;				(*comparison value 2*)
	END_VAR
END_FUNCTION
FUNCTION NE : BOOL 				(*compares two values whether they are not equal*)
	VAR_INPUT
		IN1	:ANY;				(*comparison value 1*)
		IN2	:ANY;				(*comparison value 2*)
	END_VAR
END_FUNCTION
FUNCTION ABS : ANY_NUM 			(*returns the absolute value of a number*)
	VAR_INPUT
		IN	:ANY_NUM;			(*input value*)
	END_VAR
END_FUNCTION
FUNCTION SQRT : ANY_REAL 		(*returns the square root of a number*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value*)
	END_VAR
END_FUNCTION
FUNCTION LN : ANY_REAL 			(*returns the result of a natural logarithm*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value*)
	END_VAR
END_FUNCTION
FUNCTION LOG : ANY_REAL 		(*returns the result of a base 10 logarithm*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value*)
	END_VAR
END_FUNCTION
FUNCTION EXP : ANY_REAL 		(*returns the result of a natural exponential function*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value*)
	END_VAR
END_FUNCTION
FUNCTION SIN : ANY_REAL 		(*returns the sine of a number (calculated in radian)*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value in radian*)
	END_VAR
END_FUNCTION
FUNCTION COS : ANY_REAL 		(*returns the cosine of a number (calculated in radian)*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value in radian*)
	END_VAR
END_FUNCTION
FUNCTION TAN : ANY_REAL 		(*returns the tangent of a number (calculated in radian)*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value in radian*)
	END_VAR
END_FUNCTION
FUNCTION ASIN : ANY_REAL 		(*returns the arc sine (inverse function of sine) of a number (calculated in radian)*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value*)
	END_VAR
END_FUNCTION
FUNCTION ACOS : ANY_REAL 		(*returns the arc cosine (inverse function of cosine) of a number (calculated in radian)*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value*)
	END_VAR
END_FUNCTION
FUNCTION ATAN : ANY_REAL 		(*returns the arc tangent (inverse function of tangent) of a number (calculated in radian)*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value*)
	END_VAR
END_FUNCTION
FUNCTION EXPT : REAL 			(*raises one variable to the power of another*)
	VAR_INPUT
		IN1	:REAL;				(*base*)
		IN2	:ANY_NUM;			(*exponent*)
	END_VAR
END_FUNCTION
FUNCTION TRUNC : INT 			(*converts from type REAL to type INT (number is always rounded down)*)
	VAR_INPUT
		IN	:ANY_REAL;			(*input value*)
	END_VAR
END_FUNCTION
