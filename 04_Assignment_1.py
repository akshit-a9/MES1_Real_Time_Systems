class node:
    def __init__(self,name=None, rt = 0, dl = 0, successors = []):
        self.name = name
        self.rt = rt
        #self.ex = 0
        self.dl = dl 
        self.successors = successors

    #def setEx(self,ex):
        #self.ex = ex
             
        
    def setRt(self,rt):
        self.rt = rt
        
    def setDl(self,dl):
        self.dl = dl
        
        
    # def getEx(self):
    #     return self.ex

    def getRt(self):
        return self.rt
          
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

with open("/Volumes/Data/Codes/BPGC_Courses/S1_RTES/04_Assignment_1_Input.txt", "r") as inputfile:
    for line in inputfile:
        words = line.strip().split(",")
        name = ''
        rt = 1000 
        dl = 0 
        successors =[]
        for word in words:
            if name == '':
                name = word
            elif rt == 1000:
                rt = word
            elif dl == 0:
                dl = word
            else: successors.append(word)
        newnode = node(name, rt, dl, successors)
        print(newnode.name, newnode.rt, newnode.dl, newnode.successors)






# myNode = node('J1')
# myNode.setRt(2)
# myNode.setDl(10)
# sourceNode.addSuccessorByName('S',myNode)
# myNode = node('J2')
# myNode.setRt(2)
# myNode.setDl(10)
# sourceNode.addSuccessorByName('S',myNode)
# myNode = node('J3')
# myNode.setRt(2)
# myNode.setDl(10)
# sourceNode.addSuccessorByName('J1',myNode)
# myNode = node('J4')
# myNode.setRt(2)
# myNode.setDl(10)
# sourceNode.addSuccessorByName('J1',myNode)
# sourceNode.visitNode(sourceNode)