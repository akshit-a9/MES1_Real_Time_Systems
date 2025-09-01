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
        for successor in node.successors:
            print(successor.name)
            self.visitNode(successor)
        

    def addSuccessorByName(self,predecessorName,node):
        for successor in node.successors:
            print(successor.name)
            if successor.name == predecessorName:
                successor.addSuccessor(node)
            else:
                self.addSuccessorByName(successor,node)


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


print(sourceNode.name)

#sourceNode.addSuccessor(myNode)
#
#myNode = node('J2')
#myNode.setRt(0)
#myNode.setDl(7)
#
#sourceNode.addSuccessor(myNode)
#
#
#myNode = node('J3')
#myNode.setRt(1)
#myNode.setDl(12)
#
#sourceNode.addSuccessor('J1',myNode)
#
#sourceNode.successors[0].addSuccessor(myNode)
#
#myNode = node('J4')
#myNode.setRt(1)
#myNode.setDl(12)
#
#sourceNode.successors[0].addSuccessor(myNode)
#
#
#sourceNode.visitNode(sourceNode)