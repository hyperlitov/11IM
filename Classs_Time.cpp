#include <iostream>

class Time{

private:

    int second;
    int minute;
    int hour;

public:

    Time(int a, int b, int c){
        second = c;
        minute = b;
        hour = a;
    }

    Time(){
        second = 0;
        minute = 0;
        hour = 0;
    }



};


class Date{

private:

    Time time;
    int day;
    int month;
    int year;

public:

    Date(int d, int m, int y, Time t = Time()){
        day = d;
        month = m;
        year = y;
    }

};


int main(){

}