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
        Property<T> *addProperty(const std::string &name, T value)
        {
            if (hasProperty(name))
            {
                std::cout << "Warning: property \"" << name << 
                    "\" already exists" << std::endl;
                return 0;
            }
            properties_[name] = PropertyPtr(new Property<T>(name, value));
            return getProperty(name);
        }
        
        /**
         * Removes a property.
         */
        void removeProperty(const std::string &name)
        {
            properties_.erase(name);
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
         */
        bool setPropertyValue(const std::string &name, T value)
        {
            if (not hasProperty(name))
            {
                std::cout << "No such property \"" << name << "\"" << std::endl;
                return false;
            }
            else
                properties_[name]->setValue(value);
            return true;
        }

        /**
         * Returns the value of a property.
         * 
         * You should always call hasProperty before calling this method in order to avoid having to catch the exception it may throw if the given property does not exist.
         * @throw NoSuchPropertyError if the given property does not exist.
         */
        T getPropertyValue(const std::string &name) throw (NoSuchPropertyError)
        {
            if (not hasProperty(name))
            {
                std::string message = "No such property: \"" + name + "\".";
                std::cout << message << std::endl;
                throw NoSuchPropertyError(message.c_str());
            }
                
            return properties_[name]->getValue();
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
        std::map<std::string, PropertyPtr> getProperties()
        {
            return properties_;
        }

        /**
         * Prints a list of all properties and their values.
         */
        void printProperties()
        {
            typename std::map<std::string, PropertyPtr>::iterator iter;
            for (iter = properties_.begin(); iter != properties_.end(); iter++)
                std::cout << " * " << iter->second->getName() << " = " << iter->second->getValue() << std::endl;
        }

    private:
        std::map<std::string, PropertyPtr> properties_;
};

} // end namespace spatosc

#endif

