#pragma once
#ifndef VECTOR
#define VECTOR

template <class T>
struct vector {
	typedef T* iterator;
	typedef T const* const_iterator;

	vector();
	vector(vector const& a);
	~vector();

	vector& operator=(vector a);

	T& operator[](size_t ind);
	T const& operator[](size_t ind) const;

	T& front();
	T const& front() const;

	T& back();
	T const& back() const;

	T* data();
	T const* data() const;

	bool empty() const; // быстрее size == 0
	size_t size() const; // в стандартной реализации выполняет вычитаение и деление на sizeof

	size_t capacity() const;
	void reserve(size_t n);
	void shrink_to_fit();

	void clear();

	void push_back(T const& value);
	void pop_back(); // в 03 может бросить эксепшен поэтому он не возвращает последний элемент. в 11 можно мув, но нет.

	void swap(vector& a) noexcept;

	iterator insert(iterator pos, const T& value);
	iterator insert(const_iterator pos, const T& value);

	iterator erase(iterator pos);
	iterator erase(const_iterator pos);

	iterator erase(iterator first, iterator last);
	iterator erase(const_iterator first, const_iterator last);

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

private:
	T* _data;
	size_t _size;
	size_t _capacity;

	static size_t get_new_capacity(size_t n);
	void set_capacity(size_t capacity);
	void delete_all();
};

template <class T>
vector<T>::vector() :
	_data(nullptr),
	_size(0),
	_capacity(0) { }

template <class T>
vector<T>::vector(vector const& a) {
	_data = copy_data(a._size, a._data, a._size);
	_size = a._size;
	_capacity = a._size;
}

template <class T>
vector<T>::~vector() {
	delete_all();
}

template <class T>
vector<T>& vector<T>::operator=(vector a) {
	swap(a);
	return *this;
}

template <class T>
T& vector<T>::operator[](size_t ind) {
	return _data[ind];
}

template <class T>
T const& vector<T>::operator[](size_t ind) const {
	return _data[ind];
}

template <class T>
T& vector<T>::front() {
	return _data[0];
}

template <class T>
T const& vector<T>::front() const {
	return _data[0];
}

template <class T>
T& vector<T>::back() {
	return _data[_size - 1];
}

template <class T>
T const& vector<T>::back() const {
	return _data[_size - 1];
}

template <class T>
T* vector<T>::data() {
	return _data;
}

template <class T>
T const* vector<T>::data() const {
	return _data;
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
size_t vector<T>::capacity() const {
	return _capacity;
}

template <class T>
void vector<T>::reserve(size_t n) {
	if (_capacity < n) {
		set_capacity(get_new_capacity(n));
	}
}

template <class T>
void vector<T>::shrink_to_fit() {
	set_capacity(_size);
}

template <class T>
void vector<T>::clear() {
	delete_data(_data, _size);
	_size = 0;
}

template <class T>
void vector<T>::push_back(T const& value) {
	if (_size == _capacity) {
		size_t capacity = get_new_capacity(_size + 1);
		T* temp = copy_data(capacity, _data, size());
		new(&temp[_size]) T(value);
		delete_all();
		_data = temp;
		_capacity = capacity;
	}
	else {
		new(&(*this)[_size]) T(value);
	}
	_size++;
}

template <class T>
void vector<T>::pop_back() {
	_data[_size - 1].~T();
	_size--;
}

template <class T>
void vector<T>::swap(vector& a) noexcept {
	using std::swap;
	swap(a._data, _data);
	swap(a._size, _size);
	swap(a._capacity, _capacity);
}

template <class T>
typename vector<T>::iterator vector<T>::insert(iterator pos, const T& value) {
	return insert(static_cast<const_iterator>(pos), value);
}

template <class T>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, const T& value) {
	ptrdiff_t d = pos - _data;
	push_back(value);
	for (iterator i = end() - 1; i != d + _data; --i) {
		std::swap(*i, *(i - 1));
	}
	return d + _data;
}

template <class T>
typename vector<T>::iterator vector<T>::erase(iterator pos) {
	ptrdiff_t d = pos - _data;
	for (iterator i = pos; i + 1 != end(); ++i) {
		std::swap(*i, *(i + 1));
	}
	pop_back();
	return d + _data;
}

template <class T>
typename vector<T>::iterator vector<T>::erase(const_iterator pos) {
	return erase(_data + (pos - _data));
}

template <class T>
typename vector<T>::iterator vector<T>::erase(iterator first, iterator last) {
	ptrdiff_t d = first - _data;
	for (iterator i = first, e = last; e != end(); ++i , ++e) {
		std::swap(*i, *e);
	}
	for (ptrdiff_t dl = last - first; dl > 0; dl--) {
		pop_back();
	}
	return d + _data;
}

template <class T>
typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last) {
	return erase(_data + (first - _data), _data + (last - _data));
}

template <class T>
typename vector<T>::iterator vector<T>::begin() {
	return _data;
}

template <class T>
typename vector<T>::iterator vector<T>::end() {
	return _data + _size;
}

template <class T>
typename vector<T>::const_iterator vector<T>::begin() const {
	return _data;
}

template <class T>
typename vector<T>::const_iterator vector<T>::end() const {
	return _data + _size;
}

template <class T>
size_t vector<T>::get_new_capacity(size_t n) {
	if (n == 0) {
		return 8;
	}
	return n * 3 / 2;
}

template <class T>
void vector<T>::set_capacity(size_t capacity) {
	T* temp = copy_data(capacity, _data, size());
	delete_all();
	_data = temp;
	_capacity = capacity;
}

template <class T>
void vector<T>::delete_all() {
	if (_capacity > 0) {
		delete_data(_data, _size);
		operator delete(_data);
	}
}

template <class T>
T* copy_data(size_t capacity, T* data, size_t size,
             typename std::enable_if<!std::is_trivially_constructible<T>::value>::type* = nullptr) {
	T* temp = static_cast<T*>(operator new(capacity * sizeof(T)));
	for (size_t i = 0; i < size; i++) {
		try {
			new(&temp[i]) T(data[i]);
		}
		catch (...) {
			delete_data(temp, i);
			operator delete(temp);
			throw;
		}
	}
	return temp;
}

template <class T>
T* copy_data(size_t capacity, T* data, size_t size,
             typename std::enable_if<std::is_trivially_constructible<T>::value>::type* = nullptr) {
	T* temp = static_cast<T*>(operator new(capacity * sizeof(T)));
	memcpy(temp, data, sizeof(T) * size);
	return temp;
}

template <class T>
void delete_data(T* data, size_t size,
                 typename std::enable_if<!std::is_trivially_destructible<T>::value>::type* = nullptr) {
	for (size_t j = 0; j < size; j++) {
		data[j].~T();
	}
}

template <class T>
void delete_data(T* data, size_t size,
                 typename std::enable_if<std::is_trivially_destructible<T>::value>::type* = nullptr) { }
#endif // VECTOR


/* чтоб сделать итератор через указатель нужно использовать iterator_traits. Это будет корректно работать всегда, так как внутри есть правильное переопределение.
 * темплейтные функции нужно писать в хедере!!!
 * 
 * 
 * Не забывай, что можно писать insert(T.begin(), T[0]), при передачи по ссылке может испортиться значение при удаление
 * 
 * 
 * is_trivially_copyable проверить, что тип имеет примитивный конструктор копирования, позвоялет использовать memcpy
 * 
 * is_trivially_destructible можно не использовать оператор delete 
 * 
 * 
 * 
 * 
 * is_trivially_copyable:
 * template <bool C>
 * struct enable_if;
 * 
 * template <>
 * struct enable_if<true> {
 *	typedef void type;
 * };
 * 
 * 
 * template <typename T>
 * void delete_all(T* data, size_t size,
 *			void* = enable_if<!std::is_trivially_copyable<T>::value>::type*  = nullptr) {
 *		...	
 * }
 * 
 * 
 */
