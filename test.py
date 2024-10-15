# address 0x5f4d059142a0 p:
# address 0x5f4d059142a1 p+1:
# address 0x5f4d059142a2 p+2:

# tab = 0x7108523c0000
# tmpPtr =0x7108523c0020


# tmpPtr passed for 32 bytes
# print(tmpPtr-tab)
# so when is executed the mmap i have a new chunk of memory of 96bytes for 128
# print((96 << 7) / 96)
# print(0x7ffff7fb5060 - 0x7ffff7fb5000)
# def buddy():
#     pass
# def addNode()


# change fontaine for grenoble
# chagne remote to all france
# msc instead of engineer
# change the finance part to something that make sense in that field
# Change names of cvs pdf
# In each bullet point, I need to put capital letter
# gpa Uninorte has to be 4,39/5.0
# I have to be more clear in the uninorte calculus II / in experience below uninorte exp
# Change uninorte description in exp
# Change profile photo
# Put it in french too
# Change tilvest description to make it uniform


class Node:
    def __init__(self, value):
        self.value = value
        self.next: None | Node = None


class LinkedList:
    def __init__(self):
        self.ptr: Node | None = None


class Buddy:
    def __init__(self, size):
        if self.checkSize(size):
            self.size = size
            self.buddy = self.setBuddy()

    def setBuddy(self):
        size = self.getArraySize()
        buddy = [LinkedList() for x in range(0, size)]
        buddy[len(buddy) - 1].ptr = Node(1024)
        return buddy

    def getArraySize(self):
        counter = 0
        while self.size % 2 == 0:
            self.size /= 2
            counter += 1
        return counter

    def checkSize(self, size):
        while size % 2 == 0:
            size /= 2
        if size == 1:
            return True
        else:
            return False

    def createDobleBlock(self, index):
        ptr = Node(2**index)
        next = Node(2**index)
        ptr.next = next
        return ptr

    def assignSplittedBlock(self, index):
        index -= 1
        self.buddy[index].ptr = self.createDobleBlock(index)

    def splitBlock(self, index):
        if self.buddy[index].ptr == None:
            # print(index)
            index += 1
            self.splitBlock(index)
        else:
            self.assignSplittedBlock(index)
            # print(self.buddy[index].ptr.value)

    def request(self, blockSize):
        correctIndex = 0
        while 2**correctIndex < blockSize:
            correctIndex += 1
        # for x in self.buddy:
        # print(x)
        # print(self.buddy)
        self.splitBlock(correctIndex)


def main():
    buddy = Buddy(1024)
    buddy.request(64)

    pass
    # buddy()


if __name__ == "__main__":
    main()
