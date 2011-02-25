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
 * The Property class.
 */

/** The Property template class holds a single value. */

#ifndef __PROPERTY_H__
#define __PROPERTY_H__

#include <string>
#include <functional>

/**
 * Property value
 *
 * Can hold a value of a type such as int, float, etc.
 */
template <typename T>
class Property
{
    public:
        /**
         * Constructor with name and value as arguments.
         */
        Property(const std::string &name, T value) : 
            name_(name),
            value_(value)
        {}
        /**
         * Returns the current value of this property.
         */
        T get_value() const
        {
            return value_;
        }
        /**
         * Returns the name of this property.
         */
        const std::string &get_name() const
        {
            return name_;
        }

        void set_value(T value)
        {
            value_ = value;
            //TODO: trigger signal / callback
            //value_changed_signal_(name_, value_);
        }
    private:
        std::string name_;
        T value_;
};

#endif
