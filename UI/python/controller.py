from view import View
from tkinter import Tk

class Controller:
    def __init__(self, model):
        self.model = model
        self.view = View(self.model.graph.width(),
                         self.model.graph.height(),
                         self.model.graph_path)

    def run(self):
        self.view.draw_model(self.model)
        self.view.root.mainloop()
