#pragma once
#include<iostream>

// Creating generic Node using template
template <typename T>
struct Node
{
	T data;			// Generic Type Data
	Node<T>* prev;	// Previous Generic Type pointer
	Node<T>* next;	// Next Generic Type pointer
};

// Creating generic List using template
template <typename T>
class List
{
private:
	Node<T>* first;
	Node<T>* last;

public:
	int count = 0;

	// Generic List Constructor
	List()
	{
		// Initializing
		this->first = NULL;
		this->last = NULL;
	}

	// This function adds data to end of the List
	void Add(T data)
	{
		// If first == NULL
		if (!this->first) {
			// The list is empty
			this->first = new Node<T>;
			this->first->data = data;
			this->first->next = NULL;
			this->first->prev = NULL;
			this->last = this->first;
		}
		else {
			// The list isn't empty
			if (this->last == this->first) {
				// The list has one element
				this->last = new Node<T>;
				this->last->data = data;
				this->last->next = NULL;
				this->first->next = this->last;
				this->last->prev = this->first;
			}
			else 
			{
				// The list has more than one element
				Node<T>* tmp = new Node<T>;
				tmp->data = data;
				tmp->next = NULL;
				this->last->next = tmp;
				tmp->prev = last;
				this->last = tmp;
			}
		}

		// Incrementing the count
		this->count++;
	}

	// This function inserts data at some index in the List
	void Insert(T data, int index)
	{
		/*
		Insert data at some index
		*/

		if (index > this->count || index < 0)
		{
			throw "List::Insert::Cannot add insert node at invalid position";
			return;
		}

		Node<T>* tmp = this->first;
		Node<T>* newNode = new Node<T>;
		if (index == 0)
		{
			newNode->data = data;
			newNode->next = this->first;
			newNode->prev = NULL;
			this->first->prev = newNode;
			this->first = newNode;
			this->count++;
			return;
		}

		if (index == this->count)
		{
			newNode->data = data;
			newNode->next = NULL;
			newNode->prev = this->last;
			this->last->next = newNode;
			this->last = newNode;
			this->count++;
			return;
		}

		for (int i = 0; i < index - 1; i++)
		{
			tmp = tmp->next;
		}

		newNode->data = data;
		newNode->next = tmp->next;
		tmp->next->prev = newNode;
		tmp->next = newNode;
		newNode->prev = tmp;
		this->count++;
	}

	// This function replaces the data at some index in the List
	void Replace(T data, int index)
	{
		if (index >= this->count || index < 0)
		{
			throw "List::Insert::Cannot add insert node at invalid position";
			return;
		}

		Node<T>* tmp = this->first;
		for (int i = 0; i < index; i++)
		{
			tmp = tmp->next;
		}

		tmp->data = data;
	}

	// This function deletes the specified index from the List
	void DeleteIndex(int index)
	{
		if (index >= this->count || index < 0)
		{
			throw "List::DeleteIndex::Cannot add insert node at invalid position";
			return;
		}

		// Index == 0
		if (index == 0)
		{
			this->first = this->first->next;
			if (this->first != NULL)
				this->first->prev = NULL;

			if(this->count > 0)
				this->count -= 1;

			return;
		}

		// Index == this->count-1
		if (index == this->count-1)
		{
			this->last = this->last->prev;
			if(this->last != NULL)
				this->last->next = NULL;

			if(this->count > 0)
				this->count -= 1;

			return;
		}

		// Else remove the ith index
		Node<T>* tmp = this->first;
		for (int i = 0; i < index; i++)
		{
			tmp = tmp->next;
		}

		if (tmp->prev != NULL)
			tmp->prev->next = tmp->next;

		if(tmp->next != NULL)
			tmp->next->prev = tmp->prev;

		// Update the count
		if(this->count > 0)
			this->count -= 1;
	}

	// Get first element in the List
	T GetFirst()
	{
		return this->first->data;
	}

	// Get the last element in the List
	T GetLast()
	{
		return this->last->data;
	}

	// Get function
	T Get(int index)
	{
		if (index >= this->count || index < 0)
		{
			throw "List::Get::INVALID INDEX";
		}

		Node<T>* tmp = this->first;
		for (int i = 0; i < index; i++)
		{
			tmp = tmp->next;
		}

		return tmp->data;
	}

	// Index operator overloading
	T operator[](int index) {
		return Get(index);
	}

	// Pops the first element in the List
	void PopFirst()
	{
		// Moves head to next
		// TODO delete this in future as well
		this->first = this->first->next;

		if (this->first != NULL)
			this->first->prev = NULL;

		// Update the count
		if(this->count > 0)
			this->count -= 1;
	}

	// Pops the last element in the List
	void PopLast()
	{
		// Moves head to next
		// TODO delete this in future as well
		this->last = this->last->prev;

		if (this->last != NULL)
			this->last->next = NULL;

		// Update the count
		if (this->count > 0)
			this->count -= 1;
	}

	// Checks if the List is empty
	bool Empty()
	{
		if (this->count <= 0)
			return true;

		return false;
	}

	// Checks if the List constains the data
	bool Contains(T data)
	{
		Node<T>* tmp = this->first;
		for (int i = 0; i < this->count; i++) {
			if (tmp->data == data)
				return true;

			tmp = tmp->next;
		}

		return false;
	}
};