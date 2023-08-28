//
// Created by Efe Cinar on 23.07.2023.
//

#ifndef CS_300_HW1_MAZESTACK_H
#define CS_300_HW1_MAZESTACK_H

template<class Object>
class MazeStack{
private:
    struct StackNode {
        Object element;
        StackNode *next;
        explicit StackNode(const Object &theElement, StackNode *n = nullptr): element(theElement), next(n) {}
    };
    StackNode *topOfStack;
    int nodeCount;

public:
    MazeStack();
    MazeStack(const MazeStack & rhs);
    ~MazeStack();

    bool isEmpty() const;
    void makeEmpty();

    void pop();
    void push(const Object & x);
    const Object topAndPop();
    const Object & top() const;
    int elementNum() const;

    const MazeStack & operator=( const MazeStack & rhs );

    // member functions to use if the Object is MazeNode
    bool findMazeCell(int ro, int co); // returns true if the stack has the MazeNode with the specific x and y values
    Object & findNReturnCell(int ro, int co); // returns the MazeNode in the stack with the specific x and y values
};

template<class Object>
Object & MazeStack<Object>::findNReturnCell(int ro, int co) {
    StackNode * ptr = topOfStack;
    while (ptr!= nullptr) {
        if (ptr->element.y == ro && ptr->element.x == co) {
            return ptr->element;
        }
        ptr = ptr->next;
    }
    /* no need to cover the "if it's empty" case since this function is used in the program
    when it's certain that stack is not empty */
}

template<class Object>
int MazeStack<Object>::elementNum() const {
    return nodeCount;
}

template<class Object>
bool MazeStack<Object>::findMazeCell(int ro, int co) {
    StackNode * ptr = topOfStack;
    while (ptr!= nullptr){
        if (ptr->element.y == ro && ptr->element.x == co){
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

template<class Object>
MazeStack<Object>::MazeStack() {
    topOfStack = nullptr;
    nodeCount = 0;
}

template<class Object>
MazeStack<Object>::MazeStack(const MazeStack &rhs) {
    topOfStack = nullptr;
    nodeCount = 0;
    *this = rhs;
}

template<class Object>
MazeStack<Object>::~MazeStack() {
    makeEmpty();
}

template<class Object>
const Object & MazeStack<Object>::top() const{
    return topOfStack->element;
}

template<class Object>
bool MazeStack<Object>::isEmpty() const{
    return topOfStack == nullptr;
}

template <class Object>
void MazeStack<Object>::pop()
{
    nodeCount--;
    if (isEmpty())
        return;
    StackNode *oldTop = topOfStack;
    topOfStack = topOfStack->next;
    delete oldTop;
}

template<class Object>
void MazeStack<Object>::makeEmpty() {
    while(!isEmpty()){
        pop();
    }
    nodeCount = 0;
}

template <class Object>
void MazeStack<Object>::push( const Object & x )
{
    topOfStack = new StackNode(x, topOfStack);
    nodeCount++;
}

template <class Object>
const Object MazeStack<Object>::topAndPop()
{
    Object topItem = top();
    pop();
    return topItem;
}

template <class Object>
const MazeStack<Object> & MazeStack<Object>:: operator=( const MazeStack<Object> & rhs )
{
    if (this != &rhs)
    {
        makeEmpty();
        if (rhs.isEmpty())
            return *this;

        StackNode *rptr = rhs.topOfStack;
        StackNode *ptr  = new StackNode(rptr->element);
        topOfStack = ptr;

        for (rptr = rptr->next; rptr != nullptr; rptr = rptr->next)
            ptr = ptr->next = new StackNode(rptr->element);
        this->nodeCount = rhs.nodeCount;
    }
    return *this;
}

#endif //CS_300_HW1_MAZESTACK_H
