/* 
 * spatdif_translator.h
 *
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
 * The Spatdif translator class.
 */

#ifndef _SPATDIF_TRANSLATOR_H_
#define _SPATDIF_TRANSLATOR_H_

#include "translator.h"
#include <lo/lo.h>
#include <string>

namespace spatosc
{

/**
 * Translator for the SpatDIF protocol
 */
class SpatdifTranslator : public Translator
{
    public:
        explicit SpatdifTranslator(const std::string &ip);
        virtual ~SpatdifTranslator();
        virtual void update(Connection *conn);

    private:
        lo_address destAddr_;
        lo_server lo_serv_;
        // not implemented
        SpatdifTranslator(const SpatdifTranslator&);
        const SpatdifTranslator& operator=(const SpatdifTranslator&);
};

} // end namespace spatosc
#endif // _SPATDIF_TRANSLATOR_H_
