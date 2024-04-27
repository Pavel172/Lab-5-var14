#include <iostream>
#include <string>
#include <deque>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <unordered_set>
using namespace std;

class Book //Класс - книга
{
public:
	string FIO;
	string Title;
	string publishing_house;
	int year;
	int pages;
	Book() //Пустой конструктор
	{
		FIO = "";
		Title = "";
		publishing_house = "";
		year = 0;
		pages = 0;
	}
	Book(string FIO, string Title, string publishing_house, int year, int pages) //Конструктор с начальными значениями
	{
		this->FIO = FIO;
		this->Title = Title;
		this->publishing_house = publishing_house;
		this->year = year;
		this->pages = pages;
	}
	Book(const Book& book) //Конструктор копирования
	{
		this->FIO = book.FIO;
		this->Title = book.Title;
		this->publishing_house = book.publishing_house;
		this->year = book.year;
		this->pages = book.pages;
	}
	Book(Book&& book) //Конструктор перемещения
	{
		this->FIO = book.FIO;
		this->Title = book.Title;
		this->publishing_house = book.publishing_house;
		this->year = book.year;
		this->pages = book.pages;
		book.FIO = "";
		book.Title = "";
		book.publishing_house = "";
		book.year = 0;
		book.pages = 0;
	}
	Book& operator = (const Book& book) //Перегрузка оператора присваивания с копированием
	{
		if (this != &book)
		{
			this->FIO = book.FIO;
			this->Title = book.Title;
			this->publishing_house = book.publishing_house;
			this->year = book.year;
			this->pages = book.pages;
		}
		return *this;
	}
	Book& operator = (Book&& book) //Перегрузка оператора присваивания с перемещением
	{
		if (this != &book)
		{
			this->FIO = book.FIO;
			this->Title = book.Title;
			this->publishing_house = book.publishing_house;
			this->year = book.year;
			this->pages = book.pages;
			book.FIO = "";
			book.Title = "";
			book.publishing_house = "";
			book.year = 0;
			book.pages = 0;
		}
		return *this;
	}
	friend ostream& operator << (ostream& fout, const Book& book); //Описание дружественной перегруженной операции вставки объекта в поток cout
	bool operator < (const Book& book) const //Перегрузка оператора < для сортировки в set  [Лабораторная 5] 
	{
		return size(FIO) < size(book.FIO);
	}
	bool operator ==(const Book& book) const //Перегрузка оператора == для unordered_set  [Лабораторная 5]
	{ 
		return size(FIO) == size(book.FIO);
	}
};

struct Hasher  //Структура с функтором [Лабораторная 5]
{
	// Хешеры для отдельных полей
	hash<string> shash;
	hash<int> ihash;
	hash <double> dhash;
	size_t operator()(const Book& obj) const //Функтор для задания хеш-функции для unordered_set [Лабораторная 5]
	{
		const size_t coef = 5171;
		return (coef * coef * coef * coef * shash(obj.FIO) + coef * coef * coef * shash(obj.Title) + coef * coef * shash(obj.publishing_house) + coef * ihash(obj.year) + dhash(obj.pages));
	}
};

ostream& operator << (ostream& fout, const Book& book) //Объявление дружественной перегруженной операции вставки объекта в поток cout
{
	fout << "Автор: " << book.FIO << endl;
	fout << "Название: " << book.Title << endl;
	fout << "Издательство: " << book.publishing_house << endl;
	fout << "Год издания: " << book.year << endl;
	fout << "Количество страниц: " << book.pages << endl;
	return fout;
}

bool bool_sort(Book book1, Book book2) //булева функция для правильной сортировки элементов контейнера deque
{ 
	return size(book1.FIO) > size(book2.FIO); 
}



int main()
{
	setlocale(LC_ALL, "Rus"); //реализуем возможность печати русских букв
	set <Book> books_set; //Создаём контейнер множество  [Лабораторная 5]
	unordered_set <Book, Hasher> books_unord_set; //Создаём контейнер неупорядоченное множество  [Лабораторная 5]
	deque<Book> books_deque; //Создаём контейнер очередь
	list<Book> books_list; //Создаём контейнер список
	string temp, fio, title, publish, pages_str, year_str;
	int count = -1, flag = 0, pages = 0, year = 0;
	deque <string> temp_deque;
	ifstream fin;
	fin.open("input.txt"); //Открываем файл для чтения
	while (!fin.eof()) //Цикл поочередной записи всех строк из файла в вспомогательную очередь
	{
		getline(fin, temp);
		temp_deque.push_back(temp);
		temp = "";
		++count;
	}
	fin.close(); //закрываем файл после чтения
	for (int i = 0; i < count; ++i) //Основной цикл работы с данными, полученными из файла. На выходе получаем очередь(books_deque) из объектов класса Book 
	{
		temp = temp_deque[i];
		for (int j = 0; j < size(temp); ++j) //Цикл, с помощью которого мы сплошную строку разбиваем на строки для полей класса Book
		{
			if (temp[j] == ';')
			{
				++flag;
				continue;
			}
			if (flag == 0)
			{
				fio += temp[j];
			}
			if (flag == 1)
			{
				title += temp[j];
			}
			if (flag == 2)
			{
				publish += temp[j];
			}
			if (flag == 3)
			{
				year_str += temp[j];
			}
			if (flag == 4)
			{
				pages_str += temp[j];
			}
		}
		istringstream(year_str) >> year; //Преобразуем строку в число
		istringstream(pages_str) >> pages; //Преобразуем строку в число
		Book book(fio, title, publish, year, pages); //Создаём объект класса Book
		books_set.insert(book); //Добавляем объект класса Book в множество books_set  [Лабораторная 5]
		books_unord_set.insert(book); //Добавляем объект класса Book в неупорядоченное множество books_unord_set  [Лабораторная 5]
		books_deque.push_back(book); //Добавляем объект класса Book в очередь books_deque
		temp = fio = title = publish = year_str = pages_str = "";
		flag = 0;
	}
	copy(books_deque.begin(), books_deque.end(), inserter(books_list, books_list.begin())); //Вызов функции копирования контейнера типа deque в контейнер типа list
	ofstream fout;
	fout.open("output.txt"); //Открываем файл для записи
	fout << "Исходный контейнер deque:" << endl;
	for (int i = 0; i < count; ++i)
	{
		fout << books_deque[i] << endl;
	}
	fout << endl;
	sort(books_deque.begin(), books_deque.end(), bool_sort); //Вызов функции сортировки контейнера deque при помощи булевой функции
	fout << "Исходный контейнер deque после сортировки:" << endl;
	for (int i = 0; i < count; ++i)
	{
		fout << books_deque[i] << endl;
	}
	fout << endl;
	fout << "Контейнер, в который копирован исходный контейнер(list):" << endl;
	for (auto i = books_list.begin(); i != books_list.end(); ++i) //Цикл для вывода контейнера типа list в файл
	{
		fout << *i << endl;
	}
	fout << endl;
	fout << "Контейнер set:" << endl;
	for(auto i = books_set.begin(); i != books_set.end(); ++i) //Цикл для вывода контейнера типа set в файл  [Лабораторная 5]
	{
		fout << *i << endl;
	}
	fout << "Контейнер unordered_set:" << endl;
	for (auto i = books_unord_set.begin(); i != books_unord_set.end(); ++i) //Цикл для вывода контейнера типа unordered_set в файл  [Лабораторная 5]
	{
		fout << *i << endl;
	}
	fout.close(); //Закрываем файл после записи
	return 0;
}