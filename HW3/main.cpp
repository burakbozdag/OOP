/* @Author

 * Student Name: Burak Bozdað
 * Student ID : 150170110
 * Date: 24.04.2019 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Stock {
	string name;
	int type;
	int itemCount;
	double price;
public:
	Stock(string n, int t, int i, double p) {
		this->name = n;
		this->type = t;
		this->itemCount = i;
		this->price = p;
	};
	string getName() {
		return this->name;
	};
	int getType() {
		return this->type;
	};
	int getItemCount() {
		return this->itemCount;
	};
	double getPrice() {
		return this->price;
	};
	void subItemCount(int newItemCount) {
		int temp = this->itemCount;
		this->itemCount = temp - newItemCount;
	};
};

class Menu {
	string name;
	string ingredients;
public:
	Menu(string n, string i) {
		this->name = n;
		this->ingredients = i;
	};
	string getName() {
		return this->name;
	};
	string getIngredients(){
		return this->ingredients;
	};
};

int main()
{
	ifstream readStock("stock.txt");
	ifstream readMenu("menu.txt");
	ifstream readOrder("order.txt");
	
	string line;
	vector<Stock> stock;
	getline(readStock, line);
	for (int i = 0; i < 27; i++) {
		getline(readStock, line);
		istringstream readLine(line);
		string name;
		int type, itemCount;
		double price;
		getline(readLine, name, '\t');
		readLine >> type >> itemCount >> price;
		if (!isprint(name.front()))
			name.erase(name.begin());
		if (!isprint(name.back()))
			name.pop_back();
		Stock temp(name, type, itemCount, price);
		stock.push_back(temp);
		line.clear(); 
	}
	//for (int i = 0; i < 27; i++) cout << stock[i].getName() << "\t" << stock[i].getType() << "\t" << stock[i].getItemCount() << "\t" << stock[i].getPrice() << endl;
	//cout << endl;
	
	vector<Menu> menu;
	getline(readMenu, line);
	for (int i = 0; i < 14; i++) {
		getline(readMenu, line);
		istringstream readLine(line);
		string name;
		string ingredients;
		getline(readLine, name, '\t');
		getline(readLine, ingredients);
		if (!isprint(name.front()))
			name.erase(name.begin());
		if (!isprint(name.back()))
			name.pop_back();
		if (!isprint(ingredients.front()))
			ingredients.erase(ingredients.begin());
		if (!isprint(ingredients.back()))
			ingredients.pop_back();
		Menu temp(name, ingredients);
		menu.push_back(temp);
		line.clear();
	}
	//for (int i = 0;  i < 14; i++) cout << menu[i].getName() << "\t" << menu[i].getIngredients() << endl;
	//cout << endl;
	
	while (getline(readOrder, line)) {
		line.pop_back();
		cout << line << " ordered:" << endl;
		int count;
		double checkout = 0;
		readOrder >> count;
		getline(readOrder, line);
		line.clear();
		for (int a = 0; a < count; a++) {
			getline(readOrder, line);
			istringstream readLine(line);
			int amount;
			double cost = 0;
			string name;
			string ingredient;
			readLine >> amount;
			readLine.get();
			getline(readLine, name);
			if (!isprint(name.front()))
				name.erase(name.begin());
			if (!isprint(name.back()))
				name.pop_back();
			bool found = false;
			for (int i = 0; i < 14 && !found; i++) {
				if (menu[i].getName() == name) {
					found = true;
					if (menu[i].getIngredients() == "N/A") {
						for (int j = 0; j < 27; j++) {
							if (stock[j].getName() == name) {
								if (stock[j].getItemCount() >= amount) {
									stock[j].subItemCount(amount);
									cost = stock[j].getPrice() * amount;
									checkout += cost;
									cout << amount << " " << name << " cost = " << cost << endl;
								} else cout << "We don't have enough " << name << endl;
								break;
							}
						}
					} else {
						bool issue = false;
						istringstream read1(menu[i].getIngredients());
						string str;
						string temp;
						while (getline(read1, str, ',') && !issue) {
							read1.get();
							istringstream readStr(str);
							readStr >> amount;
							readStr >> temp;
							if (temp == "gram" || temp == "ml") {
								readStr.get();
								getline(readStr, temp);
								if (!isprint(temp.front()))
									temp.erase(temp.begin());
								if (!isprint(temp.back()))
									temp.pop_back();
							} else {
								string t;
								getline(readStr, t);
								temp += t;
								if (!isprint(temp.front()))
									temp.erase(temp.begin());
								if (!isprint(temp.back()))
									temp.pop_back();
							}
							for (int j = 0; j < 27; j++) {
								if (stock[j].getName() == temp) {
									if (stock[j].getItemCount() >= amount) {
										break;
									} else {
										issue = true;
										break;
									}
								}
							}
						}
						if (issue) {
							cout << "We don't have enough " << name << endl;
						} else {
							istringstream read1(menu[i].getIngredients());
							string str;
							string temp;
							while (getline(read1, str, ',')) {
								read1.get();
								istringstream readStr(str);
								readStr >> amount;
								readStr >> temp;
								if (temp == "gram" || temp == "ml") {
									readStr.get();
									getline(readStr, temp);
									if (!isprint(temp.front()))
										temp.erase(temp.begin());
									if (!isprint(temp.back()))
										temp.pop_back();
								} else {
									string t;
									getline(readStr, t);
									temp += t;
									if (!isprint(temp.front()))
										temp.erase(temp.begin());
									if (!isprint(temp.back()))
										temp.pop_back();
								}
								for (int j = 0; j < 27; j++) {
									if (stock[j].getName() == temp) {
										if (stock[j].getItemCount() >= amount) {
											stock[j].subItemCount(amount);
											cost += stock[j].getPrice() * amount;
											break;
										} else {
											break;
										}
									}
								}
							}
							checkout += cost;
							cout << amount << " " << name << " cost: " << cost << endl;
						}
					}
				}
			}
			if (!found)
				cout << "We don't have enough " << name << endl;
			line.clear();
		}
		cout << "Tip is " << (double)checkout * 15 / 100 << endl;
		cout << "Total cost: " << (double)checkout * 123 / 100 << " TL" << endl;
		cout << "**************************" << endl;
		checkout = 0;
	}
	return 0;
}
