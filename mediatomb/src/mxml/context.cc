/*  context.cc - this file is part of MediaTomb.
                                                                                
    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
                       Sergey Bostandzhyan <jin@deadlock.dhs.org>
                                                                                
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
                                                                                
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                                                                                
    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "context.h"

using namespace zmm;
using namespace mxml;

Context::Context(String location) : Object()
{
	this->location = location;
	line = 1;
	col = 1;
}
Context::Context()
{
}

Ref<Context> Context::clone()
{
	Ref<Context> copy = Ref<Context>(new Context());
	copy->location = location;
	copy->parent = parent;
	copy->line = line;
	copy->col = col;
	return copy;
}

void Context::setParent(Ref<Context> parent)
{
	this->parent = parent;
}
