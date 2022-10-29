#include "bigint.h"
#include <vector>

BigInt::BigInt() {

    positive = true;

}

BigInt::BigInt(int val) {

    if (val < 0) {
        positive = false;
        val *= -1;
    } else {
        positive = true;
    }

    while (val) {
        digits.push_back((int) (val % base));
        val /= base;
    }

}

BigInt::BigInt(std::string val) {

    unsigned long long size = val.length();

    positive = (val[0] != '-');

    while ((size > 0) && (!positive && (size > 1))) {

        int length = 0;
        int num = 0;
        int level = 1;

        for (unsigned long long i=(size-1); i>=size-9; i--) {
            if (val[i] < '0' || val[i] > '9') break;
            num += (val[i] - '0') * level;
            level *= 10;
            ++length;
        }

        digits.push_back(num);
        size -= length;
    }
}

BigInt::BigInt(const BigInt &val) {
    digits = val.digits;
    positive = val.positive;
}

BigInt::~BigInt() {
    digits.clear();
    positive = true;
}

void module_addition(const std::vector<int>& a, const std::vector<int>& b, std::vector<int> res) {

    auto it1 = a.begin();
    auto it2 = b.begin();

    int sum = 0;

    while (it1 != a.end() || it2 != b.end()) {

        if (it1 != a.end()) {
            sum += *it1;
            ++it1;
        }

        if (it2 != b.end()) {
            sum += *it2;
            ++it2;
        }

        res.push_back (sum % BigInt::base);
        sum /= BigInt::base;
    }

    if (sum) res.push_back(1);
}

void module_subtraction(const std::vector<int>& a, const std::vector<int>& b, std::vector<int> res) {

    auto it1 = a.begin();
    auto it2 = b.begin();

    int dif = 0;

    while (it1 != a.end()) {

        if (it1 != a.end()) {
            dif += *it1;
            ++it1;
        }

        if (it2 != b.end()) {
            dif -= *it2;
            ++it2;
        }

        if (dif < 0) {
            res.push_back (dif + BigInt::base);
            dif = -1;
        } else {
            res.push_back (dif % BigInt::base);
            dif /= BigInt::base;
        }
    }
}

bool module_compare(const std::vector<int>& a, const std::vector<int>& b){

    if (a.size() != b.size()){
        return (a.size() >= b.size());
    } else{
        return (a.back() >= b.back());
    }
} //a>=b

BigInt &BigInt::operator=(const BigInt &val) {

    if (&val == this) return *this;

    positive=val.positive;
    digits=val.digits;

   /* digits.clear();

    for(int digit : val.digits){
        digits.push_back(digit);
    }
*/
    return *this;
}

BigInt &BigInt::operator+=(const BigInt &val) {

    std::vector<int> res;

    if (val.positive == this->positive) {
        module_addition(this->digits, val.digits, res);
    }else{
        if (module_compare(this->digits, val.digits)){ //большее по модулю
            module_subtraction(this->digits, val.digits, res);
        }else{
            module_subtraction(val.digits, this->digits ,res);
            this->positive=!positive;
        }
    }

    digits.clear();

    for(int & re : res){
        digits.push_back(re);
    }

    return *this;
}

BigInt &BigInt::operator-=(const BigInt &val) {

    std::vector<int> res;

    if (val.positive != this->positive) {
        module_addition(this->digits, val.digits, res);
    }else{
        if (module_compare(this->digits, val.digits)){ //большее по модулю
            module_subtraction(this->digits, val.digits, res);
        }else{
            module_subtraction(val.digits, this->digits ,res);
            this->positive=!positive;
        }
    }

    digits.clear();

    for(int & re : res){
        digits.push_back(re);
    }

    return *this;
}

