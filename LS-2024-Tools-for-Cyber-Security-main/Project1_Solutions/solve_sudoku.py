from copy import deepcopy

class Node:
    def __init__(self, board):
        self.board = [deepcopy(row) for row in board]
        self.unfilled_cells = []
        for i in range(9):
            for j in range(9):
                if self.board[i][j] == 0:
                    self.unfilled_cells.append((i, j))

    def get_empty_cell(self):
        for i in range(9):
            for j in range(9):
                if self.board[i][j] == 0:
                    return i, j
        return None

    def is_valid(self, i, j, num):
        for k in range(9):
            if self.board[i][k] == num or self.board[k][j] == num:
                return False
        for k in range(3):
            for l in range(3):
                if self.board[3*(i//3)+k][3*(j//3)+l] == num:
                    return False
        return True

    def solve(self):
        cell = self.get_empty_cell()
        if cell is None:
            return True
        i, j = cell
        for num in range(1, 10):
            if self.is_valid(i, j, num):
                self.board[i][j] = num
                if self.solve():
                    return True
                self.board[i][j] = 0
        return False
    
    def print_board(self):
        for row in self.board:
            print(row)
        
    def check_solved(self):
        for i in range(9):
            for j in range(9):
                if self.board[i][j] == 0:
                    return False
        return True

def solve_sudoku(board):
    node = Node(board)
    if node.solve():
        node.print_board()
    else:
        print("No solution exists")


# if __name__ == "__main__":
#     board = [
#         [5, 3, 0, 0, 7, 0, 0, 0, 0],
#         [6, 0, 0, 1, 9, 5, 0, 0, 0],
#         [0, 9, 8, 0, 0, 0, 0, 6, 0],
#         [8, 0, 0, 0, 6, 0, 0, 0, 3],
#         [4, 0, 0, 8, 0, 3, 0, 0, 1],
#         [7, 0, 0, 0, 2, 0, 0, 0, 6],
#         [0, 6, 0, 0, 0, 0, 2, 8, 0],
#         [0, 0, 0, 4, 1, 9, 0, 0, 5],
#         [0, 0, 0, 0, 8, 0, 0, 7, 9]
#     ]
#     solve_sudoku(board)