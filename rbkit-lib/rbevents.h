#ifndef RBKIT_EVENTS_H
#define RBKIT_EVENTS_H

#include <QVariantMap>
#include <QSharedPointer>
#include <QVariantList>
#include <QDateTime>
#include <QString>
#include "stringutil.h"
#include "model/objectdetail.h"
#include <msgpack.hpp>


class Subscriber;               // this acts as processor also atm.

namespace RBKit
{
    // https://github.com/code-mancers/rbkit/blob/833c4bb/ext/rbkit_event.h#L5
    enum EventType
    {
        EtObjCreated       = 0,
        EtObjDestroyed     = 1,
        EtGcStart          = 2,
        EtGcStartM         = 3,
        EtGcEndS           = 4,
        EtObjectSpaceDump  = 5,
        EtGcStats          = 6,
        EtEventCollection  = 7
    };

    class EventDataBase
    {
    public:
        EventDataBase(QDateTime ts, EventType eventType);
        virtual void process(Subscriber& processor) const = 0;
        virtual ~EventDataBase() {}

        QDateTime timestamp;
        EventType eventType;
    };

    // event pointer.
    typedef QSharedPointer<EventDataBase> EventPtr;

    class EvtNewObject : public EventDataBase
    {
    public:
        EvtNewObject(QDateTime ts, EventType eventType, RBKit::ObjectDetailPtr object);
        void process(Subscriber& processor) const;

        RBKit::ObjectDetailPtr object;
    };

    class EvtDelObject : public EventDataBase
    {
    public:
        EvtDelObject(QDateTime ts, EventType eventType, QVariantMap payload);
        void process(Subscriber& processor) const;

        quint64 objectId;
    };

    class EvtGcStats : public EventDataBase
    {
    public:
        EvtGcStats(QDateTime ts, EventType eventType, QVariantMap payload);
        void process(Subscriber& processor) const;

        QVariantMap payload;
    };

    class EvtGcStart : public EventDataBase
    {
    public:
        EvtGcStart(QDateTime ts, EventType eventType);
        void process(Subscriber &processor) const;
    };

    class EvtGcStartM : public EventDataBase
    {
    public:
        EvtGcStartM(QDateTime ts, EventType eventType);
        void process(Subscriber &processor) const;
    };

    class EvtGcStop : public EventDataBase
    {
    public:
        EvtGcStop(QDateTime ts, EventType eventType);
        void process(Subscriber &processor) const;
    };

    class EvtObjectDump : public EventDataBase
    {
    public:
        EvtObjectDump(QDateTime ts, EventType eventType,
                      QList<RBKit::ObjectDetailPtr> objects);
        void process(Subscriber& processor) const;

        QList<RBKit::ObjectDetailPtr> objects;
    };

    class EvtCollection : public EventDataBase
    {
    public:
        EvtCollection(QDateTime ts, EventType eventType, QList<RBKit::EventPtr>);
        void process(Subscriber& process) const;

        QList<RBKit::EventPtr> events;
    };

    class EventParser
    {
    public:
        EventParser(const QByteArray& message);

    public:
        EventDataBase* parseEvent() const;

    public:                     // helpers
        RBKit::EventDataBase* eventFromMsgpackObject(msgpack::object&) const;
        QList<RBKit::EventPtr> parseEvents(const msgpack::object&) const;
        RBKit::EventType guessEvent(const msgpack::object&) const;

    public:
        QList<RBKit::ObjectDetailPtr> parseObjects(const msgpack::object&) const;

    private:
        const QByteArray rawMessage;
        msgpack::unpacked unpacked;
    };

    EventDataBase* parseEvent(const QByteArray& rawMessage);
    EventDataBase* makeEventFromQVariantMap(const QVariantMap& map);
}


#endif // RBKIT_EVENTS_H
