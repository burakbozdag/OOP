/*
	Name: Burak Bozdað
	ID: 150170110
*/

#ifndef Vector_H
#define Vector_H

class Vector {
private:
    int size;
    vector<int> vect;

public:
    //Constructor
    Vector()
    {
        size = 0;
    };

    //Copy constructor
    Vector(const Vector& v)
    {
        size = v.size;
        vect = v.vect;
    };

    //Destructor
    ~Vector()
    {
        vect.clear();
    };

    //Getters
    int getSize()
    {
        return this->size;
    };
    vector<int> getVect()
    {
        return this->vect;
    };

    //Setter
    void setV(const int s, const int values[])
    {
        size = s;
        for (int i = 0; i < size; i++)
            vect.push_back(values[i]);
    };

    Vector operator+(Vector const& v)
    {
        Vector Vout;
        Vout.size = MIN(size, v.size);
        for (int i = 0; i < Vout.size; i++)
            Vout.vect.push_back(vect[i] + v.vect[i]);
        return Vout;
    };
    Vector operator*(int const& num)
    {
        Vector Vout = *this;
        for (int i = 0; i < size; i++)
            Vout.vect[i] *= num;
        return Vout;
    };
    int operator*(Vector const& v)
    {
        int product = 0;
        for (int i = 0; i < size; i++)
            product += vect[i] * v.vect[i];
        return product;
    }
    friend ostream& operator<<(ostream& out, const Vector& v)
    {
        out << "(";
        for (int i = 0; i < v.size; i++) {
            if (i == v.size - 1)
                out << v.vect[i];
            else
                out << v.vect[i] << ", ";
        }
        out << ")";
        return out;
    }
};

#endif
