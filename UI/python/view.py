from tkinter import (HORIZONTAL,
                     VERTICAL,
                     Scrollbar,
                     Tk,
                     BOTTOM,
                     Canvas,
                     RIGHT,
                     X,
                     Y,
                     LEFT,
                     BOTH)
from graph import Graph, NodeSelection


class View:
    def __init__(self, graph_width, graph_height, title):
        self.scale_factor = max(650 // max(graph_width, graph_height), 3)
        self.root = Tk()
        self.root.title(title)
        self.hbar = Scrollbar(self.root, orient=HORIZONTAL)
        self.hbar.pack(side=BOTTOM, fill=X)
        self.vbar = Scrollbar(self.root, orient=VERTICAL)
        self.vbar.pack(side=RIGHT, fill=Y)
        self.canvas = Canvas(self.root,
                             width=min(900, max(graph_height, graph_width) * self.scale_factor),
                             height=650,
                             scrollregion=(0,
                                           0,
                                           graph_width*(self.scale_factor + 1.5),
                                           graph_height*(self.scale_factor + 1.5)))


    def draw_model(self, model):
        self.canvas.delete("all")
        self.draw_graph(model.graph)
        self.draw_selection(model.selection, model.graph)

        self.vbar.config(command=self.canvas.yview)
        self.hbar.config(command=self.canvas.xview)
        self.canvas.config(xscrollcommand=self.hbar.set,
                           yscrollcommand=self.vbar.set)
        self.canvas.pack(side=LEFT, expand=True, fill=BOTH)

    def draw_node(self, node: (int, int), color: str):
        x, y = node
        self.canvas.create_rectangle(self.scale_factor*x,
                                     self.scale_factor*y,
                                     self.scale_factor*x + self.scale_factor,
                                     self.scale_factor*y + self.scale_factor,
                                     fill=color)

    def draw_graph(self, graph: Graph):
        for row, nested in enumerate(graph.nodes):
            for column, is_walkable in enumerate(nested):
                node = (row, column)
                if is_walkable:
                    self.draw_node(node, 'white')
                else:
                    self.draw_node(node, 'gray')

    def draw_selection(self, selection: NodeSelection, graph: Graph):
        for node in selection.left_nodes:
            if graph.is_walkable(node):
                self.draw_node(node, 'springgreen')
            else:
                self.draw_node(node, 'darkgreen')
        for node in selection.right_nodes:
            if graph.is_walkable(node):
                self.draw_node(node, 'deepskyblue')
            else:
                self.draw_node(node, 'navy')
        for node in selection.centers:
            self.draw_node(node, 'red')
