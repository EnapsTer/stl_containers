#pragma once

#include <memory>
#include "../utils//less.hpp"
#include "../utils/pair.hpp"
#include "../utils/pair_compare.hpp"
#include "../utils/utils.hpp"
#include "../utils/map_iterator.hpp"
#include "../utils/iterator_traits.hpp"
#include "red_black_tree.hpp"
#include "node.hpp"

namespace ft{
    template<class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator <ft::pair<const Key, T> > >
    class map{
    public:
        typedef Key 													key_type;
        typedef T 														mapped_type;
        typedef ft::pair<const Key, T> 									pair_type;
        typedef Compare 												key_compare;
        typedef Alloc 													allocator_type;
        typedef typename allocator_type::reference						reference;
        typedef typename allocator_type::const_reference				const_reference;
        typedef typename allocator_type::pointer						pointer;
        typedef typename allocator_type::const_pointer					const_pointer;
        typedef ft::rbt_iterator<pair_type> 							iterator;
        typedef ft::rbt_iterator<const pair_type> 						const_iterator;
        typedef ft::reverse_iterator<iterator> 							reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> 					const_reverse_iterator;
        typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
        typedef difference_type 										size_type;

        typedef typename ft::pair_compare<key_type, mapped_type, key_compare>	value_compare;
        typedef typename Alloc::template rebind<node<pair_type> >::other		node_allocator_type;
        typedef red_black_tree<pair_type, value_compare, node_allocator_type> 	tree_type;
        typedef typename ft::node<pair_type> 									*p_node;

        explicit map(const key_compare &comp = key_compare(), const allocator_type &allocator = allocator_type()){
            allocator_ = allocator;
            node_ = red_black_tree_.create_node(pair_type());
            key_compare_ = comp;
        }

        template<class InputIterator>
        map(InputIterator first, InputIterator last, const key_compare &comp = key_compare(), const allocator_type &allocator = allocator_type()){
            allocator_ = allocator;
            node_ = red_black_tree_.create_node(pair_type());
            key_compare_ = comp;
            insert(first, last);
        }

        ~map(){
            red_black_tree_.clear(&node_->parent);
            red_black_tree_.clear(&node_);
        }

        map(const map &src){
            allocator_ = src.allocator_;
            node_ = red_black_tree_.create_node(pair_type());
            key_compare_ = src.key_compare_;
            *this = src;
        }

        map &operator=(const map &src){
            clear();
            allocator_ = src.allocator_;
            key_compare_ = src.key_compare_;
            value_compare_ = src.value_compare_;
            insert(src.begin(), src.end());
            return *this;
        }

        iterator begin(){
            return iterator(node_, red_black_tree_.min_node(node_->parent));}

        const_iterator begin() const{
            return const_iterator(node_, red_black_tree_.min_node(node_->parent));}

        iterator end(){
            return iterator(node_, 0);}

        const_iterator end() const{
            return const_iterator(node_, 0);}

        reverse_iterator rbegin(){
            return reverse_iterator(end());}

        const_reverse_iterator rbegin() const{
            return const_reverse_iterator(end());}

        reverse_iterator rend(){
            return reverse_iterator(begin());}

        const_reverse_iterator rend() const{
            return const_reverse_iterator(begin());}

        bool empty() const{
            return size_ == 0;}

        size_type size() const{
            return size_;}

        size_type max_size() const{
            return red_black_tree_.max_size();}

        mapped_type &operator[](const key_type &k){
            bool res = red_black_tree_.insert(&node_->parent, red_black_tree_.create_node(add_new_pair(k)));
            size_ += res;
            p_node p = red_black_tree_.find_node(node_->parent, add_new_pair(k));
            return p->value.second;
        }

        ft::pair<iterator, bool> insert(const pair_type &val){
            bool res = red_black_tree_.insert(&node_->parent, red_black_tree_.create_node(val));
            size_ += res;
            p_node ptr = red_black_tree_.find_node(node_->parent, val);
            return ft::pair<iterator, bool>(iterator(node_, ptr), res);
        }

        iterator insert(iterator, const pair_type &val){
            size_ += red_black_tree_.insert(&node_->parent, red_black_tree_.create_node(val));
            p_node p = red_black_tree_.find_node(node_->parent, val);
            return iterator(node_, p);
        }

        template<class InputIterator>
        void insert(InputIterator first, InputIterator last){
            for (; first != last; ++first)
                insert(*first);
        }

        void erase(iterator position){
            bool res = red_black_tree_.erase(&node_->parent, *position);
            if (res)
                --size_;
        }

        size_type erase(const key_type &k){
            bool res = (bool) red_black_tree_.erase(&node_->parent, add_new_pair(k));
            if (res)
                --size_;
            return res;
        }

