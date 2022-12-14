#pragma once
#include <iterator>
#include <iostream>
#include "utils.hpp"
#include "is_integral.hpp"

namespace ft{
template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T &>
struct iterators{
	typedef Distance difference_type;
	typedef T value_type;
	typedef Pointer pointer;
	typedef Reference reference;
	typedef Category iterator_category;
	
};

template <class Iterator>
struct iterator_traits{
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	typedef typename Iterator::iterator_category iterator_category;
};

template <class T>
struct iterator_traits<T*>{
	typedef ptrdiff_t difference;
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef std::random_access_iterator_tag iterator_category;
};

template <class T>
struct iterator_traits<const T*>{
	typedef ptrdiff_t difference;
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef std::random_access_iterator_tag iterator_category;
};

template<class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	size_t distance = 0;
	for(; first!=last; ++first)
		++distance;
	return distance;
}

template <class Iterator>
class reverse_iterator{
	public:
		typedef Iterator iterator_type;
		typedef typename ft::iterator_traits<iterator_type>::value_type value_type;
		typedef typename ft::iterator_traits<iterator_type>::difference_type difference_type;
		typedef typename ft::iterator_traits<iterator_type>::pointer pointer;
		typedef typename ft::iterator_traits<iterator_type>::reference reference;
		typedef typename ft::iterator_traits<iterator_type>::iterator_category iterator_category;

		reverse_iterator() {}
		explicit reverse_iterator(iterator_type it) : position_(it){}
		reverse_iterator(const reverse_iterator &it) : position_(it.base()){}
		~reverse_iterator() {}

		iterator_type base() const {
			return position_;
        }

		reference operator*() const {
			iterator_type tmp = position_;
			return *(--tmp);
		}

		reverse_iterator operator+(difference_type n) const {
			return reverse_iterator(position_ - n);
		}
		
		reverse_iterator &operator++() {
			position_--;
			return *this;
		}

		reverse_iterator operator++(int) {
			reverse_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		reverse_iterator& operator+= (difference_type n) {
            position_ -= n;
			return *this;
		}

		reverse_iterator &operator--() {
			position_++;
			return *this;
		}

		reverse_iterator &operator-=(difference_type n) {
            position_ += n;
			return *this;
		}

		reverse_iterator operator-(difference_type n) const {
			return reverse_iterator(position_ + n);
		}

		reverse_iterator operator--(int) {
			reverse_iterator tmp = *this;
			--(*this);
			return tmp;
		}

		pointer operator->() const {
			return &(operator*());
		}

		reference operator[](difference_type n) const {
			return *(operator-(n));
		}
		private:
			Iterator position_;
	};

	template <class Iterator>
	bool operator==(const reverse_iterator<Iterator>&lhs, const reverse_iterator<Iterator>&rhs){
		return lhs.base()==rhs.base();
	}

	template <class Iterator>
	bool operator!=(const reverse_iterator<Iterator>&lhs, const reverse_iterator<Iterator>&rhs){
		return lhs.base()!=rhs.base();
	}

	template <class Iterator>
	bool operator<(const reverse_iterator<Iterator>&lhs, const reverse_iterator<Iterator>&rhs){
		return lhs.base()>rhs.base();
	}

	template <class Iterator>
	bool operator<=(const reverse_iterator<Iterator>&lhs, const reverse_iterator<Iterator>&rhs){
		return lhs.base()>=rhs.base();
	}

	template <class Iterator>
	bool operator>(const reverse_iterator<Iterator>&lhs, const reverse_iterator<Iterator>&rhs){
		return lhs.base()<rhs.base();
	}

	template <class Iterator>
	bool operator>=(const reverse_iterator<Iterator>&lhs, const reverse_iterator<Iterator>&rhs){
		return lhs.base()<=rhs.base();
	}

	template <class Iterator>
	reverse_iterator<Iterator>operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator> &rev_it){
			return rev_it + n;
	}

	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator> &lhs, const reverse_iterator<Iterator>&rhs){
		return lhs.base() - rhs.base();
	}

    template <typename T>
	class random_access_iterator : ft::iterators<std::random_access_iterator_tag, T>
	{
	public:
		typedef T* iterator_type;
		typedef typename ft::iterators<std::random_access_iterator_tag, T>::iterator_category iterator_category;
		typedef typename ft::iterators<std::random_access_iterator_tag, T>::value_type value_type;
		typedef typename ft::iterators<std::random_access_iterator_tag, T>::difference_type difference_type;
		typedef typename ft::iterators<std::random_access_iterator_tag, T>::pointer pointer;
		typedef typename ft::iterators<std::random_access_iterator_tag, T>::reference reference;

		random_access_iterator() {}
		random_access_iterator(pointer position): position_(position) {}
		~random_access_iterator() {}

		random_access_iterator(const random_access_iterator &src) {
            *this = src;
        }

		pointer base() const {return position_;}

		random_access_iterator &operator=(const random_access_iterator &src) {
			if (this == &src)
				return (*this);
            position_ = src.base();
			return *this;
		}

		reference &operator*() {
            return (*position_);
        }

		pointer operator->() const {
            return &(operator*());
        }

		random_access_iterator operator+(difference_type n) const {
			return position_ + n;
		}

		random_access_iterator &operator++() {
			position_++;
			return *this;
		}

		random_access_iterator operator++(int) {
			random_access_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		random_access_iterator &operator+=(difference_type n) {
            position_ += n;
			return *this;
		}

		random_access_iterator operator-(difference_type n) const {
			return position_ - n;
		}

		random_access_iterator &operator--() {
			position_--;
			return (*this);
		}

		random_access_iterator operator--(int) {
			random_access_iterator tmp = *this;
			--(*this);
			return tmp;
		}

		random_access_iterator &operator-=(difference_type n) {
            position_ -= n;
			return *this;
		}

		reference operator[](difference_type n) const {
            return *(operator+(n));
        }

		operator random_access_iterator<const T>(void) const {
            return (random_access_iterator<const T>(this->position_));
        }

	private:
		pointer position_;
	};

	template<class Iterator>
	bool operator!=(const random_access_iterator<Iterator> &lhs, const random_access_iterator<Iterator> &rhs) {
		return lhs.base() != rhs.base();
	}

	template<class Iterator>
	bool operator==(const random_access_iterator<Iterator> &lhs, const random_access_iterator<Iterator> &rhs) {
		return lhs.base() == rhs.base();
	}

	template<class Iterator>
	bool operator>=(const random_access_iterator<Iterator> &lhs, const random_access_iterator<Iterator> &rhs) {
		return (lhs.base() >= rhs.base());
	}

	template<class Iterator>
	bool operator<=(const random_access_iterator<Iterator> &lhs, const random_access_iterator<Iterator> &rhs) {
		return (lhs.base() <= rhs.base());
	}

	template<class Iterator>
	bool operator>(const random_access_iterator<Iterator> &lhs, const random_access_iterator<Iterator> &rhs) {
		return (lhs.base() > rhs.base());
	}

	template<class Iterator>
	bool operator<(const random_access_iterator<Iterator> &lhs, const random_access_iterator<Iterator> &rhs) {
		return (lhs.base() < rhs.base());
	}

	template<class Iterator>
	random_access_iterator<Iterator> operator+(
			typename random_access_iterator<Iterator>::difference_type n,
			const random_access_iterator<Iterator> &it) {
		return (it + n);
	}

	template<class Iterator>
	typename random_access_iterator<Iterator>::difference_type operator-(
			const random_access_iterator<Iterator> &lhs,
			const random_access_iterator<Iterator> &rhs) {
		return (lhs.base() - rhs.base());
	}
}