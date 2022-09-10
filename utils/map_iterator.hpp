#pragma once

#include "iterator_traits.hpp"
#include "pair.hpp"
#include "utils.hpp"
#include "switch_const.hpp"
#include "../map/node.hpp"

namespace ft
{
    template<typename T>
    class rbt_iterator{
    public:
        typedef typename std::iterator<std::bidirectional_iterator_tag, T> rbt_it;
        typedef typename rbt_it::iterator_category iterator_category;
        typedef typename rbt_it::value_type value_type;
        typedef typename rbt_it::difference_type difference_type;
        typedef T *pointer;
        typedef T &reference;
        typedef ft::node<typename ft::switch_const<T>::type> *p_node;

        rbt_iterator() : root_(nullptr), node_(nullptr){}

        explicit rbt_iterator(const p_node &root, const p_node &node) : root_(root), node_(node){}

        rbt_iterator(const rbt_iterator &src) {
            *this = src;
        }

        ~rbt_iterator() {}

        rbt_iterator &operator=(const rbt_iterator &src) {
            if (this == &src)
                return *this;
            node_ = src.node_;
            root_ = src.root_;
            return *this;
        }

        reference &operator*() {
            return node_->value;
        }

        pointer operator->() const {
            return &(node_->value);
        }

        rbt_iterator &operator++() {
            if (!node_)
                return *this;
            else if (node_->right)
                node_ = minimal_node(node_->right);
            else if (node_ == maximal_node(root_->parent))
                node_ = 0;
            else{
                while (node_->parent->right == node_)
                    node_ = node_->parent;
                node_ = node_->parent;
            }
            return *this;
        }

        rbt_iterator operator++(int) {
            rbt_iterator temp = *this;
            ++(*this);
            return temp;
        }

        rbt_iterator &operator--() {
            if (!node_)
                node_ = maximal_node(root_->parent);
            else if (node_->left)
                node_ = maximal_node(node_->left);
            else{
                while (node_->parent->left == node_)
                    node_ = node_->parent;
                node_ = node_->parent;
            }
            return *this;
        }

        rbt_iterator operator--(int) {
            rbt_iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const rbt_iterator &it) const {
            return (it.node_ == node_);
        }

        bool operator!=(const rbt_iterator &it) const {
            return (it.node_ != node_);
        }

        operator rbt_iterator<const T>() const {
            return rbt_iterator<const T>(root_, node_);
        }

    private:
        p_node root_;
        p_node node_;

        p_node minimal_node(p_node node_) const {
            if (node_) {
                while (node_->left)
                    node_ = node_->left;
            }
            return node_;
        }

        p_node maximal_node(p_node node_) const {
            if (node_) {
                while (node_->right)
                    node_ = node_->right;
            }
            return node_;
        }
    };
}