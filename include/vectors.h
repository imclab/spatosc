/*
 * This file is part of Spatosc.
 *
 * Copyright (c) 2010 Society for Arts and Technologies <info@sat.qc.ca>
 *
 * Spatosc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Spatosc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Spatosc.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 * Helper functions for containers
 */
#ifndef __VECTORS_H__
#define __VECTORS_H__

#include <vector>
#include <algorithm>
#include "memory.h"

namespace spatosc {
    // useful to search and delete in a vector of shared_ptr
    template <typename T>
    class IsEqual
    {
        public:
            IsEqual(T *a) : a_(a) {}
            bool operator() (const std::tr1::shared_ptr<T> &b)
            {
                return a_ == b.get();
            }
        private:
            const T *a_;
    };
    /**
     * Removes an element from a vector of shared pointers if
     * the given pointer matches.
     * @return Whether it deleted some elements of not.
     */
    template <typename T>
    bool eraseFromVector(std::vector<std::tr1::shared_ptr<T> >& vec, T *a)
    {
        size_t size_before = vec.size();
        IsEqual<T> predicate(a);
        vec.erase(std::remove_if(vec.begin(), vec.end(), predicate), vec.end());
        return vec.size() < size_before;
    }
} // end namespace spatosc

#endif // __VECTORS_H__
