from typing import List
from pysat.formula import CNF
from pysat.solvers import Glucose3

id_variable = 0
oo = 1_000_000_000
sat_solver = Glucose3()

def plus_clause(clause):
    cl1 = []
    for x in clause:
        if abs(x) == oo:
            if (x < 0): return
        else: cl1.append(x)
    sat_solver.add_clause(cl1)
    # print("Add clause: " + str(cl1))

def exactly_k(vars: List[int], weight: List[int], k):
    n = len(vars) - 1
    global id_variable
    id_variable = n

    # Create map_register to hold the auxiliary variables
    map_register = [[] for _ in range(n + 1)]
    id_bonus = 0
    bonus = []
    
    cur = 0
    for i in range(1, n + 1):
        tmp = min(k, cur + weight[i])
        map_register[i] = [oo for _ in range(k + 1)]
        for j in range(1, tmp + 1):
            id_variable += 1
            map_register[i][j] = id_variable

        # (1) X_i -> R_i,j for j = 1 to w_i
        for j in range(1, weight[i] + 1):
            plus_clause([-vars[i], map_register[i][j]])
        
        # (5) ¬X_i -> ¬R_i,j for j = 1 + pos_{i-1} to pos_i
        for j in range(cur + 1, tmp + 1):
            plus_clause([vars[i], -map_register[i][j]])
        
        if cur != 0:
            # (2) R_{i-1,j} -> R_i,j for j = 1 to pos_{i-1}
            for j in range(1, cur + 1):
                plus_clause([-map_register[i - 1][j], map_register[i][j]])
            
            # (3) X_i ^ R_{i-1,j} -> R_i,j+w_i for j = 1 to pos_{i-1}
            for j in range(1, cur + 1):
                if j + weight[i] <= k:
                    plus_clause([-vars[i], -map_register[i - 1][j], map_register[i][j + weight[i]]])
                else:
                    plus_clause([-vars[i], -map_register[i - 1][j]])
            
            # (4) ¬X_i ^ ¬R_{i-1,j} -> ¬R_i,j for j = 1 to pos_{i-1}
            for j in range(1, cur + 1):
                plus_clause([vars[i], map_register[i - 1][j], -map_register[i][j]])

            # (6) ¬R_{i-1,j} -> ¬R_i,j+w_i for j = 1 to pos_{i-1}
            for j in range(1, cur + 1):
                if j + weight[i] <= k:
                    plus_clause([map_register[i - 1][j], -map_register[i][j + weight[i]]])

        cur = tmp
        if cur == k or i == n:
            # add a bonus bar
            if id_bonus == 0: bonus.append(map_register[i])
            else:
                bonus.append([0 for _ in range(k + 1)])
                for j in range(1, k + 1):
                    id_variable += 1
                    bonus[id_bonus][j] = id_variable
                
                a = map_register[i]
                b = bonus[id_bonus - 1]

                for j1 in range(1, k + 1): plus_clause([-a[j1], bonus[id_bonus][j1]])
                for j1 in range(1, k + 1): plus_clause([-b[j1], bonus[id_bonus][j1]])
                for j1 in range(1, k + 1):
                    for j2 in range(1, k + 1):
                        if j1 + j2 <= k: plus_clause([-a[j1], -b[j2], bonus[id_bonus][j1 + j2]])
                        else: plus_clause([-a[j1], -b[j2]])
                        if j1 + j2 - 1 <= k: plus_clause([a[j1], b[j2], -bonus[id_bonus][j1 + j2 - 1]])
            id_bonus += 1
            cur = 0

    plus_clause([bonus[id_bonus - 1][k]])

    return n

def print_solution(n):
    print(f"Number of variables: {sat_solver.nof_vars()}")
    print(f"Number of clauses: {sat_solver.nof_clauses()}")
    sat_status = sat_solver.solve()

    if not sat_status:
        print("UNSAT")
    else:
        solution = sat_solver.get_model()
        print(f"Solution found: {solution}")
        ans = []
        for i, val in enumerate(solution, start=1):
            if i <= n:
                ans.append(int(val > 0))
            else:
                break
        print(f"X = {ans}")

if __name__ == "__main__":
    # Example usage
    # the first element of the list is not used
    n = exactly_k([0, 1, 2, 3, 4, 5, 6, 7, 8], [0, 1, 1, 1, 1, 1, 1, 1, 1], 5)

    print_solution(n)