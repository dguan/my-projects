This is the implementation of Gauss-Legendre integration. When you are using (n+1) order Legendre functions as orthogonal functions, you can achieve (2n) order converging speed, so usually you could get to desired precision more quickly, and it also works good for certain special functions, which is hard to converge or cannot get precise result using normal Simpson integration.

The default Legendre function is order-4, and the maximum order is order-10. The default precision is 1e-6, which you can change. Note that, the maximum divisoin count (or, iteration count) is set to 16. If you cannot get desired precision after 16 divisions, the function will throw the result instead returning the result, indicating that the maximum division count has been meet, and the result's precision is not reliable. Under this condition, you could either increase Legendre function's order, or more practically, you could subdivide the integration section, and accumulate the piece-wise integrations together.

The Matlab scripts are also included, maybe you need to check case sensitivity if you are running Matlab in Unix/Linux environments.


