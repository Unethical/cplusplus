/*
    Author: Jose Sepulveda
    Date:   10/08/2015
    Description:
            This is my implementation of a circular doubly-linked list.


*/
#ifndef CIRCDLL_INCLUDED
#define CIRCDLL_INCLUDED

using namespace std;

template<class T>
class CircDLL{
    private:
        // This is a struct that represents a node in the list.
        struct Node{
            Node* next;
            Node* prev;
            T data;
        };
        Node* _head;
        int count;

    public:

        CircDLL();
        bool insertNode(T data, int pos);
        bool deleteNode(int pos);
        bool getData(T &s, int pos);

        // Returns a pointer to the node in position pos.
        // If the pos is out of bounds it returns a nullptr.
        Node* goToNode(int pos){
            if((pos > count) && (pos < 0)){
                return nullptr;
            }
            Node* pCurr = _head;
            for(int i=0; i<pos; i++){
                pCurr = pCurr->next;
            }
            return pCurr;
        }

        void print();
        void print(int pos);
        void printReversed();
        int getCount(){return count;}
        ~CircDLL();
};
    // Constructor, initializes the doubly linked list and points it's
    // head to a sentinel node.
    template<class T>
    CircDLL<T>::CircDLL(){
            _head = new Node;
            _head->next =_head;
            _head->prev = _head;
            count = 0;
        };

    // Appends node in empy list, or inserts it at position pos.
    // Returns false if given invalid position.
    template<class T>
    bool CircDLL<T>::insertNode(T data, int pos){
        Node* temp = new Node;
        temp->data = data;
        if(count == 0){
            _head->next = temp;
            temp->prev = _head;
            _head->prev = temp;
            temp->next = _head;
            count++;
            return true;
        } else if((pos <= count) && (pos >= 0)) {
            Node* pCurr = _head;
            for(int i=0; i<pos; i++){
                pCurr = pCurr->next;
            }
            Node* pNext = pCurr->next;
            pCurr->next = temp;
            temp->prev = pCurr;
            temp->next = pNext;
            pNext->prev = temp;

            count++;
            return true;
        }


        return false;
        };

        // Deletes node at given position only if the pointer is not null,
        // If given position returns null, then deleteNode returns false.
        template<class T>
        bool CircDLL<T>::deleteNode(int pos){
            Node* pCurr = goToNode(pos);
            if(pCurr != nullptr){
                Node* pNext = pCurr->next;
                Node* pPrev = pCurr->prev;
                pPrev->next = pNext;
                pNext->prev = pPrev;
                delete pCurr;
                count--;
                return true;
            }
            return false;
        };

        // Prints list sequentially rom the start of the list.
        // Assumes your datatype T is printable
        template<class T>
        void CircDLL<T>::print(){
            Node* temp = _head->next;
            for(int i=0; i < count; i++){
                cout << setw(4) << i+1 << " | " << temp->data << "\n";
                temp = temp->next;
            }
        }

        //  Prints list in reverse from the end of the list.
        //  Assums your datatype T is printable
        template<class T>
        void CircDLL<T>::printReversed(){
            Node* temp = _head->prev;
            for(int i=count; i >= 1; i--){
                cout << setw(4) << i << " | " << temp->data << "\n";
                temp = temp->prev;
            }
        }

        // Prints node at given position i,
        // Assumes your datatype T is printable.
        template<class T>
        void CircDLL<T>::print(int i){
            Node* pCurr = goToNode(i);
            if(pCurr != nullptr){
                cout << pCurr->data;
            }
        }

        // Destructor, deletes every node in list.
        template<class T>
        CircDLL<T>::~CircDLL(){
            Node* pCurr = _head->next;
            _head->next = nullptr;
            while(pCurr != nullptr){
                Node* pNext = pCurr->next;
                delete pCurr;
                pCurr = pNext;
            }

        };

        //Saves data stored at position pos to the reference variable d
        template<class T>
        bool CircDLL<T>::getData(T &d, int pos){
            Node* pCurr = _head;
            for(int i = 1; i <= pos; i++){
                pCurr = pCurr->next;

            }
            d = pCurr->data;
            return true;
        }




#endif // CIRCDLL_INCLUDED
