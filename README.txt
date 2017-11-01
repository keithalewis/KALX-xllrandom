Add diehard(er) tests for arbitrary random number generators.

Runs Test
---------
Let X(i) be iid random variables having cdf F.
Let R = sum 1(X(i) > X(i - 1)).
What is the distribuition of R?

E R = 1(X(i) > X(i - 1)) 
	= int_{y > x} dF(x) dF(y) 
	= int_{-infty,infty} F(y) dF(y) 
	= F^2(y)/2|_infty^infty = 1/2

Var R = Var sum 1(x(i) > x(i - 1)) = diagonal + upper + lower off diagonal

Var (1(x(i + 1) > x(i)) = 1/2 - 1/4 = 1/4.
Cov (1(x(i + 1) > x(i), 1(x(i) > x(i - 1)) = ?
