#include<boost/thread/thread.hpp>
#include<iostream>
#include<cstdlib>
using namespace std;
volatile bool isRuning =true;
void function1()
{
    static int cnt1=0;
    while(isRuning)
    {
        cout<<"function1:"<<cnt1++<<endl;
        sleep(1);
    }
}
void function2()
{
    static int cnt2=0;
    while(isRuning)
    {
        cout<<"\tfunction2:"<<cnt2++<<endl;
        sleep(1);
    }
}
int main()
{
    boost::thread thread1(&function1);
    boost::thread thread2(&function2);

    system("read");
    isRuning=false;
    thread1.join();
    thread2.join();
    cout<<"exit"<<endl;
    return 0;
}