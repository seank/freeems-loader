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

#include "inc/freeems_loader.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	FreeEMS_Loader w;
	if (w.showHelp) {
		cout << " help requested ";
		return 2;
	}
	w.show();
	return a.exec();
}
