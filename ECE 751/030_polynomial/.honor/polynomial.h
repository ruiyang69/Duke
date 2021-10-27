#include <string>

class Polynomial
{
private:

public:
    Polynomial();
    Polynomial operator+(const Polynomial & rhs) const ;
    Polynomial operator-() const;
    Polynomial operator-(const Polynomial & rhs) const;
    Polynomial operator*(const Num & n) const;
    Polynomial operator*(const Polynomial & rhs) const;
    bool operator!=(const Polynomial & rhs) const;
    bool operator==(const Polynomial & rhs) const;
    void addTerm(const Num & c, int p);
    Polynomial & operator+=(const Polynomial & rhs);
    Polynomial & operator-=(const Polynomial & rhs);
    Polynomial & operator*=(const Polynomial & rhs);
    Polynomial & operator*=(const Num & rhs);

};