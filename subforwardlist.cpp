#include <iostream>

using namespace std;

struct subforwardlist
{
    int data;
    subforwardlist * next;

};

bool init(subforwardlist** start)
{
    *start = new subforwardlist;
    *start = NULL;
}

bool init_plus(subforwardlist** start, int data)
{
    subforwardlist* temp1 = new subforwardlist;
    temp1->data = data;
    temp1->next = *start;
    *start = temp1;
}

bool print_list(subforwardlist** start) {
    subforwardlist * temp = *start;
    if (!temp) return 0;
    cout <<temp->data << " ";
    print_list(&temp->next);
}
void clear(subforwardlist** start)
{
    if (!start) return;
    clear(&((*start)->next));
    delete *start;
    *start = NULL;
}

int size(subforwardlist** start)
{
    subforwardlist * temp = *start;
    if(temp == NULL)
        return 0;
    int i = 1;
    while(temp->next != NULL)
    {
        temp = temp->next;
        ++i;
    }
    return i;
}

bool push_forward(subforwardlist** start, int data)
{

     if (*start == NULL)
     {
         init_plus(start, data);
         return 0;
     }
     subforwardlist* temp1 = new subforwardlist;
     temp1->data = data;
     temp1->next = *start;
     *start = temp1;

}

bool push_back(subforwardlist** start, int data)
{
     if (*start == NULL)
     {
        init_plus(start, data);
        return 0 ;
     }
     subforwardlist * temp = *start;
     while(temp->next != NULL)
        temp = temp->next;
     subforwardlist * temp1 = new subforwardlist;
     temp1->data = data;
     temp1->next = NULL;
     temp->next = temp1;

}

int pop_back(subforwardlist** start)
{
     int i;
     subforwardlist * temp = *start;
     if(temp == NULL)
        return 0;
     if(temp->next == NULL)
     {
         i = temp->data;
         delete *start;
         *start = NULL;
         return i;
     }
     while(temp->next->next != NULL)
        temp = temp->next;
     i = temp->next->data;
     delete temp->next;
     temp->next = NULL;
     return i;
}

int pop_forward(subforwardlist** start)
{
   int i;
   subforwardlist * temp = *start;
   if(temp == NULL)
        return 0;
   i = temp->data;
   *start = (*start)->next;
   delete temp;
   return i;
}

bool push_where(subforwardlist** start, unsigned int where, int data)
{
    subforwardlist * temp = *start;
    if(where == 0)
    {
        push_forward(start, data);
        return 0;
    }
    if(where == size(start) - 1)
    {
        push_back(start, data);
        return 0;
    }
    unsigned int i;
    subforwardlist * temp1 = new subforwardlist;
    temp1->data = data;
    --where;
    for(i = 0; i < where; ++i)
        temp = temp->next;
    temp1->next = temp->next;
    temp->next = temp1;

}

bool erase_where(subforwardlist** start, unsigned int where)
{
    subforwardlist * temp = *start;
    if(where > size(start) - 1)
        return false;
    if(where == 0)
    {
        pop_forward(start);
        return 0;
    }
    if(where == size(start) - 1)
    {
        pop_back(start);
        return 0;
    }
    unsigned i;
    --where;
    for(i = 0; i < where; ++i)
        temp = temp->next;
    subforwardlist * temp1 = temp->next->next;
    delete temp->next;
    temp->next = temp1;
}



int main()
{
    subforwardlist* start;
    init(&start);
    print_list(&start); cout << endl;
    int i = 0;
    for(i - 0; i < 1000; ++i)
        push_back(&start, i);
    print_list(&start);cout << "\n\n\n\n";
    //for(i = 0; i < 100; ++i)
    erase_where(&start, 5);
    print_list(&start); cout << " " << size(&start);
    clear(&start); cout << endl;
    print_list(&start);
    delete start;
    return 0;
}
