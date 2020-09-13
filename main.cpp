#include <QCoreApplication>

#include <telegrambot.h>
#include <storage.h>
#include <glossary.h>
TelegramKeyboardRequest menuKeys =
{
    {
        TelegramBot::constructInlineButton("Вспомнить все!","/play")
    },
    {
        TelegramBot::constructInlineButton("Новая карточка", "/new_card")
    }
};


TelegramKeyboardRequest playKeys =
{
    {
        TelegramBot::constructInlineButton("<< В меню","/menu"),
        TelegramBot::constructInlineButton("Дальше >>", "/play")
    }
};

TelegramKeyboardRequest cancelKeys =
{
    {
        TelegramBot::constructInlineButton("Отмена","/menu"),
    }
};


Glossary menu = {
  "Что дальше?",
   "Что будем делать дальше?",
    "Чего прикажешь?",
    "Выбирай",
    "Вот мы снова в меню, поехали?"
};

Glossary newCard = {
    "Для создания карточки нужно ввести пару слов. Формат такой [оригинал]:[перевод]",
    "Ладно, отправь мне сообщение. Формат такой [оригинал]:[перевод]",
    "Тебе придется написать мне. Формат такой [оригинал]:[перевод]",
    "Давай, напиши мне, что хочешь запомнить. Формат такой [оригинал]:[перевод]"
};

Glossary playNext = {
  "Попробуй отгадай это, вот твое слово",
  "Что ты скажешь на это",
  "Вот твое слово, каков перевод",
  "Не могу разобраться, как переводится это слово"
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TelegramBot bot ("1386433528:AAGZ3nEkro2J4P5bQ3dq_0vGO1GoBPyAsDg");

    Storage storage = Storage::load("storage.json").unwrap();

    QObject::connect(&bot, &TelegramBot::newMessage, [&](TelegramBotUpdate update) {
        TelegramBotMessage * message = update->message;

        QString data;
        quint32 chat_id = message->chat.id;
        switch (update->type) {
        case TelegramBotMessageType::CallbackQuery:
            data = update->callbackQuery->data;
            break;
        case TelegramBotMessageType::Message:
            data = message->text;
            break;
        default:
            return;
        }


        User u;
        auto o = storage.byChatId(chat_id);
        if(o.isNone())
        {
            u = User(chat_id, message->chat.username);
            storage.newUser(u);
        } else
            u = o.unwrap();



        if(data == "/start" || data == "/menu")
        {
            u.drop();
            bot.sendMessage(chat_id, menu.randomPhrase(), 0, TelegramBot::Html, menuKeys);
        }
        else if(data == "/new_card" )
        {
            u.setTimeToCreate(30);
            bot.sendMessage(u.chat_id, newCard.randomPhrase(), 0, TelegramBot::Html, cancelKeys);
        }
        else if(data == "/play")
        {
           auto o = u.randomCard();
           if(o.isSome())
           {
               Card c = o.unwrap();
               bot.sendMessage(u.chat_id, QString("%1: %2").arg(playNext.randomPhrase()).arg(c.original), 0, TelegramBot::Html, playKeys);
           }
           else
               bot.sendMessage(u.chat_id, QString("К сожалению я не нашел карточек, попробуй добавить одну"), 0, TelegramBot::Html, menuKeys);

        }
        else
        {
            if(u.isWaitCard())
            {
                auto ot = data.split(":");
                if(ot.size() >= 2)
                {
                    u.remember(Card(ot.at(0), ot.at(1)));
                    bot.sendMessage(u.chat_id, QString("Отлично, карточка добавлена!\n%1").arg(menu.randomPhrase()), 0, TelegramBot::Html, menuKeys);
                }
                else
                    bot.sendMessage(u.chat_id, QString("Прости, но я не понимаю тебя.\n%1").arg(menu.randomPhrase()),0, TelegramBot::Html, menuKeys);

            }
            else if (u.isWaitAnswer())
            {
                if(u.isCorrectAnswer(data))
                {
                    u.drop();
                    Card c = u.randomCard().unwrap();
                    bot.sendMessage(u.chat_id, QString("Отлично, ты угадал!\n%1: %2").arg(playNext.randomPhrase()).arg(c.original), 0, TelegramBot::Html, playKeys);
                }
                else
                    bot.sendMessage(u.chat_id, QString("Нет, попробуй еще раз"), 0, TelegramBot::Html, menuKeys);
            }
        }
        u.sync();
    });

    bot.startMessagePulling();
    return a.exec();
}
























