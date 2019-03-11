#ifndef BASE_H
#define BASE_H

#include <QByteArray>

typedef struct StudentOrEngineerInfo
{
    StudentOrEngineerInfo (){
        type = false;
        sex = false;
        age = 0;
        scoreOrWorkage = 0;
    }

    bool type;
    char name[12];
    bool sex;
    int age;
    float scoreOrWorkage;
}stuOrEngInfo;

#endif // BASE_H
