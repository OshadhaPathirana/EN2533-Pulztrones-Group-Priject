# X - virtual box
# * - Robot

#   - empty space


#  -
# | |  - node - None
#  -

#  - -
# |X| | - Box 
#  -

#  -
# |*|  - robot - 
#  -

#  -
# |#|  - robot carrying box 
#  -

import time
from creating_maze import *
from print_maze import *


def wall_following(grid, robot):
    if robot.face == "down" or robot.face == "up":
        pass

def finding_open_gate(grid, robot, box_pos):
    # position of the box
    place_box(grid, box_pos)
    i, j = robot.position
    if (i, j) != box_pos:
        robot.path.append("forward")
        grid[i][j].robot = False
        i = i + 1
        robot.position = (i, j)
        grid[i][j].robot = True

        robot.path.append("right")
        robot.face = "left"
        robot.path.append("forward")
        grid[i][j].robot = False
        j = j - 1
        robot.position = (i, j)
        grid[i][j].robot = True
        
        if grid[i][j].down is not None:
            robot.opened_gate = (i, j)
        else:
            robot.opened_gate = (i, j - 2)
        return (i, j), box_pos # robot position, box position
    else:
        robot.path.append("pick")
        grid[i][j].box = False

        robot.path.append("forward")
        grid[i][j].robot = False
        i = i + 1
        robot.position = (i, j)
        grid[i][j].robot = True

        robot.path.append("drop")
        grid[i][j].box = True

        robot.path.append("reverse")
        grid[i][j].robot = False
        i = i - 1
        robot.position = (i, j)
        grid[i][j].robot = True

        robot.path.append("right")
        robot.face = "left"
        robot.path.append("forward")
        grid[i][j].robot = False
        j = j - 1
        robot.position = (i, j)
        grid[i][j].robot = True

        robot.path.append("left")
        robot.face = "down"
        robot.path.append("forward")
        grid[i][j].robot = False
        i = i + 1
        robot.position = (i, j)
        grid[i][j].robot = True

        if grid[i][j].down is not None:
            robot.opened_gate = (i, j)
        else:
            robot.opened_gate = (i, j - 2)
        return (i, j), (1, 4)


        

        

def solve(grid, robot):
    finding_open_gate(grid, robot)
    print(robot.path)





grid = create_grid()
# open gate

robot = Robot()
grid[0][4].robot = robot



# open the gate
open_gate(grid, 1)

#solve(grid, robot)
print(finding_open_gate(grid, robot, (0, 1)))



print_grid(grid)
print(robot.opened_gate)
print(robot.path)

"""

"""