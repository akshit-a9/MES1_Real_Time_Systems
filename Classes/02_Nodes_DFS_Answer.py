class node:
    def __init__(self,name=None):
        self.name = name
        self.rt = 0
        self.ex = 0
        self.dl = 0
        self.successors = []
        
    def setRt(self,rt):
        self.rt = rt
        
    def setEx(self,ex):
        self.ex = ex
             
    def setDl(self,dl):
        self.dl = dl
        
        
    def getRt(self):
        return self.rt
        
    def getEx(self):
        return self.ex
          
    def getDl(self):
        return self.dl
        
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


sourceNode = node('S')
        
myNode = node('J1')
myNode.setRt(2)
myNode.setDl(10)

sourceNode.addSuccessorByName('S',myNode)

myNode = node('J2')
myNode.setRt(2)
myNode.setDl(10)

sourceNode.addSuccessorByName('S',myNode)

myNode = node('J3')
myNode.setRt(2)
myNode.setDl(10)

sourceNode.addSuccessorByName('J1',myNode)


myNode = node('J4')
myNode.setRt(2)
myNode.setDl(10)

sourceNode.addSuccessorByName('J1',myNode)

sourceNode.visitNode(sourceNode)