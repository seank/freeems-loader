/* FreeEMS-Loader- the open source s19 loader with special features for FreeEMS
 *
 * Copyright (C) 2008-2011 by Sean Keys <skeys@powerefi.com>
 *
 * This file is part of the FreeEMS-Loader project.
 *
 * FreeEMS-Loader software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeEMS-Loader software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeEMS-Loader software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at info(at)powerefi(dot)com or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeEMS-Loader to load your firmware!
 *
 */

#include "inc/about.h"
#include <Qt>

About::About(QWidget *parent) :
  QDialog(parent, Qt::Window | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint) 
{
	setupUi(this);
	static QString const message = \
		"freeems-loader: a freeems firmware(s19) loader\n\n"

		"Copyright (C) 2010 Sean Keys skeys at ipdatasys\n\n"

		"This program is free software: you can redistribute \n"
		"it and/or modify it under the terms of the GNU General \n"
		"Public License as published by the Free Software \n"
		"Foundation, either version 3 of the License, or\n"
		"(at your option) any later version.\n\n"

		"This program is distributed in the hope that it \n"
		"will be useful, but WITHOUT ANY WARRANTY; without \n"
		"even the implied warranty of MERCHANTABILITY or \n"
		"FITNESS FOR A PARTICULAR PURPOSE. See the GNU General\n"
		"Public License for more details.\n\n"

		"You should have received a copy of the GNU General\n"
		"Public License along with this program. If not, see"
		"http://www.gnu.org/licenses/. \n";
	textBrowser->append(message);
}

About::~About() 
{
}
