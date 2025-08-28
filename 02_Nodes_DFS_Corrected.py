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
    
    def visitNode(self, seen=None):
        if seen is None:
            seen = set()

        if self.name in seen:
            return
        seen.add(self.name)
        print(self.name)
        
        for successor in self.successors:
            successor.visitNode(seen)
            
    # EDIT2 : now both in the start and at the end :)
    # Output @ 1233 : S J1 J3 J4 J2 S
        
    # def visitNode(self,node):
    #     for successor in node.successors:
    #         print(successor.name)
    #         self.visitNode(successor)
        

    def addSuccessorByName(self,predecessorName,new_node):
        if self.name == predecessorName:
            self.addSuccessor(new_node)
            return True

        for successor in self.successors:
            if successor.addSuccessorByName(predecessorName, new_node):
                return True
        return False
        
        #EDIT 1: the problem is : S is coming at the last (#1)
        #Output @ 1230 : J1 J3 J4 J2 S
        
        # for successor in node.successors:
        #     print(successor.name)
        #     if successor.name == predecessorName:
        #         successor.addSuccessor(node)
        #     else:
        #         self.addSuccessorByName(successor,node)


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

sourceNode.visitNode()


#print(sourceNode.name)