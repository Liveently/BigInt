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

    digits.clear();

    for(int digit : val.digits){
        digits.push_back(digit);
    }

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















