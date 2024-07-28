# Flag: flag{f34r1355_5ud0ku_c0nqu3r0r}

import subprocess
from solve_sudoku import Node
import re
from tqdm import tqdm

process = subprocess.Popen(["./sudoku"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
count = 0
for k in range(840):
    sudoku = []
    while (line := process.stdout.readline().decode()) and len(sudoku) < 9:
        if 'Snuday' in line:
            count += 1
        line = line.strip()
        row = []
        if re.match(r"\| [0-9\.] [0-9\.] [0-9\.] \|", line):
            for char in line:
                if char == ".":
                    row.append(0)
                elif char.isdigit():
                    row.append(int(char))
            sudoku.append(row)
    node = Node(sudoku)
    node.solve()
    if not node.check_solved():
        print("No solution exists")
    for i in range(len(node.unfilled_cells)):
        cell = node.unfilled_cells[i]
        process.stdin.write(f"{cell[0]} {cell[1]} {node.board[cell[0]][cell[1]]}\n".encode())
        process.stdin.flush()
        if i != len(node.unfilled_cells) - 1:
            for i in range(14):
                line = process.stdout.readline()
while (line := process.stdout.readline()):
    print(line.decode().strip())