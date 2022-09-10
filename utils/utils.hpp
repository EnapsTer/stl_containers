#pragma once
#include "is_integral.hpp"
#include "enable_if.hpp"
#include <iostream>
namespace ft {
    template<class TemplateIterator1, class TemplateIterator2>
    bool lexicographical_compare(TemplateIterator1 first1, TemplateIterator1 last1, TemplateIterator2 first2,
                                 TemplateIterator2 last2) {
        while (first1 != last1) {
            if (first2 == last2 || *first2 < *first1)
                return false;
            else if (*first1 < *first2)
                return true;
            ++first1;
            ++first2;
        }
        return (first2 != last2);
    }

    template<class TemplateIterator1, class TemplateIterator2, class Compare>
    bool lexicographical_compare(TemplateIterator1 first1, TemplateIterator1 last1, TemplateIterator2 first2,
                                 TemplateIterator2 last2, Compare comp) {
        while (first1 != first2) {
            if (first2 == last2 || comp(*first2 < *first1))
                return false;
            else if (*first1 < *first2)
                return true;
            ++first1;
            ++first2;
        }
        return (first2 != last2);
    }

    template<class TemplateIterator1, class TemplateIterator2>
    bool equal(TemplateIterator1 first1, TemplateIterator1 last1, TemplateIterator2 first2) {
        for (; first1 != last1; first1++, first2++)
            if (*first1 != *first2)
                return false;
        return true;
    }

    template<class TemplateIterator1, class TemplateIterator2, class BinaryPredicate>
    bool equal(TemplateIterator1 first1, TemplateIterator1 last1, TemplateIterator2 first2, BinaryPredicate predicate) {
        while (first1 != last1) {
            if (!predicate(*first1, *first2))
                return false;
            ++first1;
            ++first2;
        }
        return true;
    }
}