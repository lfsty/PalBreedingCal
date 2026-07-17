#pragma once

#include <QFlag>

enum Gender
{
    NOTSET = 0x01,
    Male   = 0x02,
    Female = 0x04,
};

Q_DECLARE_FLAGS(Genders, Gender)
Q_DECLARE_OPERATORS_FOR_FLAGS(Genders)