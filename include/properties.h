/*
 * This file is part of Spatosc.
 *
 * Copyright (c) 2010 Alexandre Quessy <alexandre@quessy.net>
 * Copyright (c) 2010 Tristan Matthews <le.businessman@gmail.com>
 * Copyright (c) 2011 Society for Arts and Technologies <info@sat.qc.ca>
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

/**
 * @file
 * The Properties class.
 */

#ifndef __PROPERTIES_H__
#define __PROPERTIES_H__

#include <map>
// TODO: use tr1:unordered_map, since lookup is faster with it
#include <stdexcept>
#include <string>
#include <iostream>

#include "property.h"
#include "memory.h"

namespace spatosc
{

/**
 * Exception thrown when client tries to access property that does not exist.
 */
class NoSuchPropertyError : public std::runtime_error
{
    public:
        NoSuchPropertyError(const char *message) :
            std::runtime_error(message)
        {}
};

/**
 * Holds a set of properties identified by their name.
 */
template <typename T>
class Properties
{
    public:
        typedef std::tr1::shared_ptr< Property<T> > PropertyPtr;

        /**
         * Adds a property.
         */
        Property<T> *addProperty(const std::string &name, const T &value)
        {
            if (hasProperty(name))
            {
                std::cerr << "Warning: property \"" << name << 
                    "\" already exists" << std::endl;
                return 0;
            }
            properties_[name] = PropertyPtr(new Property<T>(name, value));
            return getProperty(name);
        }
        
        /**
         * Removes a property.
         * @return Property was there and got removed. False if it was not present.
         */
        bool removeProperty(const std::string &name)
        {
            if (hasProperty(name))
            {
                properties_.erase(name);
                return true;
            }
            else
            {
                std::cerr << __FUNCTION__ << ": No such property: \"" + name + "\"." << std::endl;
                return false;
            }
        }

        /**
         * Returns a property.
         * @return A Property pointer, or null if not found. Never free this pointer.
         */
        Property<T> *getProperty(const std::string &name) const
        {
            if (properties_.find(name) == properties_.end())
                return 0;
            return properties_.find(name)->second.get();
        }

        /**
         * Sets the value of a property.
         * If there is no such property, it creates it initialized to the 
         * given value.
         */
        void setPropertyValue(const std::string &name, const T &value)
        {
            if (hasProperty(name))
                getProperty(name)->setValue(value);
            else
                addProperty(name, value);
        }

        /**
         * Retrieves the value of a property.
         * If there is no such a property, it returns false.
         */
        bool getPropertyValue(const std::string &name, T &value) const
        {
            if (hasProperty(name))
            {
                value = getProperty(name)->getValue();
                return true;
            }
            else
            {
                std::cerr << __FUNCTION__ << ": No such property: \"" + name + "\"." << std::endl;
                return false;
            }
        }

        /**
         * Checks if a given property is present.
         * @return Found or not.
         */
        bool hasProperty(const std::string &name) const
        {
            return properties_.find(name) != properties_.end();
        }
        
        /**
         * Returns a copy of this map of smart pointers to Property<T>.
         */
        std::map<std::string, PropertyPtr> getProperties() const
        {
            return properties_;
        }

        /**
         * Prints a list of all properties and their values.
         */
        void printProperties() const
        {
            typename std::map<std::string, PropertyPtr>::const_iterator iter;
            for (iter = properties_.begin(); iter != properties_.end(); ++iter)
                std::cout << " * " << iter->second->getName() << " = " << iter->second->getValue() << std::endl;
        }

    private:
        std::map<std::string, PropertyPtr> properties_;
};

} // end namespace spatosc

#endif

