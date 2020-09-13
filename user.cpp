#include "user.h"
#include "storage.h"
User::User()
{
    tmrNewCard = std::shared_ptr<QTimer>(new QTimer);
    tmrNewCard->setSingleShot(true);
}

bool User::remember(const Card &card){
    if(isWaitCard())
    {
        tmrNewCard->stop();
        if(std::find(cards.begin(),cards.end(),card) == cards.end())
            cards.append(card);
        return true;
    }
    return false;
}

bool User::operator==(const User &u)
{
    return chat_id == u.chat_id;
}

Option<Card> User::randomCard(){
    tmrNewCard->stop();
    if(cards.size())
    {
        secretCardIndex = rand() % cards.size();
        Card c = cards.at(secretCardIndex);
        return Option<Card>::Some(c);
    }
    return Option<Card>::None();
}

void User::appendUniqueCards(const User &from){
    for(auto &card :  from.cards){
        if(std::find(cards.begin(),cards.end(), card) == cards.end())
            cards.append(card);
    }
}

void User::setTimeToCreate(quint16 sec){
    tmrNewCard->stop();
    tmrNewCard->start(sec*1000);
}

bool User::isWaitAnswer(){
    return secretCardIndex >= 0;
}

bool User::isWaitCard()
{
    return tmrNewCard->isActive();
}

bool User::isCorrectAnswer(const QString &answer) {
    return isWaitAnswer() && cards.at(secretCardIndex).translate == answer.trimmed().toLower();
}

void User::drop()
{
    secretCardIndex = -1;
}

void User::sync()
{
    storage->syncUser(*this);
}
