#include <string>
#include <vector>

class BigInt {

    static const int base = 1000000000;
    std::vector<int> digits;
    bool positive;



    friend void module_addition (const std::vector<int>& a, const std::vector<int>& b, std::vector<int> res);
    friend void module_subtraction (const std::vector<int>& a, const std::vector<int>& b, std::vector<int> res);
    friend bool module_compare(const std::vector<int>& a, const std::vector<int>& b);


public:
    BigInt();
    BigInt(int val);
    explicit BigInt(std::string val); // бросать исключение std::invalid_argument при ошибке
    BigInt(const BigInt & val);
    ~BigInt();

    BigInt &operator=(const BigInt & val);  //возможно присваивание самому себе!

    BigInt operator~() const;
    BigInt &operator++();
    const BigInt operator++(int);
    BigInt &operator--();
    const BigInt operator--(int);

    BigInt &operator+=(const BigInt &val);
    BigInt &operator*=(const BigInt &val);
    BigInt &operator-=(const BigInt &val);
    BigInt &operator/=(const BigInt &val);
    BigInt &operator^=(const BigInt &val);
    BigInt &operator%=(const BigInt &val);
    BigInt &operator&=(const BigInt &val);
    BigInt &operator|=(const BigInt &val);

    BigInt operator+() const;  // unary +
    BigInt operator-() const;  // unary -

    bool operator==(const BigInt &val) const;
    bool operator!=(const BigInt &val) const;
    bool operator<(const BigInt &val) const;
    bool operator>(const BigInt &val) const;
    bool operator<=(const BigInt &val) const;
    bool operator>=(const BigInt &val) const;

    operator int() const;
    operator std::string() const;

    size_t size() const;

    friend BigInt operator*(const BigInt &, const BigInt &);

    friend std::ostream &operator<<(std::ostream &o, const BigInt &i);


    friend BigInt operator+(const BigInt &, const BigInt &);
    friend BigInt operator-(const BigInt &, const BigInt &);

    friend BigInt operator/(const BigInt &, const BigInt &);

    friend BigInt operator^(const BigInt &, const BigInt &);

    friend BigInt operator&(const BigInt &, const BigInt &);
    friend BigInt operator|(const BigInt &, const BigInt &);

    friend BigInt operator%(const BigInt &, const BigInt &);

};

