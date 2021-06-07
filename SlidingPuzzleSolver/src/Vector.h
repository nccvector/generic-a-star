#include <assert.h>
#include <cstring>
#include <iostream>

const int INITIAL_CAPACITY = 2;
const int GROWTH_FACTOR = 2;

template <class T>
class Vector {
    T *array;
    int capacity = INITIAL_CAPACITY;
    int _size;

public:
    Vector() {
        array = new T[capacity];
        _size = 0;
    }

    ~Vector() {
        delete[] array;
    }

    void deleteAt(int pos) {
        assert(0 <= pos && pos < _size);
        _size--;
        for (int i = pos; i < _size; i++) {
            array[i] = array[i + 1];
        }
    }

    void insertAt(T element, int pos) {
        assert(0 <= pos && pos <= _size);
        if(_size == capacity) {
            resize();
        }
        for(int i = _size; i > pos; i--) {
            array[i] = array[i-1];
        }
        _size++;
        array[pos] = element;
    }

    void append(T element) {
        insertAt(element, _size);
    }

    int size() {
        return _size;
    }

    // doubles capacity if it has to and deletes reference to current array.
    void resize() {
        capacity *= GROWTH_FACTOR;
        T *temp = new T[capacity];
        std::copy(array, array + _size, temp);
        delete [] array;
        array = temp;
    }

    T get(int pos) {
        return array[pos];
    }

    // Index operator overloading
	T operator[](int index) {
		return array[index];
	}
};