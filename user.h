#ifndef USER_H
#define USER_H
#include <qserializer.h>
#include <card.h>
#include <option.h>
#include <QDateTime>
#include <QTimer>
#include <memory>
class User : public QSerializer
{
    Q_GADGET
    QS_SERIALIZER
public:
    User();
    User(quint32 chat_id, QString username) : User(){
        this->chat_id = chat_id;
        this->username = username;
    }

    bool remember(const Card & card);

    bool operator==(const User & u);

    Option<Card> randomCard();

    void appendUniqueCards(const User &  from);

    void setTimeToCreate(quint16 sec);

    bool isWaitAnswer();

    bool isWaitCard();

    bool isCorrectAnswer(const QString & answer);

    void drop();

    void sync();

    int secretCardIndex = -1;

    class Storage * storage = nullptr;
    std::shared_ptr<QTimer> tmrNewCard;

    QS_FIELD(QString, username);
    QS_FIELD(quint32, chat_id);
    QS_COLLECTION_OBJECTS(QVector, Card, cards);
};

#endif // USER_H