        void erase(iterator first, iterator last){
            while (first != last)
                erase(first++);
        }

        void swap(map &x){
            ft::swap(x.node_, node_);
            ft::swap(x.key_compare_, key_compare_);
            ft::swap(x.value_compare_, value_compare_);
            ft::swap(x.size_, size_);
        }

        void clear(){
            red_black_tree_.clear(&node_->parent);
            node_->parent = 0;
            size_ = 0;
        }

        key_compare key_comp() const{
            return key_compare_;}

        value_compare value_comp() const{
            return value_compare_;}

        iterator find(const key_type &k){
            p_node ptr = red_black_tree_.find_node(node_->parent, add_new_pair(k));
            return iterator(node_, ptr);
        }

        const_iterator find(const key_type &k) const{
            p_node ptr = red_black_tree_.find_node(node_->parent, add_new_pair(k));
            return const_iterator(node_, ptr);
        }

        size_type count(const key_type &k) const{
            if (red_black_tree_.find_node(node_->parent, add_new_pair(k)))
                return 1;
            return 0;
        }

        iterator lower_bound(const key_type &k){
            return (iterator(node_, red_black_tree_.lowest_elem(node_->parent, add_new_pair(k))));
        }

        const_iterator lower_bound(const key_type &k) const{
            return (const_iterator(node_, red_black_tree_.lowest_elem(node_->parent, add_new_pair(k))));
        }

        iterator upper_bound(const key_type &k){
            p_node temp = red_black_tree_.lowest_elem(node_->parent, add_new_pair(k));
            iterator res(node_, temp);

            if (temp && temp->value.first == k)
                ++res;
            return iterator(res);
        }

        const_iterator upper_bound(const key_type &k) const{
            p_node temp = red_black_tree_.lowest_elem(node_->parent, add_new_pair(k));
            iterator res(node_, temp);

            if (temp && temp->value.first == k)
                ++res;
            return const_iterator(res);
        }

        ft::pair<iterator, iterator> equal_range(const key_type &k){
            return ft::make_pair(lower_bound(k), upper_bound(k));}

        ft::pair<const_iterator, const_iterator>
        equal_range(const key_type &k) const{
            return ft::make_pair(lower_bound(k), upper_bound(k));}

        T& at(const Key &key){
            p_node temp  = red_black_tree_.lowest_elem(node_->parent, add_new_pair(key));
            iterator res(node_, temp);
            if (res == this->end())
                throw std::out_of_range("Key 'map::at' not found.");
            return (res->second);
        }

        const T& at(const Key &key) const{
            p_node temp  = red_black_tree_.lowest_elem(node_->parent, add_new_pair(key));
            iterator res(node_, temp);
            if (res == this->end())
                throw std::out_of_range("Key 'map::at' not found.");
            return (res->second);
        }

        allocator_type get_allocator() const{
            return allocator_;}

    private:
        tree_type 			red_black_tree_;
        allocator_type 		allocator_;
        p_node 				node_;
        key_compare 		key_compare_;
        value_compare 		value_compare_;
        size_type 			size_;

        pair_type add_new_pair(const Key &key) const{
            return ft::make_pair(key, mapped_type());}
    };


    template<class Key, class T, class Compare, class Allocator>
    bool operator==(const map<Key, T, Compare, Allocator> &x,
                    const map<Key, T, Compare, Allocator> &y){
        return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin()) && ft::equal(y.begin(), y.end(), x.begin());
    }

    template<class Key, class T, class Compare, class Allocator>
    bool operator!=(const map<Key, T, Compare, Allocator> &x, const map<Key, T, Compare, Allocator> &y){
        return !(x == y);}

    template<class Key, class T, class Compare, class Allocator>
    bool operator<(const map<Key, T, Compare, Allocator> &x, const map<Key, T, Compare, Allocator> &y){
        return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()) && x != y;
    }

    template<class Key, class T, class Compare, class Allocator>
    bool operator<=(const map<Key, T, Compare, Allocator> &x, const map<Key, T, Compare, Allocator> &y){
        return x < y || x == y;}

    template<class Key, class T, class Compare, class Allocator>
    bool operator>(const map<Key, T, Compare, Allocator> &x, const map<Key, T, Compare, Allocator> &y){
        return y < x;}

    template<class Key, class T, class Compare, class Allocator>
    bool operator>=(const map<Key, T, Compare, Allocator> &x, const map<Key, T, Compare, Allocator> &y){
        return x > y || x == y; }

    template<class Key, class T, class Compare, class Allocator>
    void swap(map<Key, T, Compare, Allocator> &x, map<Key, T, Compare, Allocator> &y){
        x.swap(y);}
}