#ifndef C_VLVECTOR_HPP
#define C_VLVECTOR_HPP

#include <iostream>

/**
 * @class VLVector2
 * @brief Better version of vector regarding runtime.
 */
template<class Type, size_t StaticCapacity = 16>
class VLVector2
{
private:
	size_t _size;
	size_t _capacity;
	Type _staticArray[StaticCapacity];
	Type *_heapArray;

	/// @brief Change from static memory onto heap memory when needed + update capacity
	void _updateHeap();

	/// @brief Change from heap memory onto static memory when needed
	void _heapToStatic();

public:
	typedef Type *iterator; //no need to make new class since the iterator is address for array
	// which already has all the iterator actions
	typedef const Type *const_iterator;

	/**
     * @brief Constructor.
     */
	VLVector2() : _size(0), _capacity(StaticCapacity), _heapArray(nullptr)
	{}

	/**
	 * @brief Constructor.
	 * @param first: iterator that points to the first element
	 * @param last: iterator that points to the following to the last element
	 */
	template<class InputIterator>
	VLVector2(InputIterator first, const InputIterator last);

	/**
     * @brief Constructor.
     * @param other: other VLVector2
     */
	VLVector2(const VLVector2 &other) : VLVector2(other.begin(), other.end())
	{}

	/**
	 * @brief Constructor.
	 * @param other: other VLVector2
	 */
	VLVector2 &operator=(const VLVector2 &other);

	/**
	 * @brief Destructor.
	 */
	~VLVector2();

	/**
 	 * @brief Return current size.
 	 */
	size_t size() const noexcept
	{ return _size; }

	/**
  	 * @brief Return current capacity.
  	 */
	size_t capacity() const noexcept
	{ return _capacity; }

	/**
 	 * @brief Return true if the vector is empty, false otherwise.
 	 */
	bool empty() const noexcept
	{ return !_size; }

	/**
 	 * @brief Return the element in the given index/error received if index isn't in range.
 	 * @param index Element place
 	 */
	Type &at(const size_t index) noexcept(false);

	/**
 	 * @brief Return the element in the given index/error received if index isn't in range.
 	 * @param index Element place
 	 */
	const Type &at(const size_t index) const noexcept(false);

	/**
 	 * @brief Push element to the last place; after creating space for it
 	 * @param element Element to add
 	 */
	void push_back(const Type &element);

	/**
 	 * @brief Insert element to specific location
 	 * @param position Location
 	 * @param element Element to insert
 	 */
	iterator insert(iterator position, const Type &element);

	/**
 	 * @brief Insert element to specific location
 	 * @param position Location
 	 * @param element Element to insert
 	 */
	iterator insert(const_iterator position, const Type &element);

	/**
 	 * @brief Insert range of elements to specific location
 	 * @param position Location
 	 * @param first Iterator that points to the first element in the range
 	 * @param last Iterator that points to the address right after the last element in the range
 	 */
	template<class InputIterator>
	iterator insert(iterator position, InputIterator first, const InputIterator last);

	/**
 	 * @brief Insert range of elements to specific location
 	 * @param position Location
 	 * @param first Iterator that points to the first element in the range
 	 * @param last Iterator that points to the address right after the last element in the range
 	 */
	template<class InputIterator>
	iterator insert(const_iterator position, InputIterator first,
					const InputIterator last);

	/**
 	 * @brief Removes the last element
 	 */
	void pop_back();

	/**
 	 * @brief Removes element from a specific location
 	 * @param position Location
 	 */
	iterator erase(iterator position);

	/**
 	 * @brief Removes element from a specific location
 	 * @param position Location
 	 */
	iterator erase(const_iterator position);

	/**
 	 * @brief Removes range of elements from a specific location
 	 * @param first Pointer to the first element
 	 * @param last Pointer to the address right after the last element
 	 */
	iterator erase(iterator first, iterator last);

	/**
 	 * @brief Removes range of elements from a specific location
 	 * @param first Pointer to the first element
 	 * @param last Pointer to the address right after the last element
 	 */
	iterator erase(const_iterator first, const_iterator last);

	/**
	 * @brief Clear the vector
	 */
	void clear();

	/**
 	 * @brief Returns the address of the vector
 	 */
	Type *data() noexcept;

	/**
 	 * @brief Returns the address of the vector
 	 */
	const Type *data() const noexcept;

	/**
 	 * @brief Begin iterator
 	 */
	iterator begin() noexcept
	{ return data(); }

	/**
 	 * @brief End iterator
 	 */
	iterator end() noexcept
	{ return (data() + _size); }

	/**
 	 * @brief Begin iterator
 	 */
	const_iterator begin() const noexcept
	{ return data(); }

	/**
 	 * @brief Begin iterator
 	 */
	const_iterator cbegin() const noexcept
	{ return data(); }

	/**
 	 * @brief End iterator
 	 */
	const_iterator end() const noexcept
	{ return (data() + _size); }

	/**
 	 * @brief End iterator
 	 */
	const_iterator cend() const noexcept
	{ return (data() + _size); }

	/**
 	 * @brief Operator[]
 	 * @param position Place to reach
 	 * @return Element in the given place
 	 */
	Type &operator[](const size_t position);

	/**
 	 * @brief Operator[]
 	 * @param position Place to reach
 	 * @return Element in the given place
 	 */
	const Type &operator[](const size_t position) const;
};

/**
 * @brief Change from static memory onto heap memory when needed + update capacity
 */
template<class Type, size_t StaticCapacity>
void VLVector2<Type, StaticCapacity>::_updateHeap()
{
	_capacity = (int) (_size * 1.5);
	Type *array = new Type[_capacity]();
	if (_heapArray == nullptr)//was on stack
	{
		for (size_t i = 0; i < StaticCapacity; i++)
		{
			array[i] = _staticArray[i];
		}
		_heapArray = array;
	}
	else//was on heap
	{
		for (size_t i = 0; i < _size - 1; i++)
		{
			array[i] = _heapArray[i];
		}
		delete[] _heapArray;
		_heapArray = array;
	}
}

/**
 * @brief Change from heap memory onto static memory when needed
 */
template<class Type, size_t StaticCapacity>
void VLVector2<Type, StaticCapacity>::_heapToStatic()
{
	_capacity = StaticCapacity;
	for (size_t i = 0; i < StaticCapacity; i++)
	{
		_staticArray[i] = _heapArray[i];
	}
	delete[] _heapArray;
	_heapArray = nullptr;
}

/**
 * @brief Constructor.
 * @param first: iterator that points to the first element
 * @param last: iterator that points to the following to the last element
 */
template<class Type, size_t StaticCapacity>
template<class InputIterator>
VLVector2<Type, StaticCapacity>::VLVector2(InputIterator first, const InputIterator last):VLVector2()
{
	while (first != last)
	{
		push_back(*first);
		first++;
	}
}

/**
 * @brief Constructor.
 * @param other: other VLVector2
 */
template<class Type, size_t StaticCapacity>
VLVector2<Type, StaticCapacity> &VLVector2<Type, StaticCapacity>::operator=(const VLVector2 &other)
{
	if (this == &other)
	{
		return *this;
	}
	_size = other._size;
	_capacity = other._capacity;
	for (size_t i = 0; i < StaticCapacity; i++)
	{
		_staticArray[i] = other._staticArray[i];
	}
	if (_heapArray != nullptr)
	{
		delete[] _heapArray;
	}
	_heapArray = new Type[_capacity]();
	for (size_t i = 0; i < _size - 1; i++)
	{
		_heapArray[i] = other._heapArray[i];
	}
	return *this;
}

/**
 * @brief Destructor.
 */
template<class Type, size_t StaticCapacity>
VLVector2<Type, StaticCapacity>::~VLVector2()
{
	if (_heapArray != nullptr)
	{
		delete[] _heapArray;
	}
}

/**
 * @brief Return the element in the given index/error received if index isn't in range.
 * @param index Element place
 */
template<class Type, size_t StaticCapacity>
Type &VLVector2<Type, StaticCapacity>::at(const size_t index) noexcept(false)
{
	if (index >= _size)
	{
		throw std::out_of_range("index is out of range");
	}
	return operator[](index);
}

/**
 * @brief Return the element in the given index/error received if index isn't in range.
 * @param index Element place
 */
template<class Type, size_t StaticCapacity>
const Type &VLVector2<Type, StaticCapacity>::at(const size_t index) const noexcept(false)
{
	if (index >= _size)
	{
		throw std::out_of_range("index is out of range");
	}
	return operator[](index);
}

/**
 * @brief Push element to the last place; after creating space for it
 * @param element Element to add
 */
template<class Type, size_t StaticCapacity>
void VLVector2<Type, StaticCapacity>::push_back(const Type &element)
{
	_size++;
	if (_size > capacity())
	{
		_updateHeap();
	}
	data()[_size - 1] = element;
}

/**
 * @brief Insert element to specific location
 * @param position Location
 * @param element Element to insert
 */
template<class Type, size_t StaticCapacity>
typename VLVector2<Type, StaticCapacity>::iterator
VLVector2<Type, StaticCapacity>::insert(VLVector2::iterator position, const Type &element)
{
	size_t positionIndex = position - begin();
	_size++;
	if (_size > _capacity)
	{
		_updateHeap();
	}
	size_t i = 1;
	while (_size - i != positionIndex)
	{
		data()[_size - i] = data()[_size - i - 1];
		i++;
	}
	data()[_size - i] = element;
	return &(data()[_size - i]);
}

/**
 * @brief Insert element to specific location
 * @param position Location
 * @param element Element to insert
 */
template<class Type, size_t StaticCapacity>
typename VLVector2<Type, StaticCapacity>::iterator
VLVector2<Type, StaticCapacity>::insert(VLVector2::const_iterator position, const Type &element)
{
	size_t positionIndex = position - cbegin();
	_size++;
	if (_size > _capacity)
	{
		_updateHeap();
	}
	size_t i = 1;
	while (_size - i != positionIndex)
	{
		data()[_size - i] = data()[_size - i - 1];
		i++;
	}
	data()[_size - i] = element;
	return &(data()[_size - i]);
}

/**
 * @brief Insert range of elements to specific location
 * @param position Location
 * @param first Iterator that points to the first element in the range
 * @param last Iterator that points to the address right after the last element in the range
 */
template<class Type, size_t StaticCapacity>
template<class InputIterator>
typename VLVector2<Type, StaticCapacity>::iterator
VLVector2<Type, StaticCapacity>::insert(VLVector2::iterator position, InputIterator first,
									   const InputIterator last)
{
	size_t positionIndex = position - begin();
	unsigned int amountToInsert = last - first;
	if (amountToInsert)
	{
		_size += amountToInsert;
		if (_size > _capacity)
		{
			_updateHeap();
		}
		size_t i = 1;
		while (_size - i - amountToInsert + 1 != positionIndex)//making space for the new elements
		{
			data()[_size - i] = data()[_size - i - amountToInsert];
			i++;
		}
		for (size_t j = 0; j < amountToInsert; j++)//inserting the new elements
		{
			data()[_size - i - amountToInsert + 1] = *first;
			first++;
			i--;
		}
		i += amountToInsert;
		return &(data()[_size - i - amountToInsert + 1]);
	}
	return position;
}

/**
 * @brief Insert range of elements to specific location
 * @param position Location
 * @param first Iterator that points to the first element in the range
 * @param last Iterator that points to the address right after the last element in the range
 */
template<class Type, size_t StaticCapacity>
template<class InputIterator>
typename VLVector2<Type, StaticCapacity>::iterator
VLVector2<Type, StaticCapacity>::insert(VLVector2::const_iterator position,
									   InputIterator first, const InputIterator last)
{
	size_t positionIndex = position - cbegin();
	unsigned int amountToInsert = last - first;
	if (amountToInsert)
	{
		_size += amountToInsert;
		if (_size > _capacity)
		{
			_updateHeap();
		}
		size_t i = 1;
		while (_size - i - amountToInsert + 1 != positionIndex)//making space for the new elements
		{
			data()[_size - i] = data()[_size - i - amountToInsert];
			i++;
		}
		for (size_t j = 0; j < amountToInsert; j++)//inserting the new elements
		{
			data()[_size - i - amountToInsert + 1] = *first;
			first++;
			i--;
		}
		i += amountToInsert;
		return &(data()[_size - i - amountToInsert + 1]);
	}
	return position;
}

/**
 * @brief Removes the last element
 */
template<class Type, size_t StaticCapacity>
void VLVector2<Type, StaticCapacity>::pop_back()
{
	_size--;
	if (_size == StaticCapacity)
	{
		_heapToStatic();
	}
}

/**
 * @brief Removes element from a specific location
 * @param position Location
 */
template<class Type, size_t StaticCapacity>
typename VLVector2<Type, StaticCapacity>::iterator
VLVector2<Type, StaticCapacity>::erase(VLVector2::iterator position)
{
	unsigned int dist = position - begin();
	size_t i = 0;
	while (&(data()[dist + i + 1]) != end())
	{
		data()[dist + i] = data()[dist + i + 1];
		i++;
	}
	_size--;
	if (_size == StaticCapacity)
	{
		_heapToStatic();
	}
	return &(data()[dist]);
}

/**
 * @brief Removes element from a specific location
 * @param position Location
 */
template<class Type, size_t StaticCapacity>
typename VLVector2<Type, StaticCapacity>::iterator
VLVector2<Type, StaticCapacity>::erase(VLVector2::const_iterator position)
{
	unsigned int dist = position - cbegin();
	size_t i = 0;
	while (&(data()[dist + i + 1]) != cend())
	{
		data()[dist + i] = data()[dist + i + 1];
		i++;
	}
	_size--;
	if (_size == StaticCapacity)
	{
		_heapToStatic();
	}
	return &(data()[dist]);
}

/**
 * @brief Removes range of elements from a specific location
 * @param first Pointer to the first element
 * @param last Pointer to the address right after the last element
 */
template<class Type, size_t StaticCapacity>
typename VLVector2<Type, StaticCapacity>::iterator
VLVector2<Type, StaticCapacity>::erase(VLVector2::iterator first, VLVector2::iterator last)
{
	unsigned int dist = first - begin();
	unsigned int amountToErase = last - first;
	if (amountToErase)
	{
		size_t i = 0;
		while (&(data()[dist + amountToErase + i]) != end())//moving elements to their new place
		{
			data()[dist + i] = data()[dist + amountToErase + i];
			i++;
		}
		_size -= amountToErase; //removing elements
		if (_size <= StaticCapacity && _size + amountToErase > StaticCapacity)
		{
			_heapToStatic();
		}
	}
	return &(data()[dist]);
}

/**
 * @brief Removes range of elements from a specific location
 * @param first Pointer to the first element
 * @param last Pointer to the address right after the last element
 */
template<class Type, size_t StaticCapacity>
typename VLVector2<Type, StaticCapacity>::iterator
VLVector2<Type, StaticCapacity>::erase(VLVector2::const_iterator first,
									  VLVector2::const_iterator last)
{
	unsigned int dist = first - cbegin();
	unsigned int amountToErase = last - first;
	if (amountToErase)
	{
		size_t i = 0;
		while (&(data()[dist + amountToErase + i]) != cend())//moving elements to their new place
		{
			data()[dist + i] = data()[dist + amountToErase + i];
			i++;
		}
		_size -= amountToErase; //removing elements
		if (_size <= StaticCapacity && _size + amountToErase > StaticCapacity)
		{
			_heapToStatic();
		}
	}
	return &(data()[dist]);

}

/**
 * @brief Clear the vector
 */
template<class Type, size_t StaticCapacity>
void VLVector2<Type, StaticCapacity>::clear()
{
	_size = 0;
	_capacity = StaticCapacity;
	if (_heapArray != nullptr)
	{
		delete[] _heapArray;
	}
}

/**
 * @brief Returns the address of the vector
 */
template<class Type, size_t StaticCapacity>
Type *VLVector2<Type, StaticCapacity>::data() noexcept
{
	if (_heapArray != nullptr)
	{
		return _heapArray;
	}
	return _staticArray;
}

/**
 * @brief Returns the address of the vector
 */
template<class Type, size_t StaticCapacity>
const Type *VLVector2<Type, StaticCapacity>::data() const noexcept
{
	if (_heapArray != nullptr)
	{
		return _heapArray;
	}
	return _staticArray;
}

/**
 * @brief Operator[]
 * @param position Place to reach
 * @return Element in the given place
 */
template<class Type, size_t StaticCapacity>
Type &VLVector2<Type, StaticCapacity>::operator[](const size_t position)
{
	return data()[position];
}

/**
 * @brief Operator[]
 * @param position Place to reach
 * @return Element in the given place
 */
template<class Type, size_t StaticCapacity>
const Type &VLVector2<Type, StaticCapacity>::operator[](const size_t position) const
{
	return data()[position];
}

/**
 * @brief Operator==
 * @param lhs Left vector
 * @param rhs Right vector
 * @return true in case equality, false otherwise
 */
template<class Type, size_t StaticCapacity>
bool operator==(const VLVector2<Type, StaticCapacity> &lhs,
				const VLVector2<Type, StaticCapacity> &rhs)
{
	if (lhs.size() != rhs.size())
	{
		return false;
	}
	for (size_t i = 0; i < lhs.size(); i++)
	{
		if (lhs.data()[i] != rhs.data()[i])
		{
			return false;
		}
	}
	return true;
}

/**
 * @brief Operator!=
 * @param lhs Left vector
 * @param rhs Right vector
 * @return true in case inequality, false otherwise
 */
template<class Type, size_t StaticCapacity>
bool operator!=(const VLVector2<Type, StaticCapacity> &lhs,
				const VLVector2<Type, StaticCapacity> &rhs)
{
	return !(lhs == rhs);
}

#endif //C_VLVECTOR_HPP
