#pragma once
#ifndef VECTOR
#define VECTOR
#include <cstring>
#include <type_traits>
#include <memory>

template <class T>
T* copy_data(size_t capacity, T* data, size_t size,
             typename std::enable_if<std::is_trivially_constructible<T>::value>::type* = nullptr) {
	T* temp = static_cast<T*>(operator new(capacity * sizeof(T)));
	memcpy(temp, data, sizeof(T) * size);
	return temp;
}

template <class T>
struct vector {
	vector();
	~vector();
	vector(vector const& a);
	vector(size_t size, T const& a);
	explicit vector(size_t size);

	friend bool operator==(vector const& a, vector const& b) {
		if (a.size() != b.size()) {
			return false;
		}
		return memcmp(a._cur_data, b._cur_data, a.size() * sizeof(T)) == 0;
	}

	vector& operator=(vector a);

	T& operator[](size_t ind);
	T const& operator[](size_t ind) const;

	T& front();
	T const& front() const;

	T& back();
	T const& back() const;

	bool empty() const;
	size_t size() const;

	void reserve(size_t n);

	void push_back(T value);
	void pop_back();

	void swap(vector& a) noexcept;

private:
	void switch_to_big();
	static const size_t SMALL_SIZE = 8;

	struct big_obj {
		std::shared_ptr<T> ptr;
		size_t capacity;

		big_obj(T* a, size_t capacity);
		void swap(big_obj& b) noexcept;
	};

	size_t _size;

	union small_or_big_obj {
		T small_data[SMALL_SIZE];
		big_obj big_data;

		~small_or_big_obj() {}
		small_or_big_obj() {}
	} _data;

	T* _cur_data;

	struct my_deleter {
		void operator()(T* p) {
			operator delete(p);
		}
	};

	static size_t get_new_capacity(size_t n);
	void set_capacity(size_t capacity);
	void change();

	bool is_big() const;

	size_t get_capacity();
	T* get_data();

	static void swap_big_small(small_or_big_obj& a, small_or_big_obj& b);
};

template <class T>
vector<T>::vector() :
	_size(0),
	_cur_data(_data.small_data) { }

template <class T>
vector<T>::~vector() {
	if (is_big()) {
		_data.big_data.~big_obj();
	}
}

template <class T>
vector<T>::vector(vector const& a) : _size(a.size()) {
	if (a.is_big()) {
		new(&_data.big_data) big_obj(a._data.big_data);
		_cur_data = a._cur_data;
	}
	else {
		for (size_t i = 0; i < SMALL_SIZE; i++) {
			_data.small_data[i] = a._data.small_data[i];
		}
		_cur_data = _data.small_data;
	}
}

template <class T>
vector<T>::vector(size_t size, T const& a) : vector() {
	reserve(size);
	for (size_t i = 0; i < size; i++) {
		push_back(a);
	}
}

template <class T>
vector<T>::vector(size_t size) : vector(size, T()) { }

template <class T>
vector<T>& vector<T>::operator=(vector a) {
	swap(a);
	return *this;
}

template <class T>
T& vector<T>::operator[](size_t ind) {
	change();
	return _cur_data[ind];
}

template <class T>
T const& vector<T>::operator[](size_t ind) const {
	return _cur_data[ind];
}

template <class T>
T& vector<T>::front() {
	return (*this)[0];
}

template <class T>
T const& vector<T>::front() const {
	return (*this)[0];
}

template <class T>
T& vector<T>::back() {
	return (*this)[size() - 1];
}

template <class T>
T const& vector<T>::back() const {
	return (*this)[size() - 1];
}

template <class T>
bool vector<T>::empty() const {
	return _size == 0;
}

template <class T>
size_t vector<T>::size() const {
	return _size;
}

template <class T>
void vector<T>::reserve(size_t n) {
	if (get_capacity() < n) {
		set_capacity(get_new_capacity(n));
	}
}

template <class T>
void vector<T>::push_back(T value) {
	reserve(size() + 1);
	(*this)[size()] = value;
	_size++;
}

template <class T>
void vector<T>::pop_back() {
	change();
	_size--;
}

template <class T>
void vector<T>::swap_big_small(small_or_big_obj& a, small_or_big_obj& b) {
	T temp[SMALL_SIZE] = {};
	for (size_t i = 0; i < SMALL_SIZE; i++) {
		temp[i] = b.small_data[i];
	}
	new(&b.big_data) big_obj(a.big_data);
	a.big_data.~big_obj();
	for (size_t i = 0; i < SMALL_SIZE; i++) {
		a.small_data[i] = temp[i];
	}
}

template <class T>
void vector<T>::swap(vector& a) noexcept {
	using std::swap;
	if (is_big() && a.is_big()) {
		swap(_data.big_data, a._data.big_data);
		_cur_data = _data.big_data.ptr.get();
		a._cur_data = a._data.big_data.ptr.get();
	}
	else if (!is_big() && !a.is_big()) {
		for (size_t i = 0; i < SMALL_SIZE; i++) {
			swap(_data.small_data[i], a._data.small_data[i]);
		}
		_cur_data = _data.small_data;
		a._cur_data = a._data.small_data;
	}
	else if (!is_big() && a.is_big()) {
		swap_big_small(a._data, _data);
		_cur_data = _data.big_data.ptr.get();
		a._cur_data = a._data.small_data;
	}
	else if (is_big() && !a.is_big()) {
		swap_big_small(_data, a._data);
		_cur_data = _data.small_data;
		a._cur_data = a._data.big_data.ptr.get();
	}
	swap(a._size, _size);
}

template <class T>
void vector<T>::switch_to_big() {
	T* temp = copy_data(size(), get_data(), size());
	new(&_data.big_data) big_obj(temp, size());
	_cur_data = temp;
}

template <class T>
vector<T>::big_obj::big_obj(T* a, size_t capacity):
	ptr(a), capacity(capacity) { }

template <class T>
void vector<T>::big_obj::swap(big_obj& b) noexcept {
	swap(ptr, b.ptr);
	swap(capacity, b.capacity);
}

template <class T>
size_t vector<T>::get_new_capacity(size_t n) {
	if (n == 0) {
		return 4;
	}
	return n * 3 / 2;
}

template <class T>
void vector<T>::set_capacity(size_t capacity) {
	if (is_big() || capacity > SMALL_SIZE) {
		if (!is_big()) {
			switch_to_big();
		}
		T* temp = copy_data(capacity, get_data(), size());
		_data.big_data.ptr.reset(temp, vector<T>::my_deleter());
		_data.big_data.capacity = capacity;
		_cur_data = _data.big_data.ptr.get();
	}
}

template <class T>
void vector<T>::change() {
	if (is_big() && !_data.big_data.ptr.unique()) {
		_data.big_data.ptr.reset(copy_data(size(), get_data(), size()),
		                          vector<T>::my_deleter());
		_cur_data = _data.big_data.ptr.get();
	}
}

template <class T>
bool vector<T>::is_big() const {
	return _cur_data != _data.small_data;
}

template <class T>
size_t vector<T>::get_capacity() {
	if (is_big()) {
		return _data.big_data.capacity;
	}
	return SMALL_SIZE;
}

template <class T>
T* vector<T>::get_data() {
	return _cur_data;
}
#endif // VECTOR
