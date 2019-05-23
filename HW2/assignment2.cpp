#include <fstream>
#include <iostream>

#define MAX_OPERATOR_SIZE 37 //Max possible operators in a 9-sized arithmetic operator

using namespace std;

void borderp(char sign, int size, int x, int y) //Border error while placing
{
    cerr << "BORDER ERROR: Operator " << sign << " with size " << size << " can not be placed on (" << x << "," << y << ")." << endl;
}

void conflictp(char sign, int size, int x, int y) //Conflict error while placing
{
    cerr << "CONFLICT ERROR: Operator " << sign << " with size " << size << " can not be placed on (" << x << "," << y << ")." << endl;
}

void successp(char sign, int size, int x, int y) //Success after placing
{
    cout << "SUCCESS: Operator " << sign << " with size " << size << " is placed on (" << x << "," << y << ")." << endl;
}

void borderm(char sign, int x0, int y0, int x1, int y1) //Border error while moving
{
    cerr << "BORDER ERROR: " << sign << " can not be moved from (" << x0 << "," << y0 << ") to ("
         << x1 << "," << y1 << ")." << endl;
}

void conflictm(char sign, int x0, int y0, int x1, int y1) //Conflict error while moving
{
    cerr << "CONFLICT ERROR: " << sign << " can not be moved from (" << x0 << "," << y0 << ") to ("
         << x1 << "," << y1 << ")." << endl;
}

void successm(char sign, int x0, int y0, int x1, int y1) //Success after moving
{
    cout << "SUCCESS: " << sign << " moved from (" << x0 << "," << y0 << ") to (" << x1 << "," << y1 << ")." << endl;
}

class Operator {
    int center_x;
    int center_y;
    int op_size;

public:
    Operator(int x, int y, int size)
    {
        this->center_x = x;
        this->center_y = y;
        this->op_size = size;
    };

    void reset(int new_x, int new_y, int new_size)
    {
        this->center_x = new_x;
        this->center_y = new_y;
        this->op_size = new_size;
    };

    void set_x(int new_x)
    {
        this->center_x = new_x;
    };
    int get_x()
    {
        return this->center_x;
    };

    void set_y(int new_y)
    {
        this->center_y = new_y;
    };
    int get_y()
    {
        return this->center_y;
    };
    
    int get_size()
    {
        return this->op_size;
    };
};

class ArithmeticOperator : public Operator {
    char sign;

public:
    ArithmeticOperator(int x, int y, int size, char sign)
        : Operator(x, y, size)
    {
        if (sign == '+' || sign == '-' || sign == 'x' || sign == '/')
            this->sign = sign;
        else
            cerr << "SIGN parameter is invalid!" << endl;
    };

    char get_sign()
    {
        return this->sign;
    };

    //Prints out operator's center location, size and sign character
    void print_operator()
    {
        cout << "ARITHMETIC_OPERATOR[" << this->sign << "], CENTER_LOCATION[" << this->get_x() << "," << this->get_y() << "], SIZE[" << this->get_size() << "]" << endl;
    };
};

class OperatorGrid {
    int grid_rows;
    int grid_cols;
    char** grid;

    int num_operators;
    ArithmeticOperator* operators[MAX_OPERATOR_SIZE];

public:
    OperatorGrid(int rows, int cols)
    {
        this->grid_rows = rows;
        this->grid_cols = cols;
        this->num_operators = 0;
        grid = new char*[rows];
        for (int i = 0; i < rows; i++)
            grid[i] = new char[cols];
        for (int i = 0; i < grid_rows; i++) {
            for (int j = 0; j < grid_cols; j++)
                grid[i][j] = '.';
        }
    };
    ~OperatorGrid()
    {
        for (int i = 0; i < this->grid_rows; i++)
            delete[] grid[i];
        for (int i = 0; i < num_operators - 1; i++)
            delete[] operators[i];
        delete[] grid;
        cout << "DESTRUCTOR: GIVE BACK[" << this->grid_rows << "," << this->grid_cols << "] chars." << endl;
        cout << "DESTRUCTOR: GIVE BACK[" << this->num_operators << "] Operators." << endl;
    };

    bool place_operator(ArithmeticOperator* op)
    {
        bool border = false;
        bool conflict = false;
        switch (op->get_sign()) { //Operations for different signs
        case '-':
            if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                border = true;
            for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++) {
                if (i < 1 || i > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                    continue;
                if (grid[op->get_x() - 1][i - 1] == '+' || grid[op->get_x() - 1][i - 1] == '-' || grid[op->get_x() - 1][i - 1] == 'x' || grid[op->get_x() - 1][i - 1] == '/') {
                    conflict = true;
                    break;
                }
            }
            if (border || conflict) {
                if (border)
                    borderp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
                if (conflict)
                    conflictp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
                return false;
            }
            operators[num_operators] = new ArithmeticOperator(op->get_x(), op->get_y(), op->get_size(), op->get_sign());
            for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++) {
                grid[op->get_x() - 1][i - 1] = '-';
            }
            break;
        case '+':
            if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                border = true;
            for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++) {
                if (i < 1 || i > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                    continue;
                if (grid[op->get_x() - 1][i - 1] == '+' || grid[op->get_x() - 1][i - 1] == '-' || grid[op->get_x() - 1][i - 1] == 'x' || grid[op->get_x() - 1][i - 1] == '/') {
                    conflict = true;
                    break;
                }
            }
            for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++) {
                if (i < 1 || i > grid_rows || op->get_y() < 1 || op->get_y() > grid_cols)
                    continue;
                if (grid[i - 1][op->get_y() - 1] == '+' || grid[i - 1][op->get_y() - 1] == '-' || grid[i - 1][op->get_y() - 1] == 'x' || grid[i - 1][op->get_y() - 1] == '/') {
                    conflict = true;
                    break;
                }
            }
            if (border || conflict) {
                if (border)
                    borderp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
                if (conflict)
                    conflictp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
                return false;
            }
            operators[num_operators] = new ArithmeticOperator(op->get_x(), op->get_y(), op->get_size(), op->get_sign());
            for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++) {
                grid[op->get_x() - 1][i - 1] = '+';
            }
            for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++) {
                grid[i - 1][op->get_y() - 1] = '+';
            }
            break;
        case '/':
            if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                border = true;
            for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++) {
                if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                    continue;
                if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                    conflict = true;
                    break;
                }
            }
            if (border || conflict) {
                if (border)
                    borderp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
                if (conflict)
                    conflictp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
                return false;
            }
            operators[num_operators] = new ArithmeticOperator(op->get_x(), op->get_y(), op->get_size(), op->get_sign());
            for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                grid[i - 1][j - 1] = '/';
            break;
        case 'x':
            if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                border = true;
            for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++) {
                if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                    continue;
                if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                    conflict = true;
                    break;
                }
            }
            for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++) {
                if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                    continue;
                if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                    conflict = true;
                    break;
                }
            }
            if (border || conflict) {
                if (border)
                    borderp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
                if (conflict)
                    conflictp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
                return false;
            }
            operators[num_operators] = new ArithmeticOperator(op->get_x(), op->get_y(), op->get_size(), op->get_sign());
            for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++) {
                grid[i - 1][j - 1] = 'x';
            }
            for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++) {
                grid[i - 1][j - 1] = 'x';
            }
            break;
        }
        ++num_operators;
		successp(op->get_sign(), op->get_size(), op->get_x(), op->get_y());
        return true;
    };
    
	bool move_operator(int x, int y, char direction, int move_by)
    {
        bool found = false;
        int x0, y0;
        ArithmeticOperator* op;
        for (int a = 0; !found && a < num_operators; a++) { //Finding center of the operator
            op = operators[a];
            switch (op->get_sign()) {
            	case '-':
            		for (int i = op->get_y() - op->get_size(); !found && i <= op->get_y() + op->get_size(); i++) {
            			if (x == op->get_x() && y == i) {
            				found = true;
							x0 = op->get_x();
            				y0 = op->get_y();
            				break;
            			}
            		}
            		break;
            	case '+':
            		for (int i = op->get_y() - op->get_size(); !found && i <= op->get_y() + op->get_size(); i++) {
            			if (x == op->get_x() && y == i) {
            				found = true;
							x0 = op->get_x();
            				y0 = op->get_y();
            				break;
            			}
            		}
            		for (int i = op->get_x() - op->get_size(); !found && i <= op->get_x() + op->get_size(); i++) {
            			if (x == i && y == op->get_y()) {
            				found = true;
							x0 = op->get_x();
            				y0 = op->get_y();
            				break;
            			}
            		}
            		break;
            	case '/':
            		for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); !found && i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++) {
            			if (x == i && y == j) {
            				found = true;
            				x0 = op->get_x();
            				y0 = op->get_y();
            				break;
            			}
            		}
            		break;
            	case 'x':
            		for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); !found && i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++) {
            			if (x == i && y == j) {
            				found = true;
            				x0 = op->get_x();
            				y0 = op->get_y();
            				break;
            			}
            		}
            		for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); !found && i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++) {
            			if (x == i && y == j) {
            				found = true;
            				x0 = op->get_x();
            				y0 = op->get_y();
            				break;
            			}
            		}
            		break;
            }
        }
        if (!found) {
        	cerr << "Operator not found." << endl;
        	return false;
        }
        bool border = false;
        bool conflict = false;
        switch (op->get_sign()) { //Determining sign
        case '-':
            switch (direction) { //Determining direction
            case 'R':
                if (op->get_y() - op->get_size() + move_by < 1 || op->get_y() + op->get_size() + move_by > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                    border = true;
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1][i - 1] = '.';
                for (int i = op->get_y() - op->get_size() + move_by; i <= op->get_y() + op->get_size() + move_by; i++) {
                    if (i < 1 || i > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                        continue;
                    if (grid[op->get_x() - 1][i - 1] == '+' || grid[op->get_x() - 1][i - 1] == '-' || grid[op->get_x() - 1][i - 1] == 'x' || grid[op->get_x() - 1][i - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() + move_by);
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() + move_by);
                    for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                        grid[op->get_x() - 1][i - 1] = '-';
                    return false;
                }
                for (int i = op->get_y() - op->get_size() + move_by; i <= op->get_y() + op->get_size() + move_by; i++)
                    grid[op->get_x() - 1][i - 1] = '-';
                op->set_y(y0 + move_by);
                break;
            case 'L':
                if (op->get_y() - op->get_size() - move_by < 1 || op->get_y() + op->get_size() - move_by > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                    border = true;
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1][i - 1] = '.';
                for (int i = op->get_y() - op->get_size() - move_by; i <= op->get_y() + op->get_size() - move_by; i++) {
                    if (i < 1 || i > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                        continue;
                    if (grid[op->get_x() - 1][i - 1] == '+' || grid[op->get_x() - 1][i - 1] == '-' || grid[op->get_x() - 1][i - 1] == 'x' || grid[op->get_x() - 1][i - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() - move_by);
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() - move_by);
                    for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                        grid[op->get_x() - 1][i - 1] = '-';
                    return false;
                }
                for (int i = op->get_y() - op->get_size() - move_by; i <= op->get_y() + op->get_size() - move_by; i++)
                    grid[op->get_x() - 1][i - 1] = '-';
                op->set_y(y0 - move_by);
                break;
            case 'U':
                if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - move_by < 1 || op->get_x() - move_by > grid_rows)
                    border = true;
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1][i - 1] = '.';
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++) {
                    if (i < 1 || i > grid_cols || op->get_x() - move_by < 1 || op->get_x() - move_by > grid_rows)
                        continue;
                    if (grid[op->get_x() - 1 - move_by][i - 1] == '+' || grid[op->get_x() - 1 - move_by][i - 1] == '-' || grid[op->get_x() - 1 - move_by][i - 1] == 'x' || grid[op->get_x() - 1 - move_by][i - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() - move_by, op->get_y());
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() - move_by, op->get_y());
                    for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                        grid[op->get_x() - 1][i - 1] = '-';
                    return false;
                }
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1 - move_by][i - 1] = '-';
                op->set_x(x0 - move_by);
                break;
            case 'D':
                if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() + move_by < 1 || op->get_x() + move_by > grid_rows)
                    border = true;
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1][i - 1] = '.';
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++) {
                    if (i < 1 || i > grid_cols || op->get_x() + move_by < 1 || op->get_x() + move_by > grid_rows)
                        continue;
                    if (grid[op->get_x() - 1 + move_by][i - 1] == '+' || grid[op->get_x() - 1 + move_by][i - 1] == '-' || grid[op->get_x() - 1 + move_by][i - 1] == 'x' || grid[op->get_x() - 1 + move_by][i - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                if (border || conflict) {
					if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() + move_by, op->get_y());
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() + move_by, op->get_y());
                    for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                        grid[op->get_x() - 1][i - 1] = '-';
                    return false;
                }
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1 + move_by][i - 1] = '-';
                op->set_x(x0 + move_by);
                break;
            }
            break;
        case '+':
        	switch (direction) {
            case 'R':
                if (op->get_y() - op->get_size() + move_by < 1 || op->get_y() + op->get_size() + move_by > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                    border = true;
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1][i - 1] = '.';
                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
                	grid[i - 1][op->get_y() - 1] = '.';
                for (int i = op->get_y() - op->get_size() + move_by; i <= op->get_y() + op->get_size() + move_by; i++) {
                    if (i < 1 || i > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                        continue;
                    if (grid[op->get_x() - 1][i - 1] == '+' || grid[op->get_x() - 1][i - 1] == '-' || grid[op->get_x() - 1][i - 1] == 'x' || grid[op->get_x() - 1][i - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++) {
                    if (i < 1 || i > grid_cols || op->get_y() + move_by < 1 || op->get_y() + move_by > grid_cols)
                        continue;
                    if (grid[i - 1][op->get_y() - 1 + move_by] == '+' || grid[i - 1][op->get_y() - 1 + move_by] == '-' || grid[i - 1][op->get_y() - 1 + move_by] == 'x' || grid[i - 1][op->get_y() - 1 + move_by] == '/') {
                        conflict = true;
                        break;
                    }
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() + move_by);
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() + move_by);
                    for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    	grid[op->get_x() - 1][i - 1] = '+';
	                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
	                	grid[i - 1][op->get_y() - 1] = '+';
                    return false;
                }
                for (int i = op->get_y() - op->get_size() + move_by; i <= op->get_y() + op->get_size() + move_by; i++)
                    grid[op->get_x() - 1][i - 1] = '+';
                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
                	grid[i - 1][op->get_y() - 1 + move_by] = '+';
                op->set_y(y0 + move_by);
                break;
            case 'L':
                if (op->get_y() - op->get_size() - move_by < 1 || op->get_y() + op->get_size() - move_by > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                    border = true;
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1][i - 1] = '.';
                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
                	grid[i - 1][op->get_y() - 1] = '.';
                for (int i = op->get_y() - op->get_size() - move_by; i <= op->get_y() + op->get_size() - move_by; i++) {
                    if (i < 1 || i > grid_cols || op->get_x() < 1 || op->get_x() > grid_rows)
                        continue;
                    if (grid[op->get_x() - 1][i - 1] == '+' || grid[op->get_x() - 1][i - 1] == '-' || grid[op->get_x() - 1][i - 1] == 'x' || grid[op->get_x() - 1][i - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++) {
                    if (i < 1 || i > grid_cols || op->get_y() - move_by < 1 || op->get_y() - move_by > grid_cols)
                        continue;
                    if (grid[i - 1][op->get_y() - 1 - move_by] == '+' || grid[i - 1][op->get_y() - 1 - move_by] == '-' || grid[i - 1][op->get_y() - 1 - move_by] == 'x' || grid[i - 1][op->get_y() - 1 - move_by] == '/') {
                        conflict = true;
                        break;
                    }
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() - move_by);
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() - move_by);
                    for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    	grid[op->get_x() - 1][i - 1] = '+';
	                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
	                	grid[i - 1][op->get_y() - 1] = '+';
                    return false;
                }
                for (int i = op->get_y() - op->get_size() - move_by; i <= op->get_y() + op->get_size() - move_by; i++)
                    grid[op->get_x() - 1][i - 1] = '+';
                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
                	grid[i - 1][op->get_y() - 1] = '+';
                op->set_y(y0 - move_by);
                break;
            case 'U':
                if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() - move_by < 1 || op->get_x() + op->get_size() - move_by > grid_rows)
                    border = true;
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1][i - 1] = '.';
                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
                	grid[i - 1][op->get_y() - 1] = '.';
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++) {
                    if (i < 1 || i > grid_cols || op->get_x() - move_by < 1 || op->get_x() - move_by > grid_rows)
                        continue;
                    if (grid[op->get_x() - 1 - move_by][i - 1] == '+' || grid[op->get_x() - 1 - move_by][i - 1] == '-' || grid[op->get_x() - 1 - move_by][i - 1] == 'x' || grid[op->get_x() - 1 - move_by][i - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                for (int i = op->get_x() - op->get_size() - move_by; i <= op->get_x() + op->get_size() - move_by; i++) {
                    if (i < 1 || i > grid_cols || op->get_y() < 1 || op->get_y() > grid_rows)
                        continue;
                    if (grid[i - 1][op->get_y() - 1] == '+' || grid[i - 1][op->get_y() - 1] == '-' || grid[i - 1][op->get_y() - 1] == 'x' || grid[i - 1][op->get_y() - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() - move_by, op->get_y());
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() - move_by, op->get_y());
                    for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    	grid[op->get_x() - 1][i - 1] = '+';
	                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
	                	grid[i - 1][op->get_y() - 1] = '+';
                    return false;
                }
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1 - move_by][i - 1] = '-';
                for (int i = op->get_x() - op->get_size() - move_by; i <= op->get_x() + op->get_size() - move_by; i++)
                    grid[i - 1][op->get_y() - 1] = '+';
                op->set_x(x0 - move_by);
                break;
            case 'D':
                if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() + move_by < 1 || op->get_x() + op->get_size() + move_by > grid_rows)
                    border = true;
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1][i - 1] = '.';
                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
                	grid[i - 1][op->get_y() - 1] = '.';
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++) {
                    if (i < 1 || i > grid_cols || op->get_x() + move_by < 1 || op->get_x() + move_by > grid_rows)
                        continue;
                    if (grid[op->get_x() - 1 + move_by][i - 1] == '+' || grid[op->get_x() - 1 + move_by][i - 1] == '-' || grid[op->get_x() - 1 + move_by][i - 1] == 'x' || grid[op->get_x() - 1 + move_by][i - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                for (int i = op->get_x() - op->get_size() + move_by; i <= op->get_x() + op->get_size() + move_by; i++) {
                    if (i < 1 || i > grid_cols  || op->get_y() < 1 || op->get_y() > grid_rows)
                        continue;
                    if (grid[i - 1][op->get_y() - 1] == '+' || grid[i - 1][op->get_y() - 1] == '-' || grid[i - 1][op->get_y() - 1] == 'x' || grid[i - 1][op->get_y() - 1] == '/') {
                        conflict = true;
                        break;
                    }
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() + move_by, op->get_y());
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() + move_by, op->get_y());
                    for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    	grid[op->get_x() - 1][i - 1] = '+';
	                for (int i = op->get_x() - op->get_size(); i <= op->get_x() + op->get_size(); i++)
	                	grid[i - 1][op->get_y() - 1] = '+';
                    return false;
                }
                for (int i = op->get_y() - op->get_size(); i <= op->get_y() + op->get_size(); i++)
                    grid[op->get_x() - 1 + move_by][i - 1] = '-';
                for (int i = op->get_x() - op->get_size() + move_by; i <= op->get_x() + op->get_size() + move_by; i++)
                    grid[i - 1][op->get_y() - 1] = '+';
                op->set_x(x0 + move_by);
                break;
            }
            break;
        case '/':
        	switch (direction) {
        	case 'R':
        		if (op->get_y() - op->get_size() + move_by < 1 || op->get_y() + op->get_size() + move_by > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                    border = true;
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size() + move_by; i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size() + move_by; i--, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() + move_by);
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() + move_by);
                    for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                		grid[i - 1][j - 1] = '/';
                    return false;
                }
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size() + move_by; i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size() + move_by; i--, j++)
                	grid[i - 1][j - 1] = '/';
                op->set_y(y0 + move_by);
        		break;
        	case 'L':
        		if (op->get_y() - op->get_size() - move_by < 1 || op->get_y() + op->get_size() - move_by > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                    border = true;
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size() - move_by; i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size() - move_by; i--, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() - move_by);
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() - move_by);
                    for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                		grid[i - 1][j - 1] = '/';
                    return false;
                }
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size() - move_by; i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size() - move_by; i--, j++)
                	grid[i - 1][j - 1] = '/';
                op->set_y(y0 - move_by);
        		break;
        	case 'U':
        		if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() - move_by < 1 || op->get_x() + op->get_size() - move_by > grid_rows)
                    border = true;
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() + op->get_size() - move_by, j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() - move_by && j <= op->get_y() + op->get_size(); i--, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() - move_by, op->get_y());
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() - move_by, op->get_y());
                    for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                		grid[i - 1][j - 1] = '/';
                    return false;
                }
                for (int i = op->get_x() + op->get_size() - move_by, j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() - move_by && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '/';
                op->set_x(x0 - move_by);
        		break;
        	case 'D':
        		if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() + move_by < 1 || op->get_x() + op->get_size() + move_by > grid_rows)
                    border = true;
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() + op->get_size() + move_by, j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() + move_by && j <= op->get_y() + op->get_size(); i--, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() + move_by, op->get_y());
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() + move_by, op->get_y());
                    for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                		grid[i - 1][j - 1] = '/';
                    return false;
                }
                for (int i = op->get_x() + op->get_size() + move_by, j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() + move_by && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '/';
                op->set_x(x0 + move_by);
        		break;
        	}
        	break;
        case 'x':
        	switch (direction) {
        	case 'R':
        		if (op->get_y() - op->get_size() + move_by < 1 || op->get_y() + op->get_size() + move_by > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                    border = true;
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size() + move_by; i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size() + move_by; i--, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size() + move_by; i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size() + move_by; i++, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() + move_by);
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() + move_by);
                    for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                		grid[i - 1][j - 1] = 'x';
                	for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++)
                		grid[i - 1][j - 1] = 'x';
                    return false;
                }
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size() + move_by; i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size() + move_by; i--, j++)
                	grid[i - 1][j - 1] = 'x';
                for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size() + move_by; i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size() + move_by; i++, j++)
                	grid[i - 1][j - 1] = 'x';
                op->set_y(y0 + move_by);
        		break;
        	case 'L':
        		if (op->get_y() - op->get_size() - move_by < 1 || op->get_y() + op->get_size() - move_by > grid_cols || op->get_x() - op->get_size() < 1 || op->get_x() + op->get_size() > grid_rows)
                    border = true;
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size() - move_by; i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size() - move_by; i--, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size() - move_by; i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size() - move_by; i++, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() - move_by);
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x(), op->get_y() - move_by);
                    for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                		grid[i - 1][j - 1] = 'x';
                	for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++)
                		grid[i - 1][j - 1] = 'x';
                    return false;
                }
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size() - move_by; i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size() - move_by; i--, j++)
                	grid[i - 1][j - 1] = 'x';
                for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size() - move_by; i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size() - move_by; i++, j++)
                	grid[i - 1][j - 1] = 'x';
                op->set_y(y0 - move_by);
        		break;
        	case 'U':
        		if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() - move_by < 1 || op->get_x() + op->get_size() - move_by > grid_rows)
                    border = true;
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() + op->get_size() - move_by, j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() - move_by && j <= op->get_y() + op->get_size(); i--, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                for (int i = op->get_x() - op->get_size() - move_by, j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() - move_by && j <= op->get_y() + op->get_size(); i++, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() - move_by, op->get_y());
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() - move_by, op->get_y());
                    for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                		grid[i - 1][j - 1] = 'x';
                	for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++)
                		grid[i - 1][j - 1] = 'x';
                    return false;
                }
                for (int i = op->get_x() + op->get_size() - move_by, j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() - move_by && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = 'x';
                for (int i = op->get_x() - op->get_size() - move_by, j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() - move_by && j <= op->get_y() + op->get_size(); i++, j++)
                	grid[i - 1][j - 1] = 'x';
                op->set_x(x0 - move_by);
        		break;
        	case 'D':
        		if (op->get_y() - op->get_size() < 1 || op->get_y() + op->get_size() > grid_cols || op->get_x() - op->get_size() + move_by < 1 || op->get_x() + op->get_size() + move_by > grid_rows)
                    border = true;
                for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++)
                	grid[i - 1][j - 1] = '.';
                for (int i = op->get_x() + op->get_size() + move_by, j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() + move_by && j <= op->get_y() + op->get_size(); i--, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                for (int i = op->get_x() - op->get_size() + move_by, j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() + move_by && j <= op->get_y() + op->get_size(); i++, j++) {
                	if (i < 1 || i > grid_rows || j < 1 || j > grid_cols)
                		continue;
                	if (grid[i - 1][j - 1] == '+' || grid[i - 1][j - 1] == '-' || grid[i - 1][j - 1] == 'x' || grid[i - 1][j - 1] == '/') {
                		conflict = true;
                		break;
                	}
                }
                if (border || conflict) {
                    if (border)
                        borderm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() + move_by, op->get_y());
                    if (conflict)
                        conflictm(op->get_sign(), op->get_x(), op->get_y(), op->get_x() + move_by, op->get_y());
                    for (int i = op->get_x() + op->get_size(), j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() && j <= op->get_y() + op->get_size(); i--, j++)
                		grid[i - 1][j - 1] = 'x';
                	for (int i = op->get_x() - op->get_size(), j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() && j <= op->get_y() + op->get_size(); i++, j++)
                		grid[i - 1][j - 1] = 'x';
                    return false;
                }
                for (int i = op->get_x() + op->get_size() + move_by, j = op->get_y() - op->get_size(); i >= op->get_x() - op->get_size() + move_by && j <= op->get_y() + op->get_size(); i--, j++)
                	grid[i - 1][j - 1] = 'x';
                for (int i = op->get_x() - op->get_size() + move_by, j = op->get_y() - op->get_size(); i <= op->get_x() + op->get_size() + move_by && j <= op->get_y() + op->get_size(); i++, j++)
                	grid[i - 1][j - 1] = 'x';
                op->set_x(x0 + move_by);
        		break;
        	}
            break;
        }
        successm(op->get_sign(), x0, y0, op->get_x(), op->get_y()); //Moving successful
        return true;
    };
    
	void print_operators() //Prints all of current operators
    {
        for (int i = 0; i < num_operators; i++)
            operators[i][0].print_operator();
    };

    void print_grid() //For testing purposes
    {
        for (int i = 0; i < grid_rows; i++) {
            for (int j = 0; j < grid_cols; j++) {
                cout << grid[i][j];
            }
            cout << endl;
        }
    };
};

int main()
{
	/*
	OperatorGrid a(10,7);
    ArithmeticOperator *x = new ArithmeticOperator(2,2,1,'x');
    ArithmeticOperator *y = new ArithmeticOperator(2,4,1,'+');
    ArithmeticOperator *z = new ArithmeticOperator(2,6,1,'x');
    ArithmeticOperator *t = new ArithmeticOperator(2,1,9,'/');
    ArithmeticOperator *w = new ArithmeticOperator(9,4,3,'-');
    ArithmeticOperator *xx = new ArithmeticOperator(5,4,3,'/');
    ArithmeticOperator *yy = new ArithmeticOperator(5,4,3,'-');
    ArithmeticOperator *zz = new ArithmeticOperator(5,2,1,'-');
    ArithmeticOperator *tt = new ArithmeticOperator(7,6,1,'/');
    ArithmeticOperator *ww = new ArithmeticOperator(5,6,1,'+');
    a.place_operator(x);
    //a.print_grid();
    a.place_operator(y);
    //a.print_grid();
    a.place_operator(z);
    //a.print_grid();
	a.place_operator(t);
	//a.print_grid();
    a.place_operator(w);
	//a.print_grid();
    a.place_operator(xx);
	//a.print_grid();
    a.place_operator(yy);
	//a.print_grid();
    a.place_operator(zz);
	//a.print_grid();
    a.place_operator(tt);
	//a.print_grid();
    a.place_operator(ww);
	//a.print_grid();
    a.move_operator(1,1,'D',2);
	//a.print_grid();
    a.move_operator(5,1,'D',5);
	//a.print_grid();
    a.move_operator(1,1,'D',2);
	//a.print_grid();
    a.move_operator(2,4,'L',1);
	//a.print_grid();
    a.move_operator(2,4,'L',2);
	//a.print_grid();
    a.move_operator(8,5,'L',3);
	//a.print_grid();
    a.move_operator(1,7,'D',5);
	//a.print_grid();
    a.move_operator(10,1,'R',7);
	//a.print_grid();
    a.move_operator(5,4,'U',3);
	//a.print_grid();
    a.print_operators();
    */
    return 0;
}
