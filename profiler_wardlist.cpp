#include <iostream>
#include <random>
#include <chrono>

using std::cout;
using std::endl;

double get_time()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()/1e6;
}

int rand_uns(int min, int max)
{
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        static std::default_random_engine e(seed);
        std::uniform_int_distribution<int> d(min, max);
        return d(e);
}

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

int erase_where(subforwardlist** start, unsigned int where)
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
    int i;
    --where;
    for(i = 0; i < where; ++i)
        temp = temp->next;
    subforwardlist * temp1 = temp->next;
    i = temp->next->data;
    temp->next = temp->next->next;
    delete temp1;
    temp1 = NULL;
    return i;
}



int main()
{
    int n = 10000;
    int *test_sequence = new int[n], sum_for_O3 = 0, sum_check = 0, test_sequence_sum = 0, ongoing_sum = 0,
        *push_sequence = new int[n],
        *pop_sequence = new int[n],
        *pop_push_sequence_eq = new int[n],
        *pop_push_sequence_push = new int[n],
        *pop_push_sequence_pushpush = new int[n],
        *four_ways_test = new int[n];
    double start = 0, finish = 0, total = 0;
    cout << std::fixed;
    cout.precision(4);

//----------- Initialization

    start = get_time();
    for (int i = 0; i < n; i++)
    {
        test_sequence[i] = rand_uns(0, n - 1);
        test_sequence_sum += test_sequence[i];
        push_sequence[i] = rand_uns(0, n - 1);
        pop_sequence[i] = rand_uns(0, n - 1);
        pop_push_sequence_eq[i] = rand_uns(0, 1);
        pop_push_sequence_push[i] = rand_uns(0, 5);
        pop_push_sequence_pushpush[i] = rand_uns(0, 10);
        four_ways_test[i] = rand_uns(0, 3);
    }


    finish = get_time();

    cout << "Test sequence initialization: \t\t\t\t" << finish - start << endl;

    subforwardlist *sv;
    init(&sv);

//----------- Test 000 Straight push_back

    start = get_time();
    for (int i = 0; i < n; i++)
    {
        push_back(&sv, test_sequence[i]);
    }
    finish = get_time();

    cout << "000 Straight push_back: \t\t\t\t" << finish - start << endl;
    total += finish - start;

//----------- Test 001 Straight pop_forward

    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_forward(&sv) != test_sequence[i])
        {
            cout <<endl <<"--- !!! Failed push/pop consistency !!! ---" << endl;
            return 0;
        }
    }
    finish = get_time();

    if (size(&sv))
    {
        cout <<endl <<"--- !!! Failed push/pop consistency, some elememts stuck !!! ---" << endl;
        return 0;
    }

    cout << "001 Straight pop_forward: \t\t\t\t" << finish - start << endl;
    total += finish - start;


//----------- Test 002 Straight push_forward

    start = get_time();
    for (int i = 0; i < n; i++)
    {
        push_forward(&sv, test_sequence[i]);
    }
    finish = get_time();

    cout << "002 Straight push_forward: \t\t\t\t" << finish - start << endl;
    total += finish - start;

//----------- Test 003 Straight pop_back

    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_back(&sv) != test_sequence[i])
        {
            cout <<endl <<"--- !!! Failed push/pop consistency !!! ---" << endl;
            return 0;
        }
    }
    finish = get_time();

    if (size(&sv))
    {
        cout <<endl <<"--- !!! Failed push/pop consistency, some elememts stuck !!! ---" << endl;
        return 0;
    }

    cout << "003 Straight pop_back: \t\t\t\t\t" << finish - start << endl;
    total += finish - start;

//----------- Test 004 Random push_where

    for (int i = 0; i < n; i++)
    {
        push_back(&sv, test_sequence[i]);
    }

    start = get_time();
    for (int i = 0; i < n; i++)
    {
        push_where(&sv, push_sequence[i], test_sequence[i]);
    }
    finish = get_time();

    if (size(&sv) != 2 * n)
    {
        cout <<endl <<"--- !!! Failed push/pop consistency, wrong elements number !!! ---" << endl;
        return 0;
    }

    ongoing_sum = 0;
    for (int i = 0; i < 2 * n; i++)
    {
        ongoing_sum += pop_back(&sv);
    }

    if (size(&sv))
    {
        cout <<endl <<"--- !!! Failed push/pop consistency, some elememts stuck !!! ---" << endl;
        return 0;
    }

    if (ongoing_sum != 2 * test_sequence_sum)
    {
        cout <<endl <<"--- !!! Failed push/pop consistency, sum incoherent !!! ---" << endl;
        return 0;
    }

    cout << "004 Random push_where: \t\t\t\t\t" << finish - start << endl;
    total += finish - start;

