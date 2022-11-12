#include "bigint.h"
#include <vector>
#include <algorithm>

BigInt::BigInt() {

    positive = true;

}

BigInt::BigInt(int val) {

    if (val < 0) {
        val=-val;
        positive = false;

    } else {
        positive = true;
    }


    if (val==0) digits.push_back(0);

    while (val) {

        digits.push_back((int) (  val % base));
        val /= base;
    }

}

BigInt::BigInt(std::string val) {

    positive = (val[0] != '-');


    int len_base = 0;
    int current = BigInt::base;

    while (current >= 10) {
        current /= 10;
        len_base++;
    }


    int l = val.size();
    if (!positive) l--;

    l /= len_base;

    current=val.size()-1;

    for (int i = 0; i < l + 1; i++) {

        int num = 0;
        int level = 1;
        for (int j = 0; j < len_base; j++) {
            num += (val[current] - '0') * level;
            level *= 10;
            current--;
            if ((current == -1) || (val[current] == '-')) break;
        }

        digits.push_back(num);

    }

    this->delete0();
}


BigInt::BigInt(const BigInt &val) {
    digits = val.digits;
    positive = val.positive;
}

BigInt::~BigInt() {
    digits.clear();
    positive = true;
}



void module_addition(const std::vector<int>& a, const std::vector<int>& b, std::vector<int> &res) {

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

void module_subtraction(const std::vector<int>& a, const std::vector<int>& b, std::vector<int> &res) {

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

        for(int i = a.size()-1; i>=0; i--){

            if (a[i]<b[i]) return false;
            if (a[i]>b[i]) return true;
        }

        return true;

    }
} //a>=b

BigInt &BigInt::operator=(const BigInt &val) {

    if (&val == this) return *this;

    positive=val.positive;
    digits=val.digits;

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


       this->digits.erase(digits.begin(), digits.end());

       for(int & re : res){
           this->digits.push_back(re);
       }

    this->delete0();

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

    this->digits.erase(digits.begin(), digits.end());

    for(int & re : res){
        this->digits.push_back(re);
    }

    this->delete0();

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
            it1++;
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
            it1++;
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

                *(it1-1) = dif;
                return *this;
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


    if ((positive) != (val.positive)) return false;

    if (this->digits.size() != val.digits.size()){
        return false;
    } else{
        for(int i=0; i< digits.size(); i++){

            if (digits[i] != val.digits[i]) return false;

        }

        return true;
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

bool BigInt::operator>(const BigInt &val) const { //всегда true

    if ( *this == val) {
        return false;
    }

    if (this->positive != val.positive) return this->positive;

    if(this->positive){
        return module_compare(this->digits, val.digits);
    } else{
        return !module_compare(this->digits, val.digits);
    }

}

bool BigInt::operator<(const BigInt &val) const {
    return !(*this > val ) && *this!=val;
}

bool BigInt::operator<=(const BigInt &val) const {
    return ((*this > val) || (*this == val));
}

bool BigInt::operator>=(const BigInt &val) const {
    return ((*this > val) || (*this == val));
}

BigInt BigInt::operator~() const {

    BigInt res(*this);
    res += 1;

    res.positive= !res.positive;

    return res;

}

BigInt operator*(const BigInt &a, const BigInt &b) {

    BigInt c(0);


    std::vector<int> res(b.digits.size() + a.digits.size()) ;

    for(int & re : res){
        re=0;
    }

    for (int i = 0; i < b.digits.size(); i++) {

        for (int j = 0; j < a.digits.size(); j++) {

            int cur = i + j;

            int sum = b.digits[i] * a.digits[j];
            res[cur] += sum % BigInt::base;
            cur++;

            sum /= BigInt::base;


            while (sum) {
                sum += res[cur];
                res[cur] = sum % BigInt::base;
                sum /= BigInt::base;
                cur++;
            }


        }

    }

    c.digits=res;

    c.delete0();

    c.positive= a.positive == b.positive;


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


    if (!this->positive) o.push_back('-');


    auto it = this->digits.rbegin();


    o+=std::to_string(*it++);


    for (; it != this->digits.rend(); ++it) {

        int length = 0;

        int current=BigInt::base;

        while (current-1) {  //сколько чисел вмещает ячейка
            current /= 10;
            length++;
        }

        current = *it;

        while (current) {   //сколько нужно 0
            current /= 10;
            --length;
        }

        for (int j = 0; j < length; ++j) o.push_back('0');

        o+=std::to_string(*it);
    }

    return o;

}

BigInt operator/(const BigInt &a, const BigInt &b) {

    BigInt c(0);


    if(((a.digits.size()==1 && a.digits.back()==0)) ||( (a<b))){
        return c;
    }


    BigInt d(a);
    d.positive = true;

    BigInt f(b);
    f.positive = true;


    while(d >= f){
        d-=f;
        c++;
    }
    
    c.positive=(a.positive==b.positive);

    return c;
}

BigInt &BigInt::operator/=(const BigInt &val) {
    *this = *this / val;
    return *this;
}


BigInt operator%(const BigInt &a, const BigInt &b) {

    BigInt d(a);
    d.positive = true;

    BigInt f(b);
    f.positive = true;


    while(d >= f){
        d-=f;
    }

    return d;
}

BigInt &BigInt::operator%=(const BigInt &val) {
    *this = *this % val;
    return *this;
}

void BigInt::delete0() {

    auto it=this->digits.rbegin();

    
    while (it!=this->digits.rend() - 1 ){
        if (*it==0) digits.pop_back();
        it++;
    }

    if (digits.size()==1 && digits.back()==0) this->positive=true;

}



BigInt &BigInt::operator^=(const BigInt &value) {

    for(int i = 0; i < std::min(value.digits.size(), digits.size()); ++i){
        digits[i] = digits[i] ^ value.digits[i];
    }

    positive = value.positive == positive;
    delete0();

    return *this;
}


BigInt &BigInt::operator&=(const BigInt &vec) {

    for (int i = 0; i < std::min(digits.size(), vec.digits.size()); ++i) {
        digits[i] = digits[i] & vec.digits[i];
    }

    positive = vec.positive | positive;

    delete0();

    return *this;
}


BigInt &BigInt::operator|=(const BigInt &vec) {

    for (int i = 0; i < std::min(digits.size(), vec.digits.size()); ++i) {
        digits[i] = digits[i] | vec.digits[i];
    }

    positive = vec.positive & positive;

    delete0();

    return *this;
}

BigInt operator^(const BigInt &vec1, const BigInt &vec2) {
    BigInt tmp = vec1;
    tmp ^= vec2;
    return tmp;
}

BigInt operator&(const BigInt &vec1, const BigInt &vec2) {
    BigInt tmp = vec1;
    tmp &= vec2;
    return tmp;
}

BigInt operator|(const BigInt &vec1, const BigInt &vec2) {
    BigInt tmp = vec1;
    tmp |= vec2;
    return tmp;
}


