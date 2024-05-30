import random as rand
import sys

class Algorithm:
    def __init__ (self, page_reference, frame):
        self.page_reference = page_reference
        self.frame = frame
        self.fault = 0
    
    def page_fault(self, page, i):
        pass

    def no_fault(self, page):
        pass

    def get_fault(self):
        self.fault = 0
        for i in range(len(self.page_reference)):
            page = self.page_reference[i]
            if page not in self.frame:
                self.fault += 1
                self.page_fault(page, i)
                # print(self.frame, "Fault")
            else:
                self.no_fault(page)
                # print(self.frame)
        return self.fault

class Fifo(Algorithm):
    def page_fault(self, page, i):
        self.frame.pop(0)
        self.frame.append(page)

    def no_fault(self, page):
        pass

class Lru(Algorithm):
    def page_fault(self, page, i):
        self.frame.pop(0)
        self.frame.append(page)

    def no_fault(self, page):
        self.frame.remove(page)
        self.frame.append(page)

class Optimal(Algorithm):
    def get_max_index(self, page, i):
        max_index = 0
        try:
            max_page = self.page_reference.index(self.frame[0], i)
        except ValueError:
            max_page = int(1e9)
        for j in range(1, len(self.frame)):
            page = self.frame[j]
            try: 
                index = self.page_reference.index(page, i)
            except ValueError:
                index = int(1e9)
            if index > max_page:
                max_index = j
                max_page = index
        return max_index

    def page_fault(self, page, i):
        if -1 in self.frame:
            self.frame[self.frame.index(-1)] = page
        else:
            max_index = self.get_max_index(page, i)
            self.frame[max_index] = page

    def no_fault(self, page):
        pass

def get_page_references(n):
    page_referennce = []
    for i in range(n):
        page = rand.randint(0, 9)
        page_referennce.append(page)
    return page_referennce

def main():

    if len(sys.argv) != 2:
        print("Usage: python page.py <frame_size>")
        sys.exit(1)
    frame_size = int(sys.argv[1])
    
    page_reference = get_page_references(20)
    print(page_reference)
    frame = [-1] * frame_size

    fifo = Fifo(page_reference, frame)
    lru = Lru(page_reference, frame)
    optimal = Optimal(page_reference, frame)

    fifo_fault = fifo.get_fault()
    lru_fault = lru.get_fault()
    optimal_fault = optimal.get_fault()

    print("Fifo Fault: ", fifo_fault)
    print("Lru Fault: ", lru_fault)
    print("Optimal Fault: ", optimal_fault)

main()