#pragma once
#include <QDebug>
#include "QJsonDocument"
#include "QJsonObject"

#include "Source/Main/functions.h"
#include "Source/Main/structures.h"

using namespace std;

class JsonActionParser
{
public:
    JsonActionParser();
    static JsonActionParser *getInstance() { return &japInstance; }

    bool parseNewAction(QJsonObject jsonObject);

private:
    static JsonActionParser japInstance;
};
