#ifndef LIST_H
#define LIST_H

template <class T>
class Stack
{
public:
    struct List
    {
        T a;
        List *next, *prev;
    };

private:
    List *Head, *End;

public:
    Stack();
    bool empty();
    void push(T x);
    T pop();
    T top();
    ~Stack();
};

template <class T>
Stack<T>::Stack() : Head{nullptr}, End{nullptr}
{

}

template <class T>
bool Stack<T>::empty()
{
    List *temp = Head;
    if(Head != nullptr)
    {
        return false;
    } else return true;
}

template <class T>
void Stack<T>::push(T x)
{
    List *temp = new List;
    temp->next = nullptr;
    temp->a = x;
    if(Head != nullptr)
    {
        temp->prev = End;
        End->next = temp;
        End = temp;
    }
    else
    {
        temp->prev = nullptr;
        Head = End = temp;
    }
}

template <class T>
T Stack<T>::pop()
{
    T x = End->a;
    if(Head->next == nullptr)
    {
        Head = Head->next;
    }
    else
    {
        End = End->prev;
        End->next = nullptr;
    }
    return x;
}

template <class T>
T Stack<T>::top()
{
    return End->a;
}

template <class T>
Stack<T>::~Stack()
{
    while(Head)
    {
        End = Head->next;
        delete Head;
        Head = End;
    }
}

#endif // LIST_H
