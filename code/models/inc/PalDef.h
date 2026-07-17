#pragma once

#include <QFlag>

enum Gender
{
    NOTSET = 0x00,
    Male   = 0x01,
    Female = 0x02,
};

Q_DECLARE_FLAGS(Genders, Gender)
Q_DECLARE_OPERATORS_FOR_FLAGS(Genders)