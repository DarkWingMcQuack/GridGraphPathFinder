
class GridCellCorner:
    def __init__(self, row: int, column: int):
        self.row = row
        self.column = column


class GridCell:
    def __init__(self,
                 top_left: GridCellCorner,
                 top_right: GridCellCorner,
                 bottom_left: GridCellCorner,
                 bottom_right: GridCellCorner):
        self.top_left = top_left
        self.top_right = top_right
        self.bottom_left = bottom_left
        self.bottom_right = bottom_right

    def is_in_cell(self, node: (int, int)):
        row, column = node

        return (row >= self.top_left.row
                and row <= self.bottom_left.row
                and column >= self.top_left.column
                and column <= self.top_right.column)
