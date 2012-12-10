@echo off
:: ==========================================================================
:: Product: QP-nano; script for building the game-gui application
:: Last Updated for Version: 4.5.03
:: Date of the Last Update:  Nov 26, 2012
::
::                    Q u a n t u m     L e a P s
::                    ---------------------------
::                    innovating embedded systems
::
:: Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
::
:: This program is open source software: you can redistribute it and/or
:: modify it under the terms of the GNU General Public License as published
:: by the Free Software Foundation, either version 2 of the License, or
:: (at your option) any later version.
::
:: Alternatively, this program may be distributed and modified under the
:: terms of Quantum Leaps commercial licenses, which expressly supersede
:: the GNU General Public License and are specifically designed for
:: licensees interested in retaining the proprietary status of their code.
::
:: This program is distributed in the hope that it will be useful,
:: but WITHOUT ANY WARRANTY; without even the implied warranty of
:: MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
:: GNU General Public License for more details.
::
:: You should have received a copy of the GNU General Public License
:: along with this program. If not, see <http://www.gnu.org/licenses/>.
::
:: Contact information:
:: Quantum Leaps Web sites: http://www.quantum-leaps.com
::                          http://www.state-machine.com
:: e-mail:                  info@quantum-leaps.com
:: ==========================================================================

:: NOTE: Uncomment the following line to re-build the resources (requires
:: the windres utility from MinGW).
::
::windres -O coff game-gui.rc -o Res\game-gui.o

:: NOTE: This batch file expects that the tcc compiler (TinyCC) is in the PATH
::

tcc bsp.c main.c ship.c missile.c tunnel.c mine1.c mine2.c Res\game-gui.o %QPN%\source\qepn.c %QPN%\ports\win32\qfn_win32.c %QPN%\ports\win32\win32_gui.c -I. -I%QPN%\include -I%QPN%\ports\win32 -o game-gui.exe