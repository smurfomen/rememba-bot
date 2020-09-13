#ifndef CARD_H
#define CARD_H

#include <qserializer.h>
#include <QList>

class Card : public QSerializer
{
    Q_GADGET
    QS_SERIALIZER
public:
    Card();
    Card(QString original, QString translate){
        this->original = original.trimmed().toLower();
        this->translate = translate.trimmed().toLower();
    }

    bool operator==(const Card & card)
    {
        return original == card.original && translate == card.translate;
    }

    QS_FIELD(QString, original)
    QS_FIELD(QString, translate)
};

#endif // CARD_H
