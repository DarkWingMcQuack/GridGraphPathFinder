class Graph:
    def __init__(self, path: str):
        self.nodes = []
        with open(path, 'r') as graph_file:
            for line in graph_file:
                if '@' not in line and '.' not in line:
                    continue

                self.nodes.append(list([True if x == '.' else False for x in line]))

    def is_walkable(self, node: (int, int)):
        row, column = node
        try:
            return self.nodes[row][column]
        except IndexError:
            return False

    def width(self):
        return len(self.nodes[0])

    def height(self):
        return len(self.nodes)


class NodeSelection:
    def __init__(self, path):
        self.left_nodes = []
        self.right_nodes = []
        self.centers = []
        with open(path, 'r') as path_file:
            for line in path_file:
                if line.startswith("0: "):
                    row, column = line.replace(' ', '')[3:-2].split(',')
                    self.left_nodes.append((int(row), int(column)))
                if line.startswith("1: "):
                    row, column= line.replace(' ', '')[3:-2].split(',')
                    self.right_nodes.append((int(row), int(column)))
                if line.startswith("center: "):
                    row, column= line.replace(' ', '')[8:-2].split(',')
                    self.centers.append((int(row), int(column)))