//----------- Test 005 Random erase_where

    for (int i = 0; i < 2 * n; i++)
    {
        push_back(&sv, test_sequence[i % n]);
    }

    sum_for_O3 = 0;
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        sum_for_O3 += erase_where(&sv, pop_sequence[i]);
    }
    finish = get_time();

    if (size(&sv) != n)
    {
        cout <<endl <<"--- !!! Failed push/pop consistency, wrong elements number !!! ---" << endl;
        return 0;
    }

    clear(&sv);

    if (size(&sv))
    {
        cout <<endl <<"--- !!! Clear works wrongly !!! ---" << endl;
        return 0;
    }

    cout << "005 Random erase_where: \t\t\t\t" << finish - start << " \t\t" << sum_for_O3 << endl;
    total += finish - start;


//----------- Test 006 Random pop/push back equal amount

    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_eq[i])
            push_back(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_back(&sv);
    }
    finish = get_time();

    clear(&sv);

    cout << "006 Random pop/push back equal amount: \t\t\t" << finish - start << "\t\t" << sum_for_O3 << endl;
    total += finish - start;

//----------- Test 007 Random pop/push back more push

    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_push[i])
            push_back(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_back(&sv);
    }
    finish = get_time();

    clear(&sv);

    cout << "007 Random pop/push back more push: \t\t\t" << finish - start << "\t\t" << sum_for_O3 << endl;
    total += finish - start;

//----------- Test 008 Random pop/push back much more push

    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_pushpush[i])
            push_back(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_back(&sv);
    }
    finish = get_time();

    clear(&sv);

    cout << "008 Random pop/push back much more push: \t\t" << finish - start <<"\t\t" << sum_for_O3 << endl;
    total += finish - start;

//----------- Test 009 Random pop/push forward equal amount

    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_eq[i])
            push_forward(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_forward(&sv);
    }
    finish = get_time();

    clear(&sv);

    cout << "009 Random pop/push forward equal amount: \t\t" << finish - start << "\t\t" << sum_for_O3 << endl;
    total += finish - start;

//----------- Test 010 Random pop/push forward more push

    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_push[i])
            push_forward(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_forward(&sv);
    }
    finish = get_time();

    clear(&sv);

    cout << "010 Random pop/push forward more push: \t\t\t" << finish - start << "\t\t" << sum_for_O3 << endl;
    total += finish - start;

//----------- Test 011 Random pop/push forward much more push

    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (pop_push_sequence_pushpush[i])
            push_forward(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_forward(&sv);
    }
    finish = get_time();

    clear(&sv);

    cout << "011 Random pop/push forward much more push: \t\t" << finish - start <<"\t\t" << sum_for_O3 << endl;
    total += finish - start;

//----------- Test 012 Random pop/push four ways

    sum_for_O3 = 0; // This variable will be printed so O3 won't cut the whole thing.
    start = get_time();
    for (int i = 0; i < n; i++)
    {
        if (four_ways_test[i] == 0)
            push_back(&sv, test_sequence[i]);
        else if (four_ways_test[i] == 1)
            sum_for_O3 += pop_back(&sv);
        else if (four_ways_test[i] == 2)
            push_forward(&sv, test_sequence[i]);
        else
            sum_for_O3 += pop_forward(&sv);

    }
    finish = get_time();

    clear(&sv);

    cout << "012 Random pop/push four ways: \t\t\t\t" << finish - start << "\t\t" << sum_for_O3 << endl;
    total += finish - start;

//----------- End of tests
//
    cout << "-----------" << endl <<"Alltests finished, total time: \t" << total << endl;

    delete[] test_sequence;
    delete[] pop_push_sequence_eq;
    delete[] pop_push_sequence_push;
    delete[] pop_push_sequence_pushpush;
    delete[] push_sequence,
    delete[] pop_sequence,
    delete[] four_ways_test;


    return 0;
}
