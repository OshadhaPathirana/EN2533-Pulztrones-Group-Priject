class Node:

    def __init__(self,x, y):
        self.left = None
        self.right = None
        self.up = None
        self.down = None
        self.box = False
        self.robot = False
        

class Robot:

    def __init__(self):
        self.position = (0, 4)
        self.face = "down"
        self.path = []
        self.opened_gate = (None, None)
        

def create_grid():
    # create a 2 X 5 grid
    grid = [[Node(i, j) for i in range(5)] for j in range(3)]
    
    grid[0][0].right = grid[0][1]
    grid[0][1].left = grid[0][0]
    grid[0][1].right = grid[0][2]
    grid[0][2].left = grid[0][1]
    grid[0][2].right = grid[0][3]
    grid[0][3].left = grid[0][2]
    grid[0][3].right = grid[0][4]
    grid[0][4].left = grid[0][3]

    grid[1][0].right = grid[1][1]
    grid[1][1].left = grid[1][0]
    grid[1][1].right = grid[1][2]
    grid[1][2].left = grid[1][1]
    grid[1][2].right = grid[1][3]
    grid[1][3].left = grid[1][2]
    grid[1][3].right = grid[1][4]
    grid[1][4].left = grid[1][3]

    grid[0][0].down = grid[1][0]
    grid[1][0].up = grid[0][0]
    grid[0][1].down = grid[1][1]
    grid[1][1].up = grid[0][1]
    grid[0][2].down = grid[1][2]
    grid[1][2].up = grid[0][2]
    grid[0][3].down = grid[1][3]
    grid[1][3].up = grid[0][3]
    grid[0][4].down = grid[1][4]
    grid[1][4].up = grid[0][4]

    

    return grid


def place_box(grid, box_pos=(0, 0)):
    # initial position of the robot
    grid[box_pos[0]][box_pos[1]].box = True

def open_gate(grid, gate):
    if gate == 1:
        grid[1][1].down = grid[2][1]
    
    elif gate == 3:
        grid[1][3].down = grid[2][3]