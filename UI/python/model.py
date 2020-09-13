from graph import Graph, NodeSelection

class Model:
    def __init__(self, graph_path: str, selection_path: str = ''):
        self.graph_path = graph_path
        self.selection_path = selection_path

        if graph_path:
            self.graph = Graph(graph_path)
        if selection_path:
            self.selection = NodeSelection(selection_path)
