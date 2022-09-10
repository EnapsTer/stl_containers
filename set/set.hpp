#pragma once

#include <memory>
#include "../utils//less.hpp"
#include "../utils/pair.hpp"
#include "../utils/pair_compare.hpp"
#include "../utils/utils.hpp"
#include "../utils/map_iterator.hpp"
#include "../utils/iterator_traits.hpp"
#include "../map/red_black_tree.hpp"
#include "../map/node.hpp"

namespace ft
{
    template <class Key, class Compare = ft::less<Key>, class Alloc = std::allocator<Key> >
    class set{
    public:
        typedef Key key_type;
        typedef key_type value_type;
        typedef Compare key_compare;
        typedef key_compare value_compare;
        typedef Alloc allocator_type;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::pointer pointer;
        typedef ft::rbt_iterator<const value_type> const_iterator;
        typedef ft::rbt_iterator<value_type> iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef typename ft::iterator_traits<iterator>::difference_type difference_type;
        typedef difference_type size_type;

        typedef typename ft::node<value_type> *p_node;
        typedef typename Alloc::template rebind<node<value_type> >::other node_allocator_type;
        typedef red_black_tree<value_type, value_compare, node_allocator_type> tree_type;

    public:
        explicit set(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type()) {
            alloc_ = alloc;
            node_ = rb_tree_.create_node(value_type());
            cmpr_ = comp;
            sz_ = 0;
        }

        template <class TemplateIterator>
        set(TemplateIterator first, TemplateIterator last,
                const key_compare &comp = key_compare(),
                const allocator_type &alloc = allocator_type()) {
            alloc_ = alloc;
            node_ = rb_tree_.create_node(value_type());
            cmpr_ = comp;
            sz_ = 0;
            insert(first, last);
        }

        ~set() {
            rb_tree_.clear(&node_->parent);
            rb_tree_.clear(&node_);
        }

        set(const set &src) {
            alloc_ = src.alloc_;
            node_ = rb_tree_.create_node(value_type());
            cmpr_ = src.cmpr_;
            sz_ = src.sz_;
            *this = src;
        }

        set &operator=(const set &src) {
            clear();
            cmpr_ = src.cmpr_;
            alloc_ = src.alloc_;
            insert(src.begin(), src.end());

            return *this;
        }

        iterator begin() {
            return iterator(node_, rb_tree_.min_node(node_->parent));
        }

        const_iterator begin() const {
            return const_iterator(node_, rb_tree_.min_node(node_->parent));
        }

        iterator end() {
            return iterator(node_, nullptr);
        }

        const_iterator end() const {
            return const_iterator(node_, nullptr);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

        bool empty() const {
            return sz_ == 0;
        }

        size_type size() const {
            return sz_;
        }

        size_type max_size() const {
            return rb_tree_.max_size();
        }

        ft::pair<iterator, bool> insert(const value_type &val) {
            bool res = rb_tree_.insert(&node_->parent, rb_tree_.create_node(val));
            sz_ += res;
            p_node ptr = rb_tree_.find_node(node_->parent, val);

            return ft::pair<iterator, bool>(iterator(node_, ptr), res);
        }

        template <class TemplateIterator>
        void insert(TemplateIterator first, TemplateIterator last){
            for (; first != last; ++first)
                insert(*first);
        }

        iterator insert(iterator, const value_type &val){
            sz_ += rb_tree_.insert(&node_->parent, rb_tree_.create_node(val));
            p_node ptr = rb_tree_.find_node(node_->parent, val);
            return iterator(node_, ptr);
        }

        void erase(iterator first, iterator last){
            while(first != last)
                erase(first++);
        }

        void erase(iterator position){
            bool res = rb_tree_.erase(&node_->parent, *position);
            if (res)
                --sz_;
        }

        size_type erase(const key_type &k){
            bool res = (bool)rb_tree_.erase(&node_->parent, k);
            if (res)
                --sz_;
            return res;
        }

        void swap(set &x){
            ft::swap(x.alloc_, alloc_);
            ft::swap(x.sz_, sz_);
            ft::swap(x.cmpr_, cmpr_);
            ft::swap(x.node_, node_);
        }

        void clear(){
            rb_tree_.clear(&node_->parent);
            node_->parent = 0;
            sz_ = 0;
        }

        key_compare key_comp() const {
            return cmpr_;
        }

        value_compare value_comp() const {
            return cmpr_;
        }

        iterator find(const key_type &k) {
            p_node ptr = rb_tree_.find_node(node_->parent, k);
            return iterator(node_, ptr);
        }

        const_iterator find(const key_type &k) const {
            p_node ptr = rb_tree_.find_node(node_->parent, k);
            return const_iterator(node_, ptr);
        }

        size_type count(const key_type &k) const {
            if (rb_tree_.find_node(node_->parent, k))
                return 1;
            return 0;
        }

        iterator lower_bound(const key_type &k) {
            return iterator(node_, rb_tree_.lowest_elem(node_->parent, k));
        }

        const_iterator lower_bound(const key_type &k) const {
            return const_iterator(node_, rb_tree_.lowest_elem(node_->parent, k));
        }

        const_iterator upper_bound(const key_type &k) const{
            p_node tmp = rb_tree_.lowest_elem(node_->parent, k);
            iterator res(node_, tmp);
            if (tmp && tmp->value == k)
                ++res;
            return const_iterator(res);
        }

        iterator upper_bound(const key_type &k) {
            p_node tmp = rb_tree_.lowest_elem(node_->parent, k);
            iterator res(node_, tmp);

            if (tmp && tmp->value == k)
                ++res;
            return iterator(res);
        }

        ft::pair<iterator, iterator> equal_range(const key_type &k){
            return ft::make_pair(lower_bound(k), upper_bound(k));
        }

        ft::pair<const_iterator, const_iterator> equal_range(const key_type &k) const{
            return ft::make_pair(lower_bound(k), upper_bound(k));
        }

        allocator_type get_allocator() const{
            return alloc_;
        }

    private:
        tree_type		rb_tree_;
        allocator_type	alloc_;
        p_node			node_;
        key_compare		cmpr_;
        size_type		sz_;
    };

    template <class Key, class Compare, class Allocator>
    bool operator==(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return x.size() == y.size()
               && ft::equal(x.begin(), x.end(), y.begin())
               && ft::equal(y.begin(), y.end(), x.begin());
    }

    template <class Key, class Compare, class Allocator>
    bool operator!=(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return !(x == y);
    }

    template <class Key, class Compare, class Allocator>
    bool operator<(const set<Key, Compare, Allocator> &x,const set<Key, Compare, Allocator> &y){
        return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end())
               && x != y;
    }

    template <class Key, class Compare, class Allocator>
    bool operator<=(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return x < y || x == y; }

    template <class Key, class Compare, class Allocator>
    bool operator>(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return y < x;
    }

    template <class Key, class Compare, class Allocator>
    bool operator>=(const set<Key, Compare, Allocator> &x, const set<Key, Compare, Allocator> &y){
        return x > y || x == y;
    }

    template <class Key, class Compare, class Allocator>
    void swap(set<Key, Compare, Allocator> &x, set<Key, Compare, Allocator> &y){
        x.swap(y);
    }
}