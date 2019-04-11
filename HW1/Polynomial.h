/*
	Name: Burak Bozdað
	ID: 150170110
*/

#ifndef Polynomial_H
#define Polynomial_H

class Polynomial {
private:
    int degree;
    vector<int> poly;

public:
    //Constructor
    Polynomial()
    {
        degree = 0;
    };

    //Copy constructor
    Polynomial(const Polynomial& p)
    {
        degree = p.degree;
        poly = p.poly;
    };

    //Destructor
    ~Polynomial()
    {
        poly.clear();
    };

    //Getters
    int getDegree()
    {
        return this->degree;
    };
    vector<int> getPoly()
    {
        return this->poly;
    };

    //Setter
    void setP(const int d, const int values[])
    {
        degree = d;
        for (int i = 0; i < degree + 1; i++)
            poly.insert(poly.begin(), values[i]);
    };

    Polynomial operator+(Polynomial const& p)
    {
        Polynomial Pout;
        Pout.degree = MAX(degree, p.degree);
        for (int i = 0; i <= Pout.degree; i++) {
            if (i <= degree && i <= p.degree)
                Pout.poly.push_back(poly[i] + p.poly[i]);
            else if (i > degree)
                Pout.poly.push_back(p.poly[i]);
            else if (i > p.degree)
                Pout.poly.push_back(poly[i]);
        }
        return Pout;
    };
    Polynomial operator*(Polynomial const& p)
    {
        Polynomial Pout;
        Pout.degree = degree + p.degree;
        for (int i = 0; i < degree + 1; i++)
            Pout.poly.push_back(0);
        for (int i = 0; i < degree + 1; i++) {
            for (int j = 0; j < p.degree + 1; j++)
                Pout.poly[i + j] += poly[i] * p.poly[j];
        }
        return Pout;
    };
    friend ostream& operator<<(ostream& out, const Polynomial& p)
    {
        for (int i = p.degree; i >= 0; i--) {
            if (p.poly[i] == 0)
                continue;
            else if (i == 0) {
                if (p.poly[i] < 0)
                    out << "- " << (p.poly[i]) * -1;
                else {
                    if (i == p.degree)
                        out << p.poly[i];
                    else
                        out << "+ " << p.poly[i];
                }
            } else if (i == 1) {
                if (p.poly[i] < 0) {
                    out << "- " << (p.poly[i]) * -1 << "x ";
                } else {
                    if (i == p.degree) {
                        if (p.poly[i] == 1)
                            out << "x ";
                        else
                            out << p.poly[i] << "x ";
                    } else {
                        if (p.poly[i] == 1)
                            out << "+x ";
                        else
                            out << "+ " << p.poly[i] << "x ";
                    }
                }
            } else if (p.poly[i] == 1) {
                if (i == p.degree)
                    out << "x^" << i << " ";
                else
                    out << "+ x^" << i << " ";
            } else if (p.poly[i] == -1)
                out << "- x^" << i << " ";
            else if (p.poly[i] < -1)
                out << "- " << (p.poly[i]) * -1 << "x^" << i << " ";
            else {
                if (i == p.degree)
                    out << p.poly[i] << "x^" << i << " ";
                else
                    out << "+ " << p.poly[i] << "x^" << i << " ";
            }
        }
        return out;
    };
};

#endif
