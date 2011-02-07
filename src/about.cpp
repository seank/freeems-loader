#include "about.h"

About::About(QWidget *parent)
    : QWidget(parent)
{
  static QString message =
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
      "http://www.gnu.org/licenses/. \n"
      ;
      ui.setupUi(this);
      ui.textBrowser->append(message);
}

About::~About()
{

}
