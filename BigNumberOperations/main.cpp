//
//  main.cpp
//  BigNumberOperations
//
//  Created by Mihai Visuian on 23/02/2015.
//  Copyright (c) 2015 Mihai Visuian. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class BigNumber {
    
private: // private area
    
    vector<short>num;
    bool sign; // boolean for sign; if positive it is 0 else it is 1
    
    short verify (long long index) { // check if index of digit is smaller than the size of number, then return digit or 0
        return index < num.size()?num[index]:0;
    }
    
public: // public area
    
    // constructors----------------
    
    BigNumber() {
        num.resize(0); // set default value to 0
        sign = 0; // set sign to positive
    }
    
    BigNumber(string const &number) { // constructor if it takes a string as a parameter
        unsigned long long n = number.size();
        sign = 0;
        if(number[0]=='-') // if the number has a minus character
            sign = 1; // set sign
        for (long long i = n-1; i>=0; i--) {
            if(isdigit(number[i]))
                num.push_back(number[i]-'0');
        }

    }
    
    BigNumber(const BigNumber &a) { // constructor for copying big numbers
        num = a.num;
        sign = a.sign;
    }
    
    BigNumber(long long x) { // constructor if it takes an integer
        if(!x)
            num.push_back(0);
        else {
            sign = 0;
            if (x < 0) {
                sign = 1; // set the number negative;
            }
            
            while (x) { // get the number and pass them in the vector instance
                num.push_back(x%10);
                x /=10;
            }
        }
    }
    
    //methods------------
    
    const char getSign() { // getter for sign of number
        return sign?'-':'+';
    }
    
    const long long getSize() { // get size of number
        return num.size();
    }
    
    bool operator > (BigNumber &b) { // overload the > operator for big numbers
        if(sign == 1 && b.sign == 0) // if a > 0 and b < 0
            return false;
        if(sign == 0 && b.sign == 1) // if b < 0 and a > 0
            return true;
        if(this->getSize() > b.getSize()) // checking the sizes
            return true-b.getSign(); // returning result according to the sign, provided they have the same sign
        if(this->getSize() < b.getSize()) // checking the sizes
            return b.sign; // returning result according to the sign, provided they have the same sign
        long long n = b.getSize();
        for (long long i = n-1; i >= 0; i-- ) {
            if(num[i]>b.num[i])
                return true - b.sign;
        }
        return false;
    }
    
    bool operator == (BigNumber &b) { // overload the == operator for big numbers
        if (getSize() != b.getSize())
            return false;
        long long n = getSize();
        for(int i = 0; i<n; i++ )
            if(num[i]!=b.num[i])
                return false;
        return true;
    }
    
    bool operator <= (BigNumber &b) { // overload the <= operator for big numbers
        return !(*this > b);
    }
    
    bool operator < (BigNumber &b) { // overload the < operator for big numbers
        return !(*this > b || *this == b);
    }
    
    bool operator >= (BigNumber &b) { // overload the >= operator for big numbers
        return !(*this < b);
    }
    
    BigNumber absolute() {
        this->sign=0;
        return *this;
    }
    
    BigNumber operator + (BigNumber b) { // adding big numbers
        int t = 0;
        BigNumber c;
        c.sign = 0;
        BigNumber zero = BigNumber(0);
        if(*this>zero && b < zero) {  // if a > 0 and b < 0
            b=b.absolute(); // change b to its absolute value
            if(*this<b){ // if absolute of b > a
                c.sign = 1; //then result is negative
                c=b-(*this); //subtract them, the result is positive
                return c;
            }
            else if (*this > b) {// if absolute of b < a
                c= *this-b; // subtract them knowing result is positive
                return c;
            }
            else if (*this == b) // if the absolute is equal to a
                return zero; // result is 0
                
        }
        if(*this<zero && b > zero) { // if a < 0 and b > 0
            *this = absolute(); // change a to its absolute value
            if(*this < b) { // if absolute of a < b
                c= b-(*this); // subtract them, the result is positive
                return c;
            }
            else if(*this>b) { // if absolute of a > b
                c.sign=1; // result is negative
                c = *this-b; // subtract them
                return c;
            }
            else if (*this == b)
                return zero;
        }
        if(sign*b.sign==1) // if they are both negative
            c.sign = 1; // then the result is negative
        unsigned long long n = max(num.size(),b.num.size());
        for (long long i = 0; i<n; i++) {
            unsigned short newDigit = verify(i)+b.verify(i)+t;
            c.num.push_back(newDigit%10);
            t = newDigit/10;
        }
        return c;
    }
    
    BigNumber operator - (BigNumber b) { // subtracting big numbers provided that A > B > 0
        BigNumber c; // declaring the result that is going to be returned
        c.sign=0; // initial sign of result is positive
        int t = 0; // initial transport variable set to 0
        BigNumber zero = BigNumber(0); // initializing 0 as a big number
        if(*this<b) { // if a < b
            c.sign = 1; // result becomes negative
            swap(*this, b); // swap their values to do b-a instead
        }
        if(b < zero && *this > zero) { // if a > 0 and b < 0
            b=b.absolute(); // change value of b to it's absolute
            return *this+b; // return the sum of a+b, knowing a-(-b)==a+b
        }
        unsigned long long n = max(num.size(),b.num.size());
        for(unsigned long long i = 0; i < n; i++ ) {
            unsigned short newDigit = (verify(i)-t<b.verify(i))*10+verify(i)-t-b.verify(i); // computing new digit we are going to push to result
            t=verify(i)-t<b.verify(i);
            c.num.push_back(newDigit);
        }
        while (c.num[--n]==0) {
            c.num.pop_back();
        }
        return c;
    }
    
    void write() {
        if(sign)
            cout<<'-';
        for(vector<short>::iterator it=num.end()-1; it>= num.begin(); it--)
            cout<<*it;
        cout<<'\n';
    }
    
};




int main()
{
    BigNumber a,b;
    a=BigNumber(233);
    a.write();
    b = BigNumber("1232");
    b.write();
    (a-b).write();
    return 0;
}
