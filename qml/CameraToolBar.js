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

function __push(tools) {
    if (stack.length >= 1) {
	hide(stack[stack.length - 1]);
    }

    var container = createContainer(tools);
    stack.push(container);

    return container;
}

function push(tools) {
    var container = __push(tools);
    return container.tools;
}

function pop() {
    if (stack.length == 0) {
	return null;
    }

    var container = stack.pop();
    hide(container);
    destroyContainer(container);

    if (stack.length == 0) {
	return null;
    }

    container = stack[stack.length - 1];
    show(container);

    return container.tools;
}

function show(container) {
    container.tools.width = dock.width;
    container.tools.height = dock.height;
    container.tools.visible = true;
}

function hide(container) {
    container.tools.visible = false;
}

function createContainer(tools) {
    var container = toolsContainer.createObject(dock);
    container.tools = tools;
    container.owner = tools.parent;
    container.tools.parent = dock;

    return container;
}

function destroyContainer(container) {
    container.tools.parent = container.owner;
    container.tools = null;
    container.owner = null;
    container.destroy();
}

function pushAndShow(tools) {
    var container = __push(tools);
    show(container);
    return container.tools;
}

function clear() {
    while (stack.length > 0) {
	pop();
    }
}

function depth() {
    return stack.length;
}

function isEmpty() {
    return stack.length == 0 ? true : false;
}

function showLast() {
    show(stack[stack.length - 1])
}

function hideLast() {
    hide(stack[stack.length - 1])
}

function calculateChildrenWidth(children) {
    var totalWidth = 0;

    for (var x = 0; x < children.length; x++) {
	if (children[x].visible) {
	    totalWidth += children[x].width;
	}
    }

    return totalWidth;
}

function countVisibleChildren(children) {
    var total = 0;

    for (var x = 0; x < children.length; x++) {
	if (children[x].visible) {
	    ++total;
	}
    }

    return total;
}
