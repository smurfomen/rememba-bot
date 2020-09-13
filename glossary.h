#ifndef GLOSSARY_H
#define GLOSSARY_H

#include <QString>
#include <QList>
class Glossary
{
public:
    Glossary(std::initializer_list<QString> list){
        for(auto & it : list)
        {
            phrases.append(it);
        }
    }

    QString randomPhrase(){
        int i = rand() % phrases.size();
        return phrases.at(i);
    }

    QList<QString> phrases;
};

#endif // GLOSSARY_H
