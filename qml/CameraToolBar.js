// -*- js -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

var stack = new Array();

function push(items) {
    if (stack.length >= 1) {
	hide(stack[stack.length - 1]);
    }

    stack.push(items);

    layout();
}

function pop() {
    var items = stack[stack.length - 1];
    hide(items);
    stack.pop();
    layout();
}

function hide(items) {
    var len = items.length;

    for (var x = 0; x < len; x++) {
        var item = items[x];
        item.visible = false;
    }
}

function show(items) {
    var len = items.length;

    var width = 0;
    for (var x = 0; x < len; x++) {
	width += items[x].width;
    }

    var totalWidth = tools.width - width;
    if (tools.hideBack) {
	len -= 1;
    } else {
	totalWidth -= tools.menuWidth;
    }

    var spacing = totalWidth / len;

    for (var x = 0; x < items.length; x++) {
	var child = items[x];

	if (x != 0) {
	    var prev = items[x - 1];
	    child.x = prev.x + prev.width + spacing;
	} else if (tools.hideBack) {
	    child.x = 0;
	} else {
	    child.x = spacing + 80;
	}

	child.parent = tools;
	child.visible = true;
	child.y = 0;
    }
}

function layout() {
    if (stack.length == 0) {
	return;
    }

    var items = stack[stack.length - 1];
    var len = items.length;

    if (!tools.expanded) {
	hide(items);
    }
    else if (tools.width == tools.targetWidth) {
	show(items);
    }
}
