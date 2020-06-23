#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <locale>

using namespace std;

void Menu()
{
	cout << "------------------" << endl;
	cout << "1. List" << endl;
	cout << "2. Add" << endl;
	cout << "3. Modify" << endl;
	cout << "4. Remove" << endl;
	cout << "5. Save and Quit" << endl;
	cout << "------------------" << endl;
}

class Setting
{
public:
	Setting()
	{
		name = "";
		val_str = "";
		val_int = 0;
		is_number = false;
	}

	Setting(const string set_name, const string val)
	{
		name = set_name;
		val_str = val;
		is_number = false;

		val_int = 0;
	}

	Setting(const string set_name, int val)
	{
		name = set_name;
		val_int = val;
		is_number = true;

		val_str = "";
	}

	string name;
	bool is_number;

	string val_str;
	int val_int;
};


class Section
{
public:
	Section(const string sect_name)
	{
		name = sect_name;
	}

	void Add_Item(const Setting& new_item)
	{
		items.push_back(new_item);
	}

	string name;
	vector<Setting> items;
};

bool FindSection(const string line)
{
	bool result = false;

	if (line.length() >= 2)
	{
		if ((line[0] == '[') && (line[line.length() - 1] == ']'))
		{
			result = true;
		}
	}

	return result;
}

string Trim(string line)
{
	size_t a = line.find("[");
	size_t b = line.find("]");
	size_t c = b - a - 1;
	return line.substr(1,c);
}

Setting Func(const string line)
{
	Setting set;

	size_t pos = line.find("=");

	if (pos < line.length())
	{
		// We found the = character
		string set_name = line.substr(0, pos);
		string val = line.substr(pos + 1);
		bool is_num = true;
		for (size_t i = 0; i < val.length(); i++)
		{
			if (!isdigit(val[i]))
			{
				is_num = false;
			}
		}

		if (is_num)
		{
			set = Setting(set_name, stoi(val));
		}
		else
		{
			set = Setting(set_name, val);
		}
	}
	else
	{
		throw exception();
	}

	return set;
}


int main(void)
{
	string line;
	ifstream myfile("Setting.txt");

	vector<Section> sections;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line.length() > 0)
			{
				bool check = FindSection(line);

				if (check == true)
				{
					// Line is a section marker
					string section_name = line.substr(1, line.length() - 2);

					sections.emplace_back(Section(section_name));
				}
				else
				{
					// Line is a setting
					try
					{
						if (sections.size() > 0)
						{
							sections.back().Add_Item(Func(line));
						}
					}
					catch(exception ex)
					{
						// Invalid line, possibly empty line -- ignore it
					}
				}
			}
		}
	}
	else cout << "Unable to open file!" << endl;

	bool Quit = false;

	while (!Quit)
	{
		int choice;
		Menu();
		cin >> choice;
		if (choice == 1) //List
		{
			for (size_t i = 0; i < sections.size(); i++)
			{
				cout << "[ " << sections[i].name << " ]" << endl;
				for (size_t j = 0; j < sections[i].items.size(); j++)
				{
					cout << sections[i].items[j].name << " = ";
					if (sections[i].items[j].is_number)
					{
						cout << sections[i].items[j].val_int;
					}
					else
					{
						cout << "'" << sections[i].items[j].val_str << "'";
					}

					cout << endl;
				}		
			}
		}
		else if (choice == 2) //Add
		{
			string section;
			cout << "Which section you want to add?" << endl;
			cin >> section;
			for (size_t i = 0; i < sections.size(); i++)
			{
				if (sections[i].name == section)
				{
					string key;
					cout << "Enter new key: ";
					cin >> key;
					char is_num;
					cout << "Is that number(y/n)?: ";
					cin >> is_num;
					if (is_num == 'y')
					{
						int val_int;
						cout << "Enter new number: ";
						cin >> val_int;
						Setting set(key, val_int);
						sections[i].items.push_back(set);
					}
					else if (is_num == 'n')
					{
						string val_str;
						cout << "Enter new string: ";
						getline(cin, val_str);
						Setting set(key, val_str);
						sections[i].items.push_back(set);
					}
				}
			}
		}
		else if (choice == 3) //Modify
		{
			string section;
			cout << "Which section you want to modify?" << endl;
			cin >> section;
			for (size_t i = 0; i < sections.size(); i++)
			{
				if (sections[i].name == section)
				{
					string key;
					cout << "Enter key you want to modify: ";
					cin >> key;
					for (size_t j = 0; j < sections[i].items.size(); j++)
					{
						if (sections[i].items[j].name == key)
						{
							char is_num;
							cout << "Is that number(y/n)?: ";
							cin >> is_num;
							if (is_num == 'y')
							{
								int val_int;
								cout << "Enter new number: ";
								cin >> val_int;
								Setting set(key, val_int);
								sections[i].items[j] = set;
							}
							else if (is_num == 'n')
							{
								string val_str;
								cout << "Enter new string: ";
								getline(cin, val_str);
								Setting set(key, val_str);
								sections[i].items[j] = set;
							}
						}
					}
				}
			}
		}
		else if (choice == 4) // Remove
		{
			string section;
			cout << "Which section has the key you want to delete?" << endl;
			cin >> section;
			for (size_t i = 0; i < sections.size(); i++)
			{
				if (sections[i].name == section)
				{
					string key;
					cout << "Enter key you want to delete: ";
					cin >> key;
					for (size_t j = 0; j < sections[i].items.size(); j++)
					{
						if (sections[i].items[j].name == key)
						{
							sections[i].items.erase(sections[i].items.begin()+j);
						}
					}
				}
			}
		}
		else if (choice == 5) // Save and Quit
		{
			ofstream myfile("Setting.txt");
			for (size_t i = 0; i < sections.size(); i++)
			{
				myfile << "[" << sections[i].name << "]" << endl;
				for (size_t j = 0; j < sections[i].items.size(); j++)
				{
					myfile << sections[i].items[j].name << "=";
					if (sections[i].items[j].is_number)
					{
						myfile << sections[i].items[j].val_int;
					}
					else
					{
						myfile << sections[i].items[j].val_str;
					}

					myfile<< endl;
				}
			}
			Quit = true;
		}
		else
			cout << "Invalid option!" << endl;
	}

	return 0;
}