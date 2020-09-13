#ifndef STORAGE_H
#define STORAGE_H
#include <qserializer.h>
#include <user.h>
#include <option.h>
#include <QFile>
class Storage : public QSerializer
{
    Q_GADGET
    QS_SERIALIZER
public:
    Storage();
    Storage(QString path){
        this->path = path;
    }
    static Option<Storage> load(QString path){
        Storage s (path);
        if(s.load())
            return Option<Storage>::Some(s);

        return Option<Storage>::None();
    }

    bool load(){
        QFile f(path);

        if(!f.exists())
            upload();

        if(f.open(QIODevice::ReadOnly))
        {
            fromJson(f.readAll());
            f.close();
            return true;
        }
        return false;
    }

    bool upload(){
        QFile f(path);
        bool success = false;
        if(f.open(QIODevice::WriteOnly))
        {
            success = f.write(QSerializer::toByteArray(toJson()));
            f.close();
        }
        return success;
    }


    void newUser(User & u){
        u.storage = this;
        if(std::find(users.begin(),users.end(), u) == users.end())
            users.append(u);
    }

    void syncUser(const User & u){
        auto it = std::find(users.begin(), users.end(), u);
        if(it != users.end())
        {
            *it = u;
            upload();
        }
    }

    Option<User> byChatId(quint32 chat_id){
        for(auto & u : users){
            if(u.chat_id == chat_id)
            {
                u.storage = this;
                return Option<User>::Some(u);
            }
        }
        return Option<User>::None();
    }

    QString path;
    QS_COLLECTION_OBJECTS(QVector, User, users);
};

#endif // STORAGE_H
