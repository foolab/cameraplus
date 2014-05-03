/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

/*!
    \class QRangeModel
    \brief The QRangeModel class, helps users to build components that depend
           on some value and/or position to be in a certain range previously defined

    With this class, the user sets a value range and a position range, which
    represent the valid values/positions the model can assume. It is worth telling
    that the value property always has priority over the position property. A nice use
    case, would be a Slider implementation with the help of QRangeModel. If the user sets
    a value range to [0,100], a position range to [50,100] and sets the value
    to 80, the equivalent position would be 90. After that, if the user decides to
    resize the slider, the value would be the same, but the knob position would
    be updated due to the new position range.

*/

#include "qrangemodel.h"

/*!
    Constructs a QRangeModel with \a parent
*/

QRangeModel::QRangeModel(QObject *parent)
  : QObject(parent),
    m_posatmin(0),
    m_posatmax(0),
    m_minimum(0),
    m_maximum(99),
    m_stepSize(0),
    m_pos(0),
    m_value(0),
    m_inverted(false) {

}

/*!
    Destroys the QRangeModel
*/

QRangeModel::~QRangeModel()
{

}

/*!
    Sets the range of valid positions, that \l position can assume externally, with
    \a min and \a max.
    Such range is represented by \l positionAtMinimum and \l positionAtMaximum
*/

void QRangeModel::setPositionRange(qreal min, qreal max)
{
    bool emitPosAtMinChanged = !qFuzzyCompare(min, m_posatmin);
    bool emitPosAtMaxChanged = !qFuzzyCompare(max, m_posatmax);

    if (!(emitPosAtMinChanged || emitPosAtMaxChanged))
        return;

    const qreal oldPosition = position();
    m_posatmin = min;
    m_posatmax = max;

    // When a new positionRange is defined, the position property must be updated based on the value property.
    // For instance, imagine that you have a valueRange of [0,100] and a position range of [20,100],
    // if a user set the value to 50, the position would be 60. If this positionRange is updated to [0,100], then
    // the new position, based on the value (50), will be 50.
    // If the newPosition is different than the old one, it must be updated, in order to emit
    // the positionChanged signal.
    m_pos = equivalentPosition(m_value);

    if (emitPosAtMinChanged)
        emit positionAtMinimumChanged(m_posatmin);
    if (emitPosAtMaxChanged)
        emit positionAtMaximumChanged(m_posatmax);

    emitValueAndPositionIfChanged(value(), oldPosition);
}
/*!
    Sets the range of valid values, that \l value can assume externally, with
    \a min and \a max. The range has the following constraint: \a min must be less or equal \a max
    Such range is represented by \l minimumValue and \l maximumValue
*/

void QRangeModel::setRange(qreal min, qreal max)
{
    bool emitMinimumChanged = !qFuzzyCompare(min, m_minimum);
    bool emitMaximumChanged = !qFuzzyCompare(max, m_maximum);

    if (!(emitMinimumChanged || emitMaximumChanged))
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();

    m_minimum = min;
    m_maximum = qMax(min, max);

    // Update internal position if it was changed. It can occurs if internal value changes, due to range update
    m_pos = equivalentPosition(m_value);

    if (emitMinimumChanged)
        emit minimumChanged(m_minimum);
    if (emitMaximumChanged)
        emit maximumChanged(m_maximum);

    emitValueAndPositionIfChanged(oldValue, oldPosition);
}

/*!
    \property QRangeModel::minimumValue
    \brief the minimum value that \l value can assume

    This property's default value is 0
*/

void QRangeModel::setMinimum(qreal min)
{
    setRange(min, m_maximum);
}

qreal QRangeModel::minimum() const
{
    return m_minimum;
}

/*!
    \property QRangeModel::maximumValue
    \brief the maximum value that \l value can assume

    This property's default value is 99
*/

void QRangeModel::setMaximum(qreal max)
{
    // if the new maximum value is smaller than
    // minimum, update minimum too
    setRange(qMin(m_minimum, max), max);
}

