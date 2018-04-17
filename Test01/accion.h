#ifndef ACCION_H
#define ACCION_H

#include <string>
class Accion
{
public:
    Accion();

    int listIndex;
    std::string accionString;
    bool completed;
    std::string actionPerformed;


};

#endif // ACCION_H
