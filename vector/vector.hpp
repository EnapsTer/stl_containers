#pragma once

# include <memory>
# include <algorithm>
# include <cstddef>
# include <tgmath.h>
#include <iostream>
#include "../utils/iterator_traits.hpp"
#include "../utils/is_integral.hpp"
#include "../utils/enable_if.hpp"
#include "../utils/utils.hpp"
namespace ft{
    template <typename T, typename Alloc = std::allocator<T> >
    class vector{
        public:
            typedef T value_type;
            typedef Alloc allocator_type;
            typedef typename allocator_type::reference reference;
            typedef typename allocator_type::const_reference const_reference;
            typedef typename allocator_type::const_pointer const_pointer;
            typedef typename allocator_type::pointer pointer;
            typedef ft::random_access_iterator<value_type> iterator;
            typedef ft::random_access_iterator<const value_type> const_iterator;
            typedef ft::reverse_iterator<iterator> reverse_iterator;
            typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
            typedef typename ft::iterator_traits<iterator>::difference_type difference_type;
            typedef typename allocator_type::size_type size_type;

			explicit vector(const allocator_type &allocator = allocator_type()) {
                allocator_ = allocator;
                data_ = nullptr;
                capacity_ = 0;
                size_ = 0;
			}

			explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &allocator = allocator_type()) {
				if (n < 0)
					throw std::bad_alloc();
				else	
					{
                        allocator_ = allocator;
                        capacity_ = n;
                        size_ = n;

						if (!n)
                            data_ = nullptr;
						else {
                            data_ = allocator_.allocate(n);
							for (size_type i = 0; i < n; ++i)
                                allocator_.construct(data_ + i, val);
						}
					}
			}

			template<class TemplateIterator>
			vector(TemplateIterator first, TemplateIterator last, const allocator_type &allocator = allocator_type(),
                   typename std::enable_if <!ft::is_integral<TemplateIterator>::value>::type* = nullptr) {
                size_ = ft::distance(first, last);
                allocator_ = allocator;
                capacity_ = size_;
                data_ = allocator_.allocate(capacity_);
                value_type *tempData = data_;

                for (TemplateIterator it = first; it != last; ++tempData, ++it)
                    allocator_.construct(tempData, *it);
			}
			
			vector(const vector &x) {
                size_ = x.size_;
                allocator_ = x.allocator_;
                capacity_ = x.capacity_;
                data_ = allocator_.allocate(capacity_);

				for (size_type i = 0; i < size_; ++i)
					allocator_.construct(data_ + i, x[i]);
			}

			~vector() {
				for (size_type i = 0; i < size_; ++i)
					allocator_.destroy(data_ + i);
				allocator_.deallocate(data_, capacity_);
			}

			vector &operator=(const vector &x) {
                if (&x != this) {
                    size_ = x.size_;
                    allocator_.deallocate(data_, capacity_);
                    capacity_ = x.capacity_;
                    data_ = allocator_.allocate(capacity_);

                     for (size_type i = 0; i < size_; ++i)
                         allocator_.construct(data_ + i, x[i]);
                }
				return *this;
			}

			iterator begin() {
				return &data_[0];
			}
			
			const_iterator begin() const {
				return &data_[0];
			}

			iterator end() {
				return &data_[size_];
			}

			const_iterator end() const {
				return &data_[size_];
			}

			reverse_iterator rbegin() {
				return reverse_iterator(end());
			}

			const_reverse_iterator rbegin() const {
				return reverse_iterator(end());
			}

			reverse_iterator rend() {
				return reverse_iterator(begin());
			}

			const_reverse_iterator rend() const {
				return reverse_iterator(begin());
			}

			size_type size() const {
				return size_;
			}

			size_type max_size() const {
				return allocator_.max_size();
			}

			void resize(size_type n, value_type val = value_type()) {
				if (n < size_) {
                    for (; size_ > n; --size_)
                        allocator_.destroy(&data_[size_]);
                }
				else if (n > size_ && n < max_size()) {
					if (n > capacity_ && n <= capacity_ * 2)
						reserve(capacity_ * 2);
					else if (n > capacity_ * 2)
						reserve(n);
					for(; size_ < n; ++size_)
						allocator_.construct(&data_[size_], val);
				}
			}

			void reserve(size_type n) {
				if (n > capacity_ && n < max_size()) {
					pointer temp = allocator_.allocate(n);
					try {
						std::uninitialized_copy(data_, data_ + size_, temp);
					}
					catch(...) {
						allocator_.deallocate(temp, n);
						throw;
					}
					allocator_.deallocate(data_, capacity_);
                    data_ = temp;
                    capacity_ = n;
				}
			}

			bool empty() const{
				return !size_;
			}

			size_type capacity() const{
				return capacity_;
			}

			const_reference operator[](size_type n) const {
				return data_[n];
			}

			reference operator[](size_type n) {
				return data_[n];
			}

			const_reference at(size_type n) const {
				if (n < size_)
					return data_[n];
				throw std::out_of_range("Error: position_ out of range");
			}

			reference at(size_type n) {
				if (n < size_)
					return data_[n];
				throw std::out_of_range("Error: position_ out of range");
			}

			const_reference front() const {
				return data_[0];
			}

			reference front() {
				return data_[0];
			}

			const_reference back() const {
				return data_[size_ - 1];
			}

			reference back() {
				return data_[size_ - 1];
			}


			template<class TemplateIterator>
			void assign (TemplateIterator first, TemplateIterator last, typename ft::enable_if<!ft::is_integral<TemplateIterator>::value>::type* = nullptr) {
				clear();
				size_type distance = ft::distance(first, last);

				if (capacity_ < distance) {
					if(capacity_ * 2 >= distance)
						reserve(capacity_ * 2);
					else
						reserve(distance);
				}
				for(; first < last; ++size_, ++first)
					allocator_.construct(data_ + size_, *first);
			}

			void assign (size_type n, const value_type &val) {
				clear();
				if (capacity_ < n) {
					if (capacity_ < n) {
						if (capacity_ * 2 >= n)
							reserve(capacity_ * 2);
						else
							reserve(n);
					}
					for(; size_ < n; ++size_)
						allocator_.construct(data_ + size_, val);
				}
			}

			void push_back(const value_type &val) {
				if (!size_)
					reserve(1);
				else if(size_ == capacity_)
					reserve(capacity_ * 2);
				allocator_.construct(&(*end()), val);
				++size_;
			}

			void pop_back() {
				if (size_) {
					allocator_.destroy(&(*(end() - 1)));
					--size_;
				}
			}

			iterator insert(iterator position, const value_type &val) {
				size_type dist = ft::distance(begin(), position);
				if (size_ < capacity_) {
					for (iterator it = end(); it != position - 1; --it)
						allocator_.construct(&(*position), val);
					allocator_.construct(&(*position), val);
					++size_;
				}
				else{
					value_type *temp_data = allocator_.allocate(capacity_ ? capacity_ *=2 : 1);
					size_type i = 0;
					for (iterator it = begin(); it != end(); ++it) {
						if (it == position) {
							allocator_.construct(temp_data + i, val);
							++i;
						}
						allocator_.construct(temp_data + i, *it);
						allocator_.destroy(&(*it));
						++i;
					}
					allocator_.deallocate(data_, capacity_);
                    data_ = temp_data;
					++size_;
				}
				return (begin() + dist);
			}

			void insert(iterator position, size_type n, const value_type &val) {
				if (size_ + n > capacity_) {
					if (size_ + n >= capacity_ * 2)
                        capacity_ = size_ + n;
					else
                        capacity_ *= 2;
				}
				value_type *temp_data = allocator_.allocate(capacity_);
				value_type *p = temp_data;
				for (int i = 0; begin() + i < position; ++i) {
					allocator_.construct(temp_data, *(begin() + i));
					++temp_data;
					allocator_.destroy(&(*(begin() + i)));
				}
				for (size_type i = 0; i < n; ++i) {
					allocator_.construct(temp_data, val);
					++temp_data;
				}
				for (int i = 0; position + i < end(); ++i) {
					allocator_.construct(temp_data, *(position + i));
					++temp_data;
					allocator_.destroy(&(*(position + i)));
				}
				allocator_.deallocate(&(*begin()), size_);
                data_ = p;
                size_ += n;
			}

			template<class TemplateIterator>
			void insert (iterator position, TemplateIterator first, TemplateIterator last, typename ft::enable_if<!ft::is_integral<TemplateIterator>::value>::type* = 0) {
				size_type n = static_cast<size_type>(ft::distance(first, last));
				size_type distance = static_cast<size_type>(ft::distance(begin(), position));
				if (position > end() || position < begin())
					throw std::range_error("Index Error");
				value_type *temp_data = allocator_.allocate(n);
				try{
					for (size_type i = 0; i < n; ++i)
						allocator_.construct(temp_data + i, *first++);
				}
				catch(...) {
					for (size_type i = 0; temp_data + i != 0 && i < n; ++i)
						allocator_.destroy(temp_data + i);
					allocator_.deallocate(temp_data, n);
					throw std::runtime_error("Error");
				}
				if (capacity_ == size_ && capacity_)
					reserve(capacity_ * 2);
				if (capacity_ < size_ + n)
					reserve(size_ + n);
				for (size_type i = 0; size_ - i != distance; ++i) {
					allocator_.construct(data_ + size_ - 1 - i + n,
                                         data_[size_ - i - 1]);
					allocator_.destroy(data_ + size_ - i - 1);
				}
				for (size_type i = 0; i < n; ++i) {
					allocator_.construct(data_ + distance + i, temp_data[i]);
					allocator_.destroy(temp_data + i);
					++size_;
				}
				allocator_.deallocate(temp_data, n);
			}

			iterator erase(iterator first, iterator last) {
				iterator temp = first;
				size_type distance = ft::distance(first, last);

				for (; first < end() - distance; ++first) {
					allocator_.destroy(&(*first));
					allocator_.construct(&(*first), *(first + distance));
					allocator_.destroy(&(*(first + distance)));
				}
                size_ -= distance;
				return temp;
			}

			iterator erase(iterator position) {
				if (position + 1 == end())
					allocator_.destroy(&(*position));
				else{
					for (int i = 0; position + i + 1 < end(); ++i) {
						allocator_.construct(&(*(position + i)), *(position + i + 1));
						allocator_.destroy(&(*(position + i + 1)));
					}
                    size_ -= 1;
					return position;
				}
			}

			void clear() {
				size_type i = 0;
				while(i != size_) {
					allocator_.destroy(data_ + i);
					++i;
				}
                size_ = 0;
			}
				
			allocator_type get_allocator()const{
				return allocator_;
			}

			void swap(vector &x) {
				ft::swap(data_, x.data_);
				ft::swap(capacity_, x.capacity_);
				ft::swap(size_, x.size_);
			}

		private:
			allocator_type allocator_;
			value_type *data_;
			size_type capacity_;
			size_type size_;
	};

	template<class T, class Alloc>
	bool operator==(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc>&rhs) {
		if (lhs.size() != rhs.size())
			return false;
		if(ft::equal(lhs.begin(), lhs.end(), rhs.begin()))
			return true;
		return false;}

	template<class T, class Alloc>
	bool operator!=(const ft::vector<T, Alloc>&lhs, const ft::vector<T, Alloc> &rhs) {
		return !(lhs == rhs);}

	template <class T, class Alloc>
	bool operator<(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) {
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());}

	template <class T, class Alloc>
	bool operator>(const ft::vector <T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) {
		return (rhs < lhs);
	}

	template <class T, class Alloc>
	bool operator<=(const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) {
		return (!(rhs < lhs));
	}

	template<class T, class Alloc>
	bool operator>=(const ft::vector <T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) {
		return (!(lhs < rhs));
	}

	template<class T, class Alloc>
	void swap(ft::vector<T, Alloc>&x, ft::vector<T, Alloc>&y) {
		x.swap(y);
	}
}