qreal QRangeModel::maximum() const
{
    return m_maximum;
}

/*!
    \property QRangeModel::stepSize
    \brief the value that is added to the \l value and \l position property

    Example: If a user sets a range of [0,100] and stepSize
    to 30, the valid values that are going to be seen externally would be: 0, 30, 60, 90, 100.
*/

void QRangeModel::setStepSize(qreal stepSize)
{
    stepSize = qMax(qreal(0.0), stepSize);
    if (qFuzzyCompare(stepSize, m_stepSize))
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();
    m_stepSize = stepSize;

    emit stepSizeChanged(m_stepSize);
    emitValueAndPositionIfChanged(oldValue, oldPosition);
}

qreal QRangeModel::stepSize() const
{
    return m_stepSize;
}

/*!
    Returns a valid position, respecting the \l positionAtMinimum,
    \l positionAtMaximum and the \l stepSize properties.
    Such calculation is based on the parameter \a value (which is valid externally).
*/

qreal QRangeModel::positionForValue(qreal value) const
{
    const qreal unconstrainedPosition = equivalentPosition(value);
    return publicPosition(unconstrainedPosition);
}

/*!
    \property QRangeModel::position
    \brief the current position of the model

    Represents a valid external position, based on the \l positionAtMinimum,
    \l positionAtMaximum and the \l stepSize properties.
    The user can set it internally with a position, that is not within the current position range,
    since it can become valid if the user changes the position range later.
*/

qreal QRangeModel::position() const
{
    // Return the internal position but observe boundaries and
    // stepSize restrictions.
    return publicPosition(m_pos);
}

void QRangeModel::setPosition(qreal newPosition)
{
    if (qFuzzyCompare(newPosition, m_pos))
        return;

    const qreal oldPosition = position();
    const qreal oldValue = value();

    // Update position and calculate new value
    m_pos = newPosition;
    m_value = equivalentValue(m_pos);
    emitValueAndPositionIfChanged(oldValue, oldPosition);
}

/*!
    \property QRangeModel::positionAtMinimum
    \brief the minimum value that \l position can assume

    This property's default value is 0
*/

void QRangeModel::setPositionAtMinimum(qreal min)
{
    setPositionRange(min, m_posatmax);
}

qreal QRangeModel::positionAtMinimum() const
{
    return m_posatmin;
}

/*!
    \property QRangeModel::positionAtMaximum
    \brief the maximum value that \l position can assume

    This property's default value is 0
*/

void QRangeModel::setPositionAtMaximum(qreal max)
{
    setPositionRange(m_posatmin, max);
}

qreal QRangeModel::positionAtMaximum() const
{
    return m_posatmax;
}

/*!
    Returns a valid value, respecting the \l minimumValue,
    \l maximumValue and the \l stepSize properties.
    Such calculation is based on the parameter \a position (which is valid externally).
*/

qreal QRangeModel::valueForPosition(qreal position) const
{
    const qreal unconstrainedValue = equivalentValue(position);
    return publicValue(unconstrainedValue);
}

/*!
    \property QRangeModel::value
    \brief the current value of the model

    Represents a valid external value, based on the \l minimumValue,
    \l maximumValue and the \l stepSize properties.
    The user can set it internally with a value, that is not within the current range,
    since it can become valid if the user changes the range later.
*/

qreal QRangeModel::value() const
{
    // Return internal value but observe boundaries and
    // stepSize restrictions
    return publicValue(m_value);
}

void QRangeModel::setValue(qreal newValue)
{
    if (qFuzzyCompare(newValue, m_value))
        return;

    const qreal oldValue = value();
    const qreal oldPosition = position();

    // Update relative value and position
    m_value = newValue;
    m_pos = equivalentPosition(m_value);
    emitValueAndPositionIfChanged(oldValue, oldPosition);
}

/*!
    \property QRangeModel::inverted
    \brief the model is inverted or not

    The model can be represented with an inverted behavior, e.g. when \l value assumes
    the maximum value (represented by \l maximumValue) the \l position will be at its
    minimum (represented by \l positionAtMinimum).
*/

