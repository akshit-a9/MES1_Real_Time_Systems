class node:
    def __init__(self,name=None, rt = 0, dl = 0, predecessors = None):
        self.name = name
        self.rt = rt
        #self.ex = 0
        self.dl = dl 
        self.successors = []
        self.predecessors = list(predecessors) if predecessors else []

        
    def addSuccessor(self,node):
        self.successors.append(node)
        
        
    def visitNode(self,node):
        if node:
            print(node.name)
        for successor in node.successors:
            self.visitNode(successor)
        

    def addSuccessorByName(self,predecessorName,node):
        if self.name == predecessorName:
            self.addSuccessor(node)
        else:
            for successor in self.successors:
                successor.addSuccessorByName(predecessorName,node)

nodes = {}

with open("/Volumes/Data/Codes/BPGC_Courses/S1_RTES/04_Assignment_1_Input.txt", "r") as inputfile:
    for line in inputfile:
        words = line.strip().split(",")
        name = ''
        rt = 1000 
        dl = 0 
        predecessors =[]
        for word in words:
            if name == '':
                name = word
            elif rt == 1000:
                rt = int(word)
            elif dl == 0:
                dl = int(word)
            elif word != 'S':
                predecessors.append(word)
        newnode = node(name, rt, dl, predecessors)
        nodes[name] = newnode
        # print(newnode.name, newnode.rt, newnode.dl, newnode.predecessors)

#SUCCESSORS
for thenode in nodes.values():
    for thepredecessor in thenode.predecessors:
        nodes[thepredecessor].addSuccessor(thenode)

# for name, n in nodes.items():
#     print(name, "->", [s.name for s in n.successors])
