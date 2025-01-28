def print_grid(grid):
    print(" --- --- --- --- ---  ")
    for i in range(2):
        for j in range(5):
            if grid[i][j].left == None:
                print("| ", end="")
            else:
                print(" ", end="")
            
            if grid[i][j].box == False:
                if grid[i][j].robot == False:
                    print("  |", end="")
                else:
                    print("* |", end="")
            elif grid[i][j].box == True:
                if grid[i][j].robot == False:
                    print("X |", end="")
                else:
                    print("# |", end="")
        print("\n --- --- --- --- ---  ")