#ifndef C_VLVECTOR_HPP
#define C_VLVECTOR_HPP


#include <numeric>
#include <functional>
#include <stdexcept>


#define DEFAULT_CAPACITY 16

#define START 0
#define CAPACITY_MULTIPLYER 1.5

#define IDX_ERR_MSG "index error"

// for loops defiens
#define FOR_I_RNG(start, end) for(size_t i=start; i<end; ++i)
#define FOR_I(end) for(size_t i=0; i<end; ++i)
#define FOR_J(end) for(size_t j=0; j<end; ++j)

using std::size_t;

using std::out_of_range;

using std::equal_to;
using std::logical_and;
using std::inner_product;


/**
 * @class VLVector
 * @brief Better version of vector regarding runtime.
 */
template<class T, size_t staticCapacity = DEFAULT_CAPACITY>
class VLVector
{
private:
	size_t _size;
	size_t _capacity;
	T _staticArray[staticCapacity];
	T *_heapArray;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                         memory helpers                            *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void _updateHeap()
	{
		_capacity = (int) (_size * CAPACITY_MULTIPLYER);
		T *array = new T[_capacity]();
		if (_heapArray == nullptr)//was on stack
		{
			FOR_I(StaticCapacity)
			{ array[i] = _staticArray[i]; }
			_heapArray = array;
		}
		else//was on heap
		{
			FOR_I(_size - 1) { array[i] = _heapArray[i]; }
			delete[] _heapArray;
			_heapArray = array;
		}
	}

	void _heapToStatic()
	{
		_capacity = StaticCapacity;
		FOR_I (StaticCapacity) { _staticArray[i] = _heapArray[i]; }
		delete[] _heapArray;
		_heapArray = nullptr;
	}

public:
	typedef T *iterator;
	typedef const T *const_iterator;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                           constructors                            *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	VLVector() : _size(START), _capacity(staticCapacity), _heapArray(nullptr) {}

	template<class InputIterator>
	VLVector(InputIterator first, const InputIterator last)
	{
		while (first != last) // note that the auto for loop
							  // can be cleaner but is slower
		{
			push_back(*first);
			first++;
		}
	}

	VLVector(const VLVector &other) : VLVector(other.begin(), other.end()) {}

	VLVector &operator=(const VLVector &other)
	{
		if (this == &other) { return *this; }
		_size = other._size, _capacity = other._capacity;
		
		// TODO: check
		// maybe this is a code duplication????
		FOR_I(StaticCapacity) { _staticArray[i] = other._staticArray[i]; }
		
		if (_heapArray != nullptr) { delete[] _heapArray; }
		
		_heapArray = new Type[_capacity]();
		for (size_t i = 0; i < _size - 1; i++)
		{
			_heapArray[i] = other._heapArray[i];
		}
		return *this;
	}


	 // brief Destructor
	~VLVector() { if (_heapArray != nullptr) { delete[] _heapArray; } }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                              getters                              *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	size_t size() const noexcept { return _size; }

	size_t capacity() const noexcept { return _capacity; }

	bool empty() const noexcept { return !_size; }

	T &at(const size_t idx) noexcept(false)
	{
		if (idx < START || _size <= idx) { throw out_of_range(IDX_ERR_MSG); }
		return operator[](index);
	}

	const T &at(const size_t index) const noexcept(false)
	{
		if (idx < START || _size <= idx) { throw std::out_of_range("index is out of range"); }
		return operator[](index);
	}
	
	T *data() noexcept { return (_heapArray == nullptr) ? _staticArray : _heapArray; }

	const T *data() const noexcept { return (_heapArray == nullptr) ? _staticArray : _heapArray; }

	iterator begin() noexcept { return data(); }

	iterator end() noexcept { return (data() + _size); }

	const_iterator begin() const noexcept { return data(); }

	const_iterator cbegin() const noexcept { return data(); }
	
	const_iterator end() const noexcept { return (data() + _size); }

	const_iterator cend() const noexcept { return (data() + _size); }

	T &operator[](const size_t position) { return data()[position]; }

	const T &operator[](const size_t position) const { return data()[position]; }
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                           add / delete                            *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	void push_back(const T &element)
	{
		_size++;
		if (_size > capacity()) { _updateHeap(); }
		data()[_size - 1] = element;
	}

	iterator insert(iterator position, const T &element)
	{
		size_t posIdx = position - begin();
		_size++;
		
		if (_size > _capacity) { _updateHeap(); }
		
		size_t i = 1;
		while (_size - i != positionIndex)
		{
			data()[_size - i] = data()[_size - i - 1];
			i++;
		}
		data()[_size - i] = element;
		return &(data()[_size - i]);
	}

	iterator insert(const_iterator position, const T &element)
	{
		size_t positionIndex = position - cbegin();
		_size++;
		
		if (_size > _capacity) { _updateHeap(); }
		
		size_t i = 1;
		while (_size - i != positionIndex)
		{
			data()[_size - i] = data()[_size - i - 1];
			i++;
		}
		data()[_size - i] = element;
		return &(data()[_size - i]);
	}

	template<class InputIterator>
	iterator insert(iterator position, InputIterator first, const InputIterator last)
	{
		size_t positionIndex = position - begin();
		unsigned int amountToInsert = last - first;
		if (amountToInsert)
		{
			_size += amountToInsert;
			if (_size > _capacity) { _updateHeap(); }
			
			size_t i = 1;
			
			while (_size - i - amountToInsert + 1 != positionIndex) // make space for new vals
			{
				data()[_size - i] = data()[_size - i - amountToInsert];
				i++;
			}
			FOR_J(amountToInsert) // insert new vals
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

	template<class InputIterator>
	iterator insert(const_iterator position, InputIterator first, const InputIterator last)
	{
		size_t positionIndex = position - cbegin();
		unsigned int amountToInsert = last - first;
		if (amountToInsert)
		{
			_size += amountToInsert;
			if (_size > _capacity) { _updateHeap(); }
			
			size_t i = 1;
			
			while (_size - i - amountToInsert + 1 != positionIndex) // make space for new vals
			{
				data()[_size - i] = data()[_size - i - amountToInsert];
				i++;
			}
			FOR_J(amountToInsert) // insert new vals
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

	void pop_back()
	{
		_size--;
		if (_size == StaticCapacity) { _heapToStatic(); }
	}

	iterator erase(iterator position)
	{
		unsigned int dist = position - begin();
		size_t i = 0;
		while (&(data()[dist + i + 1]) != end())
		{
			data()[dist + i] = data()[dist + i + 1];
			i++;
		}
		_size--;
		if (_size == StaticCapacity) { _heapToStatic(); }
		return &(data()[dist]);
	}

	iterator erase(const_iterator position)
	{
		unsigned int dist = position - cbegin();
		size_t i = 0;
		while (&(data()[dist + i + 1]) != cend())
		{
			data()[dist + i] = data()[dist + i + 1];
			i++;
		}
		_size--;
		if (_size == StaticCapacity) { _heapToStatic(); }
		return &(data()[dist]);
	}

	iterator erase(iterator first, iterator last)
	{
		unsigned int dist = first - begin(), amountToErase = last - first;
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
			{ _heapToStatic(); }
		}
		return &(data()[dist]);
	}

	iterator erase(const_iterator first, const_iterator last)
	{
		unsigned int dist = first - cbegin(), amountToErase = last - first;
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
			{ _heapToStatic(); }
		}
		return &(data()[dist]);
	}

	void clear()
	{
		_size = 0, _capacity = StaticCapacity;
		if (_heapArray != nullptr) { delete[] _heapArray; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                         boolean operators                         *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	bool operator==(const VLVector &other) const
	{
		return (_size != other._size) ? false : inner_product(begin(), end(), other.begin(), true, logical_and<>(), equal_to<T>());
	}
	
	bool operator!=(const VLVector &other) const { return !(*this == other); }
	
};


#endif //C_VLVECTOR_HPP