BigInt &BigInt::operator++() {

    if (positive) {
        int sum = 1;
        auto it1 = digits.begin();

        while (it1 != digits.end()) {
            sum += *it1;
            *it1 = sum % base;
            sum /= base;
            if (sum == 0) break;
        }

        if (sum) digits.push_back(1);

        return *this;
    } else {
        if (digits.size()==1 && digits.back()==1) this->positive=true;

        auto it1 = digits.begin();

        int dif = -1;

        while (it1 != digits.end()) {
            dif += *it1;
            ++it1;

            if (dif < 0) {
                *it1 = dif + BigInt::base;
                dif = -1;
            } else {
                *it1 = dif;
                break;
            }

        }
        return *this;
    }
}

const BigInt BigInt::operator++(int) {

    BigInt oldValue(*this);

    ++(*this);

    return oldValue;
} //постфиксный инкремент return знач до инкр

BigInt &BigInt::operator--() {

    if (!positive) {
        int sum = 1;
        auto it1 = digits.begin();

        while (it1 != digits.end()) {
            sum += *it1;
            *it1 = sum % base;
            sum /= base;
            if (sum == 0) break;
        }

        if (sum) digits.push_back(1);

        return *this;
    } else {
        if (digits.size()==1 && digits.back()==0) this->positive=false;

        auto it1 = digits.begin();

        int dif = -1;

        while (it1 != digits.end()) {
            dif += *it1;
            ++it1;

            if (dif < 0) {
                *it1 = dif + BigInt::base;
                dif = -1;
            } else {
                *it1 = dif;
                break;
            }
        }

        if (dif!=0) digits[0]=1;
        return *this;
    }
}

const BigInt BigInt::operator--(int) {

    BigInt oldValue(*this);

    --(*this);

    return oldValue;
}

bool BigInt::operator==(const BigInt &val) const {

    if ((this->positive) != (val.positive)) return false;

    if (this->digits.size() != val.digits.size()){
        return false;
    } else{
        return (this->digits.back() == val.digits.back());
    }
}

bool BigInt::operator!=(const BigInt &val) const {
    return !(*this==val);
}

BigInt BigInt::operator+() const {
    return *this;
}

BigInt BigInt::operator-() const {

    BigInt res(*this);

    if (digits.size()==1 && digits.back()==0) return res;

    res.positive = !this->positive;

    return res;
}

bool BigInt::operator>(const BigInt &val) const {

    if (this==&val) return false;

    if (this->positive != val.positive) return this->positive;

    if(this->positive){
        return module_compare(this->digits, val.digits);
    } else{
        return !module_compare(this->digits, val.digits);
    }

}

bool BigInt::operator<(const BigInt &val) const {
    return (this > &val);
}

bool BigInt::operator<=(const BigInt &val) const {
    return !(this > &val);
}

bool BigInt::operator>=(const BigInt &val) const {
    return ((this > &val) || (this==&val));
}

BigInt BigInt::operator~() const {

    BigInt res;
    res.positive= this->positive;

    auto it= this->digits.rbegin();

   // res.digits.push_back(~*it);

    while(it!=digits.rend()){
  //      int d = *it + base;

     //   res.digits.push_back(~d);
        res.digits.push_back(~*it);

        it++;
    }


    return res;
}

BigInt operator*(const BigInt &a, const BigInt &b) {


    std::vector<int>::const_iterator it1;
    std::vector<int>::const_iterator it2;


    BigInt c;

    if ((a.digits.size()==1 && a.digits.back()==0) || (b.digits.size()==1 && b.digits.back()==0)){
        c.digits.push_back(0);
        return c;
    }

    c.positive=(a.positive==b.positive);

    for (it1 = b.digits.begin(); it1 != a.digits.end(); ++it1) {
        for (it2 = b.digits.begin(); it2 != b.digits.end(); ++it2) {
            c.digits[distance(a.digits.begin(),it1)+ distance(a.digits.begin(),it2)] += (*it1) * (*it2);
        }
    }

    return c;
}

BigInt &BigInt::operator*=(const BigInt &val) {
    *this = *this * val;
    return *this;
}

std::ostream &operator<<(std::ostream &o, const BigInt &i) {

    if (i.digits.empty()) return o << 0;

    if (i.digits.size() == 1 && i.digits.back() == 0) return o << 0;

    if (!i.positive) o << '-';


    auto it = i.digits.rbegin();

    o << *it++;


    for (; it != i.digits.rend(); ++it) {

        int length = 0;


        int current=BigInt::base;
        while (current) {
            current /= 10;
            length++;
        }

        current = *it;

        while (current) {
            current /= 10;
            --length;
        }

        for (int j = 0; j > length; ++j) o << '0';


        if (*it) o << *it;
    }

    return o;

}

BigInt operator+(const BigInt &a, const BigInt &b) {
    BigInt res(a);
    res+=b;
    return res;
}

BigInt operator-(const BigInt &a, const BigInt &b) {

    BigInt res(a);
    res-=b;
    return res;
}

size_t BigInt::size() const {
    return this->digits.size();
}

BigInt::operator int() const {

    int res=0;
    int k=1;

    for(int digit : this->digits){
        res+=digit*k;
        k*=BigInt::base;
    }

    return res;
}

BigInt::operator std::string() const {

    std::string o;

    if (this->digits.size() == 1 && this->digits.back() == 0) return o = '0';

    if (!this->positive) o.push_back('-');


    auto it = this->digits.rbegin();


    o+=std::to_string(abs(*it++));



    for (; it != this->digits.rend(); ++it) {

        int length = 0;

        int current=BigInt::base;
        while (current) {
            current /= 10;
            length++;
        }

        current = *it;

        while (current) {
            current /= 10;
            --length;
        }

        for (int j = 0; j > length; ++j) o.push_back('0');

        o+=std::to_string((abs(*it)));
    }

    return o;

}

BigInt operator/(const BigInt &a, const BigInt &b) {

    BigInt c;

    if((a.digits.size()==1 && a.digits.back()==0) || (a<b)){
        c.digits.push_back(0);
        return c;
    }

    c.positive=(a.positive==b.positive);


    BigInt d(a);

    while(module_compare(d.digits, b.digits)){
        d-=b;
        c++;
    }

    return c;
}

BigInt &BigInt::operator/=(const BigInt &val) {
    *this = *this / val;
    return *this;
}

BigInt operator^(const BigInt &a, const BigInt &b) {

    BigInt c(1);

    if(!a.positive && (b.digits[0] % 2 == 1)) c.positive = false;

    BigInt d = b;

    while(d.digits.size()==1 && d.digits.back()==0){
        c=a*c;
        d--;
    }

    return c;
}

BigInt &BigInt::operator^=(const BigInt &val) {
    *this = *this ^ val;
    return *this;
}

BigInt operator&(const BigInt &a, const BigInt &b) {

    BigInt res;
    res.positive= a.positive == b.positive;

    auto it1= a.digits.rbegin();
    auto it2= b.digits.rbegin();

    while(it1!=a.digits.rend() && it2!=a.digits.rend()){

        res.digits.push_back(*it1 & *it2);

        it1++;
        it2++;
    }

    return res;
}

BigInt &BigInt::operator&=(const BigInt &val) {
    *this = *this & val;
    return *this;
}

BigInt operator|(const BigInt &a, const BigInt &b) {
    BigInt res;
    res.positive= a.positive == b.positive;

    auto it1= a.digits.rbegin();
    auto it2= b.digits.rbegin();

    while(it1!=a.digits.rend() || it2!=a.digits.rend()){

        res.digits.push_back(*it1 || *it2);

        it1++;
        it2++;
    }

    return res;
}

BigInt &BigInt::operator|=(const BigInt &val) {
    *this = *this | val;
    return *this;
}

BigInt operator%(const BigInt &a, const BigInt &b) {

    BigInt res(a);

    res/=100;
    res*=b;
    return res;
}

BigInt &BigInt::operator%=(const BigInt &val) {
    *this = *this % val;
    return *this;
}