void QRangeModel::setInverted(bool inverted)
{
    if (inverted == m_inverted)
        return;

    m_inverted = inverted;
    emit invertedChanged(m_inverted);

    // After updating the internal value, the position property can change.
    setPosition(equivalentPosition(m_value));
}

bool QRangeModel::inverted() const
{
    return m_inverted;
}

/*!
    Sets the \l value to \l minimumValue.
*/

void QRangeModel::toMinimum()
{
    setValue(m_minimum);
}

/*!
    Sets the \l value to \l maximumValue.
*/

void QRangeModel::toMaximum()
{
    setValue(m_maximum);
}

/*!
    Calculates the position that is going to be seen outside by the component
    that is using QRangeModel. It takes into account the \l stepSize,
    \l positionAtMinimum, \l positionAtMaximum properties
    and \a position that is passed as parameter.
*/

qreal QRangeModel::publicPosition(qreal position) const
{
    // Calculate the equivalent stepSize for the position property.
    const qreal min = effectivePosAtMin();
    const qreal max = effectivePosAtMax();
    const qreal valueRange = m_maximum - m_minimum;
    const qreal positionValueRatio = valueRange ? (max - min) / valueRange : 0;
    const qreal positionStep = m_stepSize * positionValueRatio;

    if (positionStep == 0)
        return (min < max) ? qBound(min, position, max) : qBound(max, position, min);

    const int stepSizeMultiplier = (position - min) / positionStep;

    // Test whether value is below minimum range
    if (stepSizeMultiplier < 0)
        return min;

    qreal leftEdge = (stepSizeMultiplier * positionStep) + min;
    qreal rightEdge = ((stepSizeMultiplier + 1) * positionStep) + min;

    if (min < max) {
        leftEdge = qMin(leftEdge, max);
        rightEdge = qMin(rightEdge, max);
    } else {
        leftEdge = qMax(leftEdge, max);
        rightEdge = qMax(rightEdge, max);
    }

    if (qAbs(leftEdge - position) <= qAbs(rightEdge - position))
        return leftEdge;
    return rightEdge;
}

/*!
    Calculates the value that is going to be seen outside by the component
    that is using QRangeModel. It takes into account the \l stepSize,
    \l minimumValue, \l maximumValue properties
    and \a value that is passed as parameter.
*/

qreal QRangeModel::publicValue(qreal value) const
{
    // It is important to do value-within-range check this
    // late (as opposed to during setPosition()). The reason is
    // QML bindings; a position that is initially invalid because it lays
    // outside the range, might become valid later if the range changes.

    if (m_stepSize == 0)
        return qBound(m_minimum, m_value, m_maximum);

    const int stepSizeMultiplier = (m_value - m_minimum) / m_stepSize;

    // Test whether value is below minimum range
    if (stepSizeMultiplier < 0)
        return m_minimum;

    const qreal leftEdge = qMin(m_maximum, (stepSizeMultiplier * m_stepSize) + m_minimum);
    const qreal rightEdge = qMin(m_maximum, ((stepSizeMultiplier + 1) * m_stepSize) + m_minimum);
    const qreal middle = (leftEdge + rightEdge) / 2;

    return (value <= middle) ? leftEdge : rightEdge;
}

/*!
    Checks if the \l value or \l position, that is seen by the user, has changed and emits the changed signal if it
    has changed.
*/

void QRangeModel::emitValueAndPositionIfChanged(const qreal oldValue, const qreal oldPosition)
{
    // Effective value and position might have changed even in cases when e.g. d->value is
    // unchanged. This will be the case when operating with values outside range:
    const qreal newValue = value();
    const qreal newPosition = position();
    if (!qFuzzyCompare(newValue, oldValue))
        emit valueChanged(newValue);
    if (!qFuzzyCompare(newPosition, oldPosition))
        emit positionChanged(newPosition);
}
