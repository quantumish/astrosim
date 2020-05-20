#!/usr/bin/env python

import scipy
from scipy.optimize import fsolve

def solve(coords):
    x1 = coords[0][0]
    y1 = coords[0][1]
    x2 = coords[1][0]
    y2 = coords[1][1]
    x3 = coords[2][0]
    y3 = coords[2][1]
    x4 = coords[3][0]
    y4 = coords[3][1]
    x5 = coords[4][0]
    y5 = coords[4][1]
    x6 = coords[5][0]
    y6 = coords[5][1]
    def equations(p):
        A, B, C, D, E, F = p
        # print(A * x1**2 + B * y1**x1 + C * y1**2 + D*x1 + E*y1 + F)
        return (A * x1**2 + B * y1**x1 + C * y1**2 + D*x1 + E*y1 + F,
                A * x2**2 + B * y2**x2 + C * y2**2 + D*x2 + E*y2 + F,
                A * x3**2 + B * y3**x3 + C * y3**2 + D*x3 + E*y3 + F,
                A * x4**2 + B * y4**x4 + C * y4**2 + D*x4 + E*y4 + F,
                A * x5**2 + B * y5**x5 + C * y5**2 + D*x5 + E*y5 + F,
                A * x6**2 + B * y6**x6 + C * y6**2 + D*x6 + E*y6 + F)

    A, B, C, D, E, F =  scipy.optimize.broyden1(equations, (1, 1, 1, 1, 1, 1))
    # print((A, B, C, D, E, F))

solve([[400, 1140], [399.7, 1139.99], [399.4, 1139.98], [399.1, 1139.96], [398.8, 1139.93], [398.5, 1139.9]])
