from pysat.solvers import Glucose3, Solver
from prettytable import PrettyTable
from threading import Timer
import datetime
import pandas as pd
import os
import sys
from openpyxl import load_workbook
from openpyxl import Workbook
from zipfile import BadZipFile
from openpyxl.utils.dataframe import dataframe_to_rows
from datetime import datetime

id_variable: int = 0
sat_solver: Solver = Glucose3()

def plus_clause(clause):
    sat_solver.add_clause(clause)
    # print(clause)

def exactly_k(var: list[int], k, n):
    global id_variable
    x = len(var) - 1
    assert x == k * n
    map_register = [[0 for j in range(k + 1)] for i in range(x + 1)]

    for id in range(1, n + 1):
        L =  (id - 1) * k
        for i in range(L + 1, L + k + 1):
            for j in range(1, i - L + 1):
                id_variable += 1
                map_register[i][j] = id_variable

        for i in range(L + 1, L + k + 1):
            plus_clause([var[i], -map_register[i][i - L]])
            plus_clause([-var[i], map_register[i][1]])

        for i in range(L + 2, L + k + 1):
            for j in range(1, i - L):
                plus_clause([var[i], map_register[i - 1][j], -map_register[i][j]])
                plus_clause([-map_register[i - 1][j], map_register[i][j]])

        for i in range(L + 2, L + k + 1):
            for j in range(2, i - L + 1):
                plus_clause([-var[i], -map_register[i - 1][j - 1], map_register[i][j]])
                plus_clause([map_register[i - 1][j - 1], -map_register[i][j]])
    
    bonus = [[0 for j in range(k + 1)] for i in range(n - 1)]
    for id in range(n - 1):
        for i in range(1, k + 1):
            id_variable += 1
            bonus[id][i] = id_variable
        
        a = map_register[(id + 2) * k]
        b = []
        if id == 0: b = map_register[k]
        else: b = bonus[id - 1]

        for i in range(1, k + 1):
            plus_clause([-a[i], bonus[id][i]])
            plus_clause([-b[i], bonus[id][i]])
            for j in range(1, k + 1):
                if i + j <= k: plus_clause([-a[i], -b[j], bonus[id][i + j]])
                if i + j - 1 <= k: plus_clause([a[i], b[j], -bonus[id][i + j - 1]])
                if i + j == k + 1: plus_clause([-a[i], -b[j]])
    
    plus_clause([bonus[n - 2][k]])

def print_solution(n):
    print(f"Number of variables: {sat_solver.nof_vars()}")
    print(f"Number of clauses: {sat_solver.nof_clauses()}")
    sat_status = sat_solver.solve()
    if not sat_status:
        print("UNSAT")
    else:
        solution = sat_solver.get_model()
        if all(val < 0 for val in solution):
            print("No solution")
        else:
            # print(f"Solution found: {solution}")
            for i, val in enumerate(solution, start=1):
                if i <= n:
                    print(f"X{i} = {int(val > 0)}")

if __name__ == "__main__":
    # Example usage

    # 10X1 + 2X2 + 6X3 + 11X4 + 15X5 + 4X6 + 8X7 + 3X8 + 7X9 + 10X10 = 20
    n = exactly_k([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], [1, 1, 1, 1, 1, 1, 1, 1, 1, 1], 3)

    print_solution(n)