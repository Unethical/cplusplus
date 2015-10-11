#ifndef WORKITEM_H_INCLUDED
#define WORKITEM_H_INCLUDED
class WorkItem{
    string str;
public:
    WorkItem(string s) : str(s) {}
    ~WorkItem(){}
    string get(){return str;}
};


#endif // WORKITEM_H_INCLUDED
