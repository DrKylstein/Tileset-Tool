/*
 *  Tileset Tool, an editor for Commander Keen: Invasion of the Vorticons
 *  Tilesets.
 *  Copyright (C) 2010-2012  Kyle Delaney
 *
 *  This file is a part of Tileset Tool.
 *
 *  Tileset Tool is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Tileset Tool is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  You may contact the author at <dr.kylstein@gmail.com>
 */

#ifndef FOURWAY_HPP
#define FOURWAY_HPP

template <class T>
class FourWay {
	public:
		T& operator [] (int i) {
			//if(i > 0 && i < 4) {
				return _data[i];
			//}

		};
		const T& operator [] (int i) const {
			//if(i > 0 && i < 4) {
				return _data[i];
			//}
		};
		T top() {
			return _data[0];
		};
		T right() {
			return _data[1];
		};
		T bottom() {
			return _data[2];
		};
		T left() {
			return _data[3];
		};
		void setTop(T v) {
			_data[0] = v;
		};
		void setRight(T v) {
			_data[1] = v;
		};
		void setBottom(T v) {
			_data[2] = v;
		};
		void setLeft(T v) {
			_data[3] = v;
		};

	private:
		T _data[4];
};
#endif
