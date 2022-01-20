#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
 * Counter class
 * add a member function to set Observer
 * use a vector to store pointers to counter observers
 * when limit is reached or overflow occurs
 * --> Counter passes a pointer to itself as a parameter to the observer
 * --> observer prints notification & current value
 */

class CounterObserver;
class Counter {
public:
    virtual void inc() = 0;
    virtual void dec() = 0;
    virtual void SetObserver(CounterObserver*) = 0;
    virtual operator int() = 0;
    virtual ~Counter() {};
};

class CounterObserver {
public:
    virtual void HandleLimitReached(Counter* ctr) = 0;
};

class LimitedCounter: public Counter {
public:
    LimitedCounter(int initialValue = 0, int upperLimit = 0);
    virtual void inc() override;
    virtual void dec() override;
    virtual operator int() override { return val; }
    virtual void SetObserver(CounterObserver* o) override { observers.push_back(o); }
private:
    void Notify();
    int val;
    int limit;
    vector<CounterObserver*> observers;
};

LimitedCounter::LimitedCounter(int initialValue, int upperLimit){
    val = initialValue;
    limit = upperLimit;
    observers.clear();
}

void LimitedCounter::inc() {
    if( val + 1 <= limit )
        ++val;
    else
        Notify();
}

void LimitedCounter::dec() {
    if( val - 1 >= 0 )
        --val;
    else
        Notify();
}

void LimitedCounter::Notify() {
    for(auto* obs : observers) {
        obs->HandleLimitReached(this);
    }
}

/*
 * OverflowCounter class
 * when an overflow occurs --> class OverflowCounter informs a single observer
 * add a data member
 * --> points to the Observer interface class (Observer* obs)
 * add function SetObserver
 * --> sets the pointer member to point to any object that implements Observer interface (void SetObserver(Observer *))
 * add private function Notify
 * --> informs the observer by calling the function HandleLimitReached() of the observer
 */
/*
 * HandleLimitReached function
 * pass the message “Limit has been reached” from the OverflowCounter to the observer
 */

class OverflowCounter: public Counter {
public:
    OverflowCounter(int initialValue = 0, int upperLimit = 0);
    virtual void inc() override;
    virtual void dec() override;
    virtual operator int() override { return val; }
    virtual void SetObserver(CounterObserver* o) override { observers.push_back(o); }
private:
    void Notify();

    int val;
    int limit;
    vector<CounterObserver*> observers;
};

OverflowCounter::OverflowCounter(int initialValue, int upperLimit){
    val = initialValue;
    limit = upperLimit;
    observers.clear();
}

void OverflowCounter::inc() {
    if( val + 1 <= limit ) {
        ++val;
    } else {
        val = 0;
        Notify();
    }

}

void OverflowCounter::dec() {
    if( val - 1 >= 0 ) {
        --val;
    }
    else {
        val = limit;
        Notify();
    }

}

void OverflowCounter::Notify() {
    for(auto* obs : observers) {
        obs->HandleLimitReached(this);
    }
}

/*
 * CounterUser class
 * has an OverflowCounter as a data member
 * implements the observer interface
 * constructor
 * --> limited counter is initialized with a limit value
 * --> CounterUser object (“this”) is set as the observer of the OverflowCounter
 * IncrementBy
 * --> counter is incremented n (a parameter) times
 * HandleLimitReached()
 * --> displays that the limit has been reached
 */

class CounterUser: public CounterObserver {
public:
    CounterUser(std::string s);
    void addCounter(Counter& counter);
    virtual void HandleLimitReached(Counter* ctr) override;
private:
    std::string name;
};

CounterUser::CounterUser(std::string s){
    name = s;
}

void CounterUser::addCounter(Counter& counter){
    counter.SetObserver(this);
}

void CounterUser::HandleLimitReached(Counter* ctr){
    cout << name << ": Limit has been reached: " << *ctr << endl;
}

void UseCounter(Counter& ctr, int num)
{
    if( num > 0 )
    {
        while( num > 0 ) {

            ctr.inc();
            --num;
        }
    }
    else if( num < 0 )
    {
        while( num < 0 ) {
            ctr.dec();
            ++num;
        }
    }
}

/*
 * program that creates 2 counters of both types and 3 observers
 * 2 observers monitors a single counter each
 * 1 observer monitors both
 */

int main(){
    OverflowCounter oc(5, 9);
    LimitedCounter lc(0, 5);

    CounterUser cu1("cu1");
    CounterUser cu2("cu2");
    CounterUser cu3("cu3");

    cu1.addCounter(oc);
    cu2.addCounter(lc);
    cu3.addCounter(oc);
    cu3.addCounter(lc);

    cout << "\n";
    cout << "OverFlowCounter:" << endl;
    cout << oc << endl;
    UseCounter(oc, 5);
    cout << oc << endl;
    UseCounter(oc, -1);
    cout << oc << endl;
    oc.dec();
    cout << oc << endl;
    cout << "LimitedCounter:" << endl;
    cout << lc << endl;
    lc.inc();
    cout << lc << endl;
    lc.dec();
    cout << lc << endl;
    for(int i = 0; i < 10; ++i) lc.inc();
    cout << lc << endl;
    UseCounter(lc, -9);
    cout << lc << endl;

    return 0;
}
