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
	void to_big();

	static const size_t size_small = 8;

	struct my_data {
		std::shared_ptr<T> _ptr;
		size_t _capacity;
		my_data(T* a, size_t capacity) :
			_ptr(a), _capacity(capacity) { } 
	};

	size_t _size;
	bool _is_big;
	union small_big_type {
		T _small_data[size_small];
		my_data _heap_data;
		~small_big_type() {}
		small_big_type() {}
		void swap(small_big_type &b) noexcept {
			for(size_t i = 0; i < size_small; i++) {
				std::swap(_small_data[i], b._small_data[i]);
			}
		}
	} _obj;
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

	void update_data();
};

template <class T>
vector<T>::vector() :
	_size(0),
	_is_big(false) {
	update_data();
}

template <class T>
vector<T>::~vector() {
	if(is_big()) {
		_obj._heap_data.~my_data();
	}
}

template <class T>
vector<T>::vector(vector const& a) :
	_size(a.size()),
	_is_big(a._is_big) {
	if(a.is_big()) {
		new 	(&_obj._heap_data) my_data(a._obj._heap_data);
	}
	else {
		for(size_t i = 0; i < size_small; i++) {
			_obj._small_data[i] = a._obj._small_data[i];
		}
	}
	update_data();
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
void vector<T>::swap(vector& a) noexcept {
	using std::swap;
	swap(a._is_big, _is_big);
	a._obj.swap(_obj);
	swap(a._size, _size);
	update_data();
	a.update_data();
}

template <class T>
void vector<T>::to_big() {
	T* temp = copy_data(size(), get_data(), size());
	new 	(&_obj._heap_data) my_data(temp, size());
	_is_big = true;
	update_data();
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
	if (is_big() || capacity > size_small) {
		if (!is_big()) {
			to_big();
		}
		T* temp = copy_data(capacity, get_data(), size());
		_obj._heap_data._ptr.reset(temp, vector<T>::my_deleter());
		_obj._heap_data._capacity = capacity;
		_cur_data = _obj._heap_data._ptr.get();
	}
}

template <class T>
void vector<T>::change() {
	if (is_big() && !_obj._heap_data._ptr.unique()) {
		_obj._heap_data._ptr.reset(copy_data(size(), get_data(), size()),
			vector<T>::my_deleter());
		_cur_data = _obj._heap_data._ptr.get();
	}
}

template <class T>
bool vector<T>::is_big() const {
	return _is_big;
}

template <class T>
size_t vector<T>::get_capacity() {
	if (is_big()) {
		return _obj._heap_data._capacity;
	}
	return size_small;
}

template <class T>
T* vector<T>::get_data() {
	return _cur_data;
}

template <class T>
void vector<T>::update_data() {
	if(is_big()) {
		_cur_data = _obj._heap_data._ptr.get();
	}
	else {
		_cur_data = _obj._small_data;
	}
}
#endif // VECTOR