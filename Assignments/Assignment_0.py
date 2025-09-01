class node:
    def __init__(self,name=None, rt = 0, dl = 0, predecessors = None):
        self.name = name
        self.rt = rt
        self.dl = dl 
        self.successors = []
        self.predecessors = list(predecessors) if predecessors else []
        self.effrelease = None
        self.effdeadline = None

        
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

with open("/Volumes/Data/Codes/BPGC_Courses/S1_RTES/Assignments/Assignment_0_Input.txt", "r") as inputfile:
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


def findEffectiveTimes(nodes):

    def findEffRelease(dictionary):

        def dfs(keyname):
            n = dictionary[keyname] #n=value of the dictionary
            if n.effrelease is not None:
                return n.effrelease

            if not n.predecessors:
                n.effrelease = n.rt
            else:
                preds_er = (dfs(pname) for pname in n.predecessors)
                n.effrelease = max(n.rt, max(preds_er))

            return n.effrelease
        for keyname in dictionary:
            dfs(keyname)

    findEffRelease(nodes)

    def findEffDeadline(dictionary):

        def dfs(n):
            if n.effdeadline is not None:
                return n.effdeadline

            if not n.successors:
                n.effdeadline = n.dl
            else:
                successors_ed = (dfs(pname) for pname in n.successors)
                n.effdeadline = min(n.dl, min(successors_ed))

            return n.effdeadline
        for keyname in dictionary.values():
            dfs(keyname)

    findEffDeadline(nodes)


def printEffectiveTimes(nodes):
        for thenode in nodes.values():
            print(thenode.name, thenode.effrelease, thenode.effdeadline)
        



findEffectiveTimes(nodes)
printEffectiveTimes(nodes)
