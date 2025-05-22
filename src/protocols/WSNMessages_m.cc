//
// Generated file, do not edit! Created by opp_msgtool 6.1 from protocols/WSNMessages.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "WSNMessages_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace wsn_simulation {

Register_Class(BaseMessage)

BaseMessage::BaseMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

BaseMessage::BaseMessage(const BaseMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

BaseMessage::~BaseMessage()
{
}

BaseMessage& BaseMessage::operator=(const BaseMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void BaseMessage::copy(const BaseMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
}

void BaseMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
}

void BaseMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
}

int BaseMessage::getSrcId() const
{
    return this->srcId;
}

void BaseMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int BaseMessage::getDestId() const
{
    return this->destId;
}

void BaseMessage::setDestId(int destId)
{
    this->destId = destId;
}

int BaseMessage::getFrequency() const
{
    return this->frequency;
}

void BaseMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int BaseMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void BaseMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double BaseMessage::getTimestamp() const
{
    return this->timestamp;
}

void BaseMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int BaseMessage::getMessageType() const
{
    return this->messageType;
}

void BaseMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int BaseMessage::getHopCount() const
{
    return this->hopCount;
}

void BaseMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

class BaseMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
    };
  public:
    BaseMessageDescriptor();
    virtual ~BaseMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(BaseMessageDescriptor)

BaseMessageDescriptor::BaseMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::BaseMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

BaseMessageDescriptor::~BaseMessageDescriptor()
{
    delete[] propertyNames;
}

bool BaseMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<BaseMessage *>(obj)!=nullptr;
}

const char **BaseMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *BaseMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int BaseMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 7+base->getFieldCount() : 7;
}

unsigned int BaseMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
    };
    return (field >= 0 && field < 7) ? fieldTypeFlags[field] : 0;
}

const char *BaseMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
    };
    return (field >= 0 && field < 7) ? fieldNames[field] : nullptr;
}

int BaseMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    return base ? base->findField(fieldName) : -1;
}

const char *BaseMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
    };
    return (field >= 0 && field < 7) ? fieldTypeStrings[field] : nullptr;
}

const char **BaseMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *BaseMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int BaseMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void BaseMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'BaseMessage'", field);
    }
}

const char *BaseMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string BaseMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        default: return "";
    }
}

void BaseMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BaseMessage'", field);
    }
}

omnetpp::cValue BaseMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'BaseMessage' as cValue -- field index out of range?", field);
    }
}

void BaseMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BaseMessage'", field);
    }
}

const char *BaseMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr BaseMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void BaseMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    BaseMessage *pp = omnetpp::fromAnyPtr<BaseMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'BaseMessage'", field);
    }
}

Register_Class(TimeMessage)

TimeMessage::TimeMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

TimeMessage::TimeMessage(const TimeMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

TimeMessage::~TimeMessage()
{
}

TimeMessage& TimeMessage::operator=(const TimeMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void TimeMessage::copy(const TimeMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->referenceTime = other.referenceTime;
    this->accuracy = other.accuracy;
    this->identityInfo = other.identityInfo;
}

void TimeMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->referenceTime);
    doParsimPacking(b,this->accuracy);
    doParsimPacking(b,this->identityInfo);
}

void TimeMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->referenceTime);
    doParsimUnpacking(b,this->accuracy);
    doParsimUnpacking(b,this->identityInfo);
}

int TimeMessage::getSrcId() const
{
    return this->srcId;
}

void TimeMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int TimeMessage::getDestId() const
{
    return this->destId;
}

void TimeMessage::setDestId(int destId)
{
    this->destId = destId;
}

int TimeMessage::getFrequency() const
{
    return this->frequency;
}

void TimeMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int TimeMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void TimeMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double TimeMessage::getTimestamp() const
{
    return this->timestamp;
}

void TimeMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int TimeMessage::getMessageType() const
{
    return this->messageType;
}

void TimeMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int TimeMessage::getHopCount() const
{
    return this->hopCount;
}

void TimeMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

double TimeMessage::getReferenceTime() const
{
    return this->referenceTime;
}

void TimeMessage::setReferenceTime(double referenceTime)
{
    this->referenceTime = referenceTime;
}

double TimeMessage::getAccuracy() const
{
    return this->accuracy;
}

void TimeMessage::setAccuracy(double accuracy)
{
    this->accuracy = accuracy;
}

const char * TimeMessage::getIdentityInfo() const
{
    return this->identityInfo.c_str();
}

void TimeMessage::setIdentityInfo(const char * identityInfo)
{
    this->identityInfo = identityInfo;
}

class TimeMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_referenceTime,
        FIELD_accuracy,
        FIELD_identityInfo,
    };
  public:
    TimeMessageDescriptor();
    virtual ~TimeMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(TimeMessageDescriptor)

TimeMessageDescriptor::TimeMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::TimeMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

TimeMessageDescriptor::~TimeMessageDescriptor()
{
    delete[] propertyNames;
}

bool TimeMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TimeMessage *>(obj)!=nullptr;
}

const char **TimeMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *TimeMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int TimeMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int TimeMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_referenceTime
        FD_ISEDITABLE,    // FIELD_accuracy
        FD_ISEDITABLE,    // FIELD_identityInfo
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *TimeMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "referenceTime",
        "accuracy",
        "identityInfo",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int TimeMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "referenceTime") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "accuracy") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "identityInfo") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *TimeMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "double",    // FIELD_referenceTime
        "double",    // FIELD_accuracy
        "string",    // FIELD_identityInfo
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **TimeMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *TimeMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int TimeMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void TimeMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'TimeMessage'", field);
    }
}

const char *TimeMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TimeMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_referenceTime: return double2string(pp->getReferenceTime());
        case FIELD_accuracy: return double2string(pp->getAccuracy());
        case FIELD_identityInfo: return oppstring2string(pp->getIdentityInfo());
        default: return "";
    }
}

void TimeMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_referenceTime: pp->setReferenceTime(string2double(value)); break;
        case FIELD_accuracy: pp->setAccuracy(string2double(value)); break;
        case FIELD_identityInfo: pp->setIdentityInfo((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TimeMessage'", field);
    }
}

omnetpp::cValue TimeMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_referenceTime: return pp->getReferenceTime();
        case FIELD_accuracy: return pp->getAccuracy();
        case FIELD_identityInfo: return pp->getIdentityInfo();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'TimeMessage' as cValue -- field index out of range?", field);
    }
}

void TimeMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_referenceTime: pp->setReferenceTime(value.doubleValue()); break;
        case FIELD_accuracy: pp->setAccuracy(value.doubleValue()); break;
        case FIELD_identityInfo: pp->setIdentityInfo(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TimeMessage'", field);
    }
}

const char *TimeMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr TimeMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void TimeMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    TimeMessage *pp = omnetpp::fromAnyPtr<TimeMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TimeMessage'", field);
    }
}

Register_Class(TimeCalibrationMessage)

TimeCalibrationMessage::TimeCalibrationMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

TimeCalibrationMessage::TimeCalibrationMessage(const TimeCalibrationMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

TimeCalibrationMessage::~TimeCalibrationMessage()
{
}

TimeCalibrationMessage& TimeCalibrationMessage::operator=(const TimeCalibrationMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void TimeCalibrationMessage::copy(const TimeCalibrationMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->sendTime = other.sendTime;
    this->receiveTime = other.receiveTime;
    this->accuracy = other.accuracy;
    this->calibrationType = other.calibrationType;
}

void TimeCalibrationMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->sendTime);
    doParsimPacking(b,this->receiveTime);
    doParsimPacking(b,this->accuracy);
    doParsimPacking(b,this->calibrationType);
}

void TimeCalibrationMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->sendTime);
    doParsimUnpacking(b,this->receiveTime);
    doParsimUnpacking(b,this->accuracy);
    doParsimUnpacking(b,this->calibrationType);
}

int TimeCalibrationMessage::getSrcId() const
{
    return this->srcId;
}

void TimeCalibrationMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int TimeCalibrationMessage::getDestId() const
{
    return this->destId;
}

void TimeCalibrationMessage::setDestId(int destId)
{
    this->destId = destId;
}

int TimeCalibrationMessage::getFrequency() const
{
    return this->frequency;
}

void TimeCalibrationMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int TimeCalibrationMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void TimeCalibrationMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double TimeCalibrationMessage::getTimestamp() const
{
    return this->timestamp;
}

void TimeCalibrationMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int TimeCalibrationMessage::getMessageType() const
{
    return this->messageType;
}

void TimeCalibrationMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int TimeCalibrationMessage::getHopCount() const
{
    return this->hopCount;
}

void TimeCalibrationMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

double TimeCalibrationMessage::getSendTime() const
{
    return this->sendTime;
}

void TimeCalibrationMessage::setSendTime(double sendTime)
{
    this->sendTime = sendTime;
}

double TimeCalibrationMessage::getReceiveTime() const
{
    return this->receiveTime;
}

void TimeCalibrationMessage::setReceiveTime(double receiveTime)
{
    this->receiveTime = receiveTime;
}

double TimeCalibrationMessage::getAccuracy() const
{
    return this->accuracy;
}

void TimeCalibrationMessage::setAccuracy(double accuracy)
{
    this->accuracy = accuracy;
}

int TimeCalibrationMessage::getCalibrationType() const
{
    return this->calibrationType;
}

void TimeCalibrationMessage::setCalibrationType(int calibrationType)
{
    this->calibrationType = calibrationType;
}

class TimeCalibrationMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_sendTime,
        FIELD_receiveTime,
        FIELD_accuracy,
        FIELD_calibrationType,
    };
  public:
    TimeCalibrationMessageDescriptor();
    virtual ~TimeCalibrationMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(TimeCalibrationMessageDescriptor)

TimeCalibrationMessageDescriptor::TimeCalibrationMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::TimeCalibrationMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

TimeCalibrationMessageDescriptor::~TimeCalibrationMessageDescriptor()
{
    delete[] propertyNames;
}

bool TimeCalibrationMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TimeCalibrationMessage *>(obj)!=nullptr;
}

const char **TimeCalibrationMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *TimeCalibrationMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int TimeCalibrationMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 11+base->getFieldCount() : 11;
}

unsigned int TimeCalibrationMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_sendTime
        FD_ISEDITABLE,    // FIELD_receiveTime
        FD_ISEDITABLE,    // FIELD_accuracy
        FD_ISEDITABLE,    // FIELD_calibrationType
    };
    return (field >= 0 && field < 11) ? fieldTypeFlags[field] : 0;
}

const char *TimeCalibrationMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "sendTime",
        "receiveTime",
        "accuracy",
        "calibrationType",
    };
    return (field >= 0 && field < 11) ? fieldNames[field] : nullptr;
}

int TimeCalibrationMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "sendTime") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "receiveTime") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "accuracy") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "calibrationType") == 0) return baseIndex + 10;
    return base ? base->findField(fieldName) : -1;
}

const char *TimeCalibrationMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "double",    // FIELD_sendTime
        "double",    // FIELD_receiveTime
        "double",    // FIELD_accuracy
        "int",    // FIELD_calibrationType
    };
    return (field >= 0 && field < 11) ? fieldTypeStrings[field] : nullptr;
}

const char **TimeCalibrationMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *TimeCalibrationMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int TimeCalibrationMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void TimeCalibrationMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'TimeCalibrationMessage'", field);
    }
}

const char *TimeCalibrationMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TimeCalibrationMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_sendTime: return double2string(pp->getSendTime());
        case FIELD_receiveTime: return double2string(pp->getReceiveTime());
        case FIELD_accuracy: return double2string(pp->getAccuracy());
        case FIELD_calibrationType: return long2string(pp->getCalibrationType());
        default: return "";
    }
}

void TimeCalibrationMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_sendTime: pp->setSendTime(string2double(value)); break;
        case FIELD_receiveTime: pp->setReceiveTime(string2double(value)); break;
        case FIELD_accuracy: pp->setAccuracy(string2double(value)); break;
        case FIELD_calibrationType: pp->setCalibrationType(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TimeCalibrationMessage'", field);
    }
}

omnetpp::cValue TimeCalibrationMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_sendTime: return pp->getSendTime();
        case FIELD_receiveTime: return pp->getReceiveTime();
        case FIELD_accuracy: return pp->getAccuracy();
        case FIELD_calibrationType: return pp->getCalibrationType();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'TimeCalibrationMessage' as cValue -- field index out of range?", field);
    }
}

void TimeCalibrationMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sendTime: pp->setSendTime(value.doubleValue()); break;
        case FIELD_receiveTime: pp->setReceiveTime(value.doubleValue()); break;
        case FIELD_accuracy: pp->setAccuracy(value.doubleValue()); break;
        case FIELD_calibrationType: pp->setCalibrationType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TimeCalibrationMessage'", field);
    }
}

const char *TimeCalibrationMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr TimeCalibrationMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void TimeCalibrationMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    TimeCalibrationMessage *pp = omnetpp::fromAnyPtr<TimeCalibrationMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'TimeCalibrationMessage'", field);
    }
}

Register_Class(StatusMessage)

StatusMessage::StatusMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

StatusMessage::StatusMessage(const StatusMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

StatusMessage::~StatusMessage()
{
}

StatusMessage& StatusMessage::operator=(const StatusMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void StatusMessage::copy(const StatusMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->positionX = other.positionX;
    this->positionY = other.positionY;
    this->positionZ = other.positionZ;
    this->nodeType = other.nodeType;
    this->status = other.status;
    this->messageFrequency = other.messageFrequency;
    this->batteryLevel = other.batteryLevel;
}

void StatusMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->positionX);
    doParsimPacking(b,this->positionY);
    doParsimPacking(b,this->positionZ);
    doParsimPacking(b,this->nodeType);
    doParsimPacking(b,this->status);
    doParsimPacking(b,this->messageFrequency);
    doParsimPacking(b,this->batteryLevel);
}

void StatusMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->positionX);
    doParsimUnpacking(b,this->positionY);
    doParsimUnpacking(b,this->positionZ);
    doParsimUnpacking(b,this->nodeType);
    doParsimUnpacking(b,this->status);
    doParsimUnpacking(b,this->messageFrequency);
    doParsimUnpacking(b,this->batteryLevel);
}

int StatusMessage::getSrcId() const
{
    return this->srcId;
}

void StatusMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int StatusMessage::getDestId() const
{
    return this->destId;
}

void StatusMessage::setDestId(int destId)
{
    this->destId = destId;
}

int StatusMessage::getFrequency() const
{
    return this->frequency;
}

void StatusMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int StatusMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void StatusMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double StatusMessage::getTimestamp() const
{
    return this->timestamp;
}

void StatusMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int StatusMessage::getMessageType() const
{
    return this->messageType;
}

void StatusMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int StatusMessage::getHopCount() const
{
    return this->hopCount;
}

void StatusMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

double StatusMessage::getPositionX() const
{
    return this->positionX;
}

void StatusMessage::setPositionX(double positionX)
{
    this->positionX = positionX;
}

double StatusMessage::getPositionY() const
{
    return this->positionY;
}

void StatusMessage::setPositionY(double positionY)
{
    this->positionY = positionY;
}

double StatusMessage::getPositionZ() const
{
    return this->positionZ;
}

void StatusMessage::setPositionZ(double positionZ)
{
    this->positionZ = positionZ;
}

int StatusMessage::getNodeType() const
{
    return this->nodeType;
}

void StatusMessage::setNodeType(int nodeType)
{
    this->nodeType = nodeType;
}

int StatusMessage::getStatus() const
{
    return this->status;
}

void StatusMessage::setStatus(int status)
{
    this->status = status;
}

int StatusMessage::getMessageFrequency() const
{
    return this->messageFrequency;
}

void StatusMessage::setMessageFrequency(int messageFrequency)
{
    this->messageFrequency = messageFrequency;
}

double StatusMessage::getBatteryLevel() const
{
    return this->batteryLevel;
}

void StatusMessage::setBatteryLevel(double batteryLevel)
{
    this->batteryLevel = batteryLevel;
}

class StatusMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_positionX,
        FIELD_positionY,
        FIELD_positionZ,
        FIELD_nodeType,
        FIELD_status,
        FIELD_messageFrequency,
        FIELD_batteryLevel,
    };
  public:
    StatusMessageDescriptor();
    virtual ~StatusMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(StatusMessageDescriptor)

StatusMessageDescriptor::StatusMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::StatusMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

StatusMessageDescriptor::~StatusMessageDescriptor()
{
    delete[] propertyNames;
}

bool StatusMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<StatusMessage *>(obj)!=nullptr;
}

const char **StatusMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *StatusMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int StatusMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 14+base->getFieldCount() : 14;
}

unsigned int StatusMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_positionX
        FD_ISEDITABLE,    // FIELD_positionY
        FD_ISEDITABLE,    // FIELD_positionZ
        FD_ISEDITABLE,    // FIELD_nodeType
        FD_ISEDITABLE,    // FIELD_status
        FD_ISEDITABLE,    // FIELD_messageFrequency
        FD_ISEDITABLE,    // FIELD_batteryLevel
    };
    return (field >= 0 && field < 14) ? fieldTypeFlags[field] : 0;
}

const char *StatusMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "positionX",
        "positionY",
        "positionZ",
        "nodeType",
        "status",
        "messageFrequency",
        "batteryLevel",
    };
    return (field >= 0 && field < 14) ? fieldNames[field] : nullptr;
}

int StatusMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "positionX") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "positionY") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "positionZ") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "nodeType") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "status") == 0) return baseIndex + 11;
    if (strcmp(fieldName, "messageFrequency") == 0) return baseIndex + 12;
    if (strcmp(fieldName, "batteryLevel") == 0) return baseIndex + 13;
    return base ? base->findField(fieldName) : -1;
}

const char *StatusMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "double",    // FIELD_positionX
        "double",    // FIELD_positionY
        "double",    // FIELD_positionZ
        "int",    // FIELD_nodeType
        "int",    // FIELD_status
        "int",    // FIELD_messageFrequency
        "double",    // FIELD_batteryLevel
    };
    return (field >= 0 && field < 14) ? fieldTypeStrings[field] : nullptr;
}

const char **StatusMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *StatusMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int StatusMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void StatusMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'StatusMessage'", field);
    }
}

const char *StatusMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string StatusMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_positionX: return double2string(pp->getPositionX());
        case FIELD_positionY: return double2string(pp->getPositionY());
        case FIELD_positionZ: return double2string(pp->getPositionZ());
        case FIELD_nodeType: return long2string(pp->getNodeType());
        case FIELD_status: return long2string(pp->getStatus());
        case FIELD_messageFrequency: return long2string(pp->getMessageFrequency());
        case FIELD_batteryLevel: return double2string(pp->getBatteryLevel());
        default: return "";
    }
}

void StatusMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_positionX: pp->setPositionX(string2double(value)); break;
        case FIELD_positionY: pp->setPositionY(string2double(value)); break;
        case FIELD_positionZ: pp->setPositionZ(string2double(value)); break;
        case FIELD_nodeType: pp->setNodeType(string2long(value)); break;
        case FIELD_status: pp->setStatus(string2long(value)); break;
        case FIELD_messageFrequency: pp->setMessageFrequency(string2long(value)); break;
        case FIELD_batteryLevel: pp->setBatteryLevel(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'StatusMessage'", field);
    }
}

omnetpp::cValue StatusMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_positionX: return pp->getPositionX();
        case FIELD_positionY: return pp->getPositionY();
        case FIELD_positionZ: return pp->getPositionZ();
        case FIELD_nodeType: return pp->getNodeType();
        case FIELD_status: return pp->getStatus();
        case FIELD_messageFrequency: return pp->getMessageFrequency();
        case FIELD_batteryLevel: return pp->getBatteryLevel();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'StatusMessage' as cValue -- field index out of range?", field);
    }
}

void StatusMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_positionX: pp->setPositionX(value.doubleValue()); break;
        case FIELD_positionY: pp->setPositionY(value.doubleValue()); break;
        case FIELD_positionZ: pp->setPositionZ(value.doubleValue()); break;
        case FIELD_nodeType: pp->setNodeType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_status: pp->setStatus(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_messageFrequency: pp->setMessageFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_batteryLevel: pp->setBatteryLevel(value.doubleValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'StatusMessage'", field);
    }
}

const char *StatusMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr StatusMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void StatusMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    StatusMessage *pp = omnetpp::fromAnyPtr<StatusMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'StatusMessage'", field);
    }
}

Register_Class(StatusAggregationMessage)

StatusAggregationMessage::StatusAggregationMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

StatusAggregationMessage::StatusAggregationMessage(const StatusAggregationMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

StatusAggregationMessage::~StatusAggregationMessage()
{
}

StatusAggregationMessage& StatusAggregationMessage::operator=(const StatusAggregationMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void StatusAggregationMessage::copy(const StatusAggregationMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->nodeCount = other.nodeCount;
    this->activeNodes = other.activeNodes;
    this->resourceUsage = other.resourceUsage;
}

void StatusAggregationMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->nodeCount);
    doParsimPacking(b,this->activeNodes);
    doParsimPacking(b,this->resourceUsage);
}

void StatusAggregationMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->nodeCount);
    doParsimUnpacking(b,this->activeNodes);
    doParsimUnpacking(b,this->resourceUsage);
}

int StatusAggregationMessage::getSrcId() const
{
    return this->srcId;
}

void StatusAggregationMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int StatusAggregationMessage::getDestId() const
{
    return this->destId;
}

void StatusAggregationMessage::setDestId(int destId)
{
    this->destId = destId;
}

int StatusAggregationMessage::getFrequency() const
{
    return this->frequency;
}

void StatusAggregationMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int StatusAggregationMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void StatusAggregationMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double StatusAggregationMessage::getTimestamp() const
{
    return this->timestamp;
}

void StatusAggregationMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int StatusAggregationMessage::getMessageType() const
{
    return this->messageType;
}

void StatusAggregationMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int StatusAggregationMessage::getHopCount() const
{
    return this->hopCount;
}

void StatusAggregationMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

int StatusAggregationMessage::getNodeCount() const
{
    return this->nodeCount;
}

void StatusAggregationMessage::setNodeCount(int nodeCount)
{
    this->nodeCount = nodeCount;
}

int StatusAggregationMessage::getActiveNodes() const
{
    return this->activeNodes;
}

void StatusAggregationMessage::setActiveNodes(int activeNodes)
{
    this->activeNodes = activeNodes;
}

const char * StatusAggregationMessage::getResourceUsage() const
{
    return this->resourceUsage.c_str();
}

void StatusAggregationMessage::setResourceUsage(const char * resourceUsage)
{
    this->resourceUsage = resourceUsage;
}

class StatusAggregationMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_nodeCount,
        FIELD_activeNodes,
        FIELD_resourceUsage,
    };
  public:
    StatusAggregationMessageDescriptor();
    virtual ~StatusAggregationMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(StatusAggregationMessageDescriptor)

StatusAggregationMessageDescriptor::StatusAggregationMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::StatusAggregationMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

StatusAggregationMessageDescriptor::~StatusAggregationMessageDescriptor()
{
    delete[] propertyNames;
}

bool StatusAggregationMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<StatusAggregationMessage *>(obj)!=nullptr;
}

const char **StatusAggregationMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *StatusAggregationMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int StatusAggregationMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int StatusAggregationMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_nodeCount
        FD_ISEDITABLE,    // FIELD_activeNodes
        FD_ISEDITABLE,    // FIELD_resourceUsage
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *StatusAggregationMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "nodeCount",
        "activeNodes",
        "resourceUsage",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int StatusAggregationMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "nodeCount") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "activeNodes") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "resourceUsage") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *StatusAggregationMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "int",    // FIELD_nodeCount
        "int",    // FIELD_activeNodes
        "string",    // FIELD_resourceUsage
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **StatusAggregationMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *StatusAggregationMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int StatusAggregationMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void StatusAggregationMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'StatusAggregationMessage'", field);
    }
}

const char *StatusAggregationMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string StatusAggregationMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_nodeCount: return long2string(pp->getNodeCount());
        case FIELD_activeNodes: return long2string(pp->getActiveNodes());
        case FIELD_resourceUsage: return oppstring2string(pp->getResourceUsage());
        default: return "";
    }
}

void StatusAggregationMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_nodeCount: pp->setNodeCount(string2long(value)); break;
        case FIELD_activeNodes: pp->setActiveNodes(string2long(value)); break;
        case FIELD_resourceUsage: pp->setResourceUsage((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'StatusAggregationMessage'", field);
    }
}

omnetpp::cValue StatusAggregationMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_nodeCount: return pp->getNodeCount();
        case FIELD_activeNodes: return pp->getActiveNodes();
        case FIELD_resourceUsage: return pp->getResourceUsage();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'StatusAggregationMessage' as cValue -- field index out of range?", field);
    }
}

void StatusAggregationMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_nodeCount: pp->setNodeCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_activeNodes: pp->setActiveNodes(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_resourceUsage: pp->setResourceUsage(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'StatusAggregationMessage'", field);
    }
}

const char *StatusAggregationMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr StatusAggregationMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void StatusAggregationMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    StatusAggregationMessage *pp = omnetpp::fromAnyPtr<StatusAggregationMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'StatusAggregationMessage'", field);
    }
}

Register_Class(ManagementCommand)

ManagementCommand::ManagementCommand(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ManagementCommand::ManagementCommand(const ManagementCommand& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ManagementCommand::~ManagementCommand()
{
}

ManagementCommand& ManagementCommand::operator=(const ManagementCommand& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ManagementCommand::copy(const ManagementCommand& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->targetId = other.targetId;
    this->commandType = other.commandType;
    this->extensionParam = other.extensionParam;
    this->commandData = other.commandData;
    this->responseFlag = other.responseFlag;
}

void ManagementCommand::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->targetId);
    doParsimPacking(b,this->commandType);
    doParsimPacking(b,this->extensionParam);
    doParsimPacking(b,this->commandData);
    doParsimPacking(b,this->responseFlag);
}

void ManagementCommand::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->targetId);
    doParsimUnpacking(b,this->commandType);
    doParsimUnpacking(b,this->extensionParam);
    doParsimUnpacking(b,this->commandData);
    doParsimUnpacking(b,this->responseFlag);
}

int ManagementCommand::getSrcId() const
{
    return this->srcId;
}

void ManagementCommand::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int ManagementCommand::getDestId() const
{
    return this->destId;
}

void ManagementCommand::setDestId(int destId)
{
    this->destId = destId;
}

int ManagementCommand::getFrequency() const
{
    return this->frequency;
}

void ManagementCommand::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int ManagementCommand::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void ManagementCommand::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double ManagementCommand::getTimestamp() const
{
    return this->timestamp;
}

void ManagementCommand::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int ManagementCommand::getMessageType() const
{
    return this->messageType;
}

void ManagementCommand::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int ManagementCommand::getHopCount() const
{
    return this->hopCount;
}

void ManagementCommand::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

int ManagementCommand::getTargetId() const
{
    return this->targetId;
}

void ManagementCommand::setTargetId(int targetId)
{
    this->targetId = targetId;
}

int ManagementCommand::getCommandType() const
{
    return this->commandType;
}

void ManagementCommand::setCommandType(int commandType)
{
    this->commandType = commandType;
}

double ManagementCommand::getExtensionParam() const
{
    return this->extensionParam;
}

void ManagementCommand::setExtensionParam(double extensionParam)
{
    this->extensionParam = extensionParam;
}

const char * ManagementCommand::getCommandData() const
{
    return this->commandData.c_str();
}

void ManagementCommand::setCommandData(const char * commandData)
{
    this->commandData = commandData;
}

int ManagementCommand::getResponseFlag() const
{
    return this->responseFlag;
}

void ManagementCommand::setResponseFlag(int responseFlag)
{
    this->responseFlag = responseFlag;
}

class ManagementCommandDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_targetId,
        FIELD_commandType,
        FIELD_extensionParam,
        FIELD_commandData,
        FIELD_responseFlag,
    };
  public:
    ManagementCommandDescriptor();
    virtual ~ManagementCommandDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ManagementCommandDescriptor)

ManagementCommandDescriptor::ManagementCommandDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::ManagementCommand)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ManagementCommandDescriptor::~ManagementCommandDescriptor()
{
    delete[] propertyNames;
}

bool ManagementCommandDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ManagementCommand *>(obj)!=nullptr;
}

const char **ManagementCommandDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ManagementCommandDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ManagementCommandDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 12+base->getFieldCount() : 12;
}

unsigned int ManagementCommandDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_targetId
        FD_ISEDITABLE,    // FIELD_commandType
        FD_ISEDITABLE,    // FIELD_extensionParam
        FD_ISEDITABLE,    // FIELD_commandData
        FD_ISEDITABLE,    // FIELD_responseFlag
    };
    return (field >= 0 && field < 12) ? fieldTypeFlags[field] : 0;
}

const char *ManagementCommandDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "targetId",
        "commandType",
        "extensionParam",
        "commandData",
        "responseFlag",
    };
    return (field >= 0 && field < 12) ? fieldNames[field] : nullptr;
}

int ManagementCommandDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "targetId") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "commandType") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "extensionParam") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "commandData") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "responseFlag") == 0) return baseIndex + 11;
    return base ? base->findField(fieldName) : -1;
}

const char *ManagementCommandDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "int",    // FIELD_targetId
        "int",    // FIELD_commandType
        "double",    // FIELD_extensionParam
        "string",    // FIELD_commandData
        "int",    // FIELD_responseFlag
    };
    return (field >= 0 && field < 12) ? fieldTypeStrings[field] : nullptr;
}

const char **ManagementCommandDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ManagementCommandDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ManagementCommandDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ManagementCommandDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ManagementCommand'", field);
    }
}

const char *ManagementCommandDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ManagementCommandDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_targetId: return long2string(pp->getTargetId());
        case FIELD_commandType: return long2string(pp->getCommandType());
        case FIELD_extensionParam: return double2string(pp->getExtensionParam());
        case FIELD_commandData: return oppstring2string(pp->getCommandData());
        case FIELD_responseFlag: return long2string(pp->getResponseFlag());
        default: return "";
    }
}

void ManagementCommandDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_targetId: pp->setTargetId(string2long(value)); break;
        case FIELD_commandType: pp->setCommandType(string2long(value)); break;
        case FIELD_extensionParam: pp->setExtensionParam(string2double(value)); break;
        case FIELD_commandData: pp->setCommandData((value)); break;
        case FIELD_responseFlag: pp->setResponseFlag(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ManagementCommand'", field);
    }
}

omnetpp::cValue ManagementCommandDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_targetId: return pp->getTargetId();
        case FIELD_commandType: return pp->getCommandType();
        case FIELD_extensionParam: return pp->getExtensionParam();
        case FIELD_commandData: return pp->getCommandData();
        case FIELD_responseFlag: return pp->getResponseFlag();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ManagementCommand' as cValue -- field index out of range?", field);
    }
}

void ManagementCommandDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_targetId: pp->setTargetId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_commandType: pp->setCommandType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_extensionParam: pp->setExtensionParam(value.doubleValue()); break;
        case FIELD_commandData: pp->setCommandData(value.stringValue()); break;
        case FIELD_responseFlag: pp->setResponseFlag(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ManagementCommand'", field);
    }
}

const char *ManagementCommandDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ManagementCommandDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ManagementCommandDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ManagementCommand *pp = omnetpp::fromAnyPtr<ManagementCommand>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ManagementCommand'", field);
    }
}

Register_Class(ClusterAddressingMessage)

ClusterAddressingMessage::ClusterAddressingMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ClusterAddressingMessage::ClusterAddressingMessage(const ClusterAddressingMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ClusterAddressingMessage::~ClusterAddressingMessage()
{
}

ClusterAddressingMessage& ClusterAddressingMessage::operator=(const ClusterAddressingMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ClusterAddressingMessage::copy(const ClusterAddressingMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->sourceIdentity = other.sourceIdentity;
    this->targetIdentity = other.targetIdentity;
    this->addressingQuery = other.addressingQuery;
}

void ClusterAddressingMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->sourceIdentity);
    doParsimPacking(b,this->targetIdentity);
    doParsimPacking(b,this->addressingQuery);
}

void ClusterAddressingMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->sourceIdentity);
    doParsimUnpacking(b,this->targetIdentity);
    doParsimUnpacking(b,this->addressingQuery);
}

int ClusterAddressingMessage::getSrcId() const
{
    return this->srcId;
}

void ClusterAddressingMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int ClusterAddressingMessage::getDestId() const
{
    return this->destId;
}

void ClusterAddressingMessage::setDestId(int destId)
{
    this->destId = destId;
}

int ClusterAddressingMessage::getFrequency() const
{
    return this->frequency;
}

void ClusterAddressingMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int ClusterAddressingMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void ClusterAddressingMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double ClusterAddressingMessage::getTimestamp() const
{
    return this->timestamp;
}

void ClusterAddressingMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int ClusterAddressingMessage::getMessageType() const
{
    return this->messageType;
}

void ClusterAddressingMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int ClusterAddressingMessage::getHopCount() const
{
    return this->hopCount;
}

void ClusterAddressingMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

const char * ClusterAddressingMessage::getSourceIdentity() const
{
    return this->sourceIdentity.c_str();
}

void ClusterAddressingMessage::setSourceIdentity(const char * sourceIdentity)
{
    this->sourceIdentity = sourceIdentity;
}

const char * ClusterAddressingMessage::getTargetIdentity() const
{
    return this->targetIdentity.c_str();
}

void ClusterAddressingMessage::setTargetIdentity(const char * targetIdentity)
{
    this->targetIdentity = targetIdentity;
}

const char * ClusterAddressingMessage::getAddressingQuery() const
{
    return this->addressingQuery.c_str();
}

void ClusterAddressingMessage::setAddressingQuery(const char * addressingQuery)
{
    this->addressingQuery = addressingQuery;
}

class ClusterAddressingMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_sourceIdentity,
        FIELD_targetIdentity,
        FIELD_addressingQuery,
    };
  public:
    ClusterAddressingMessageDescriptor();
    virtual ~ClusterAddressingMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(ClusterAddressingMessageDescriptor)

ClusterAddressingMessageDescriptor::ClusterAddressingMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::ClusterAddressingMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ClusterAddressingMessageDescriptor::~ClusterAddressingMessageDescriptor()
{
    delete[] propertyNames;
}

bool ClusterAddressingMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ClusterAddressingMessage *>(obj)!=nullptr;
}

const char **ClusterAddressingMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ClusterAddressingMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ClusterAddressingMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int ClusterAddressingMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_sourceIdentity
        FD_ISEDITABLE,    // FIELD_targetIdentity
        FD_ISEDITABLE,    // FIELD_addressingQuery
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *ClusterAddressingMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "sourceIdentity",
        "targetIdentity",
        "addressingQuery",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int ClusterAddressingMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "sourceIdentity") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "targetIdentity") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "addressingQuery") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *ClusterAddressingMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "string",    // FIELD_sourceIdentity
        "string",    // FIELD_targetIdentity
        "string",    // FIELD_addressingQuery
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **ClusterAddressingMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ClusterAddressingMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ClusterAddressingMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ClusterAddressingMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ClusterAddressingMessage'", field);
    }
}

const char *ClusterAddressingMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ClusterAddressingMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_sourceIdentity: return oppstring2string(pp->getSourceIdentity());
        case FIELD_targetIdentity: return oppstring2string(pp->getTargetIdentity());
        case FIELD_addressingQuery: return oppstring2string(pp->getAddressingQuery());
        default: return "";
    }
}

void ClusterAddressingMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_sourceIdentity: pp->setSourceIdentity((value)); break;
        case FIELD_targetIdentity: pp->setTargetIdentity((value)); break;
        case FIELD_addressingQuery: pp->setAddressingQuery((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ClusterAddressingMessage'", field);
    }
}

omnetpp::cValue ClusterAddressingMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_sourceIdentity: return pp->getSourceIdentity();
        case FIELD_targetIdentity: return pp->getTargetIdentity();
        case FIELD_addressingQuery: return pp->getAddressingQuery();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ClusterAddressingMessage' as cValue -- field index out of range?", field);
    }
}

void ClusterAddressingMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceIdentity: pp->setSourceIdentity(value.stringValue()); break;
        case FIELD_targetIdentity: pp->setTargetIdentity(value.stringValue()); break;
        case FIELD_addressingQuery: pp->setAddressingQuery(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ClusterAddressingMessage'", field);
    }
}

const char *ClusterAddressingMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr ClusterAddressingMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ClusterAddressingMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ClusterAddressingMessage *pp = omnetpp::fromAnyPtr<ClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ClusterAddressingMessage'", field);
    }
}

Register_Class(InterClusterAddressingMessage)

InterClusterAddressingMessage::InterClusterAddressingMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

InterClusterAddressingMessage::InterClusterAddressingMessage(const InterClusterAddressingMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

InterClusterAddressingMessage::~InterClusterAddressingMessage()
{
}

InterClusterAddressingMessage& InterClusterAddressingMessage::operator=(const InterClusterAddressingMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void InterClusterAddressingMessage::copy(const InterClusterAddressingMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->sourceCluster = other.sourceCluster;
    this->targetCluster = other.targetCluster;
    this->addressingQuery = other.addressingQuery;
}

void InterClusterAddressingMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->sourceCluster);
    doParsimPacking(b,this->targetCluster);
    doParsimPacking(b,this->addressingQuery);
}

void InterClusterAddressingMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->sourceCluster);
    doParsimUnpacking(b,this->targetCluster);
    doParsimUnpacking(b,this->addressingQuery);
}

int InterClusterAddressingMessage::getSrcId() const
{
    return this->srcId;
}

void InterClusterAddressingMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int InterClusterAddressingMessage::getDestId() const
{
    return this->destId;
}

void InterClusterAddressingMessage::setDestId(int destId)
{
    this->destId = destId;
}

int InterClusterAddressingMessage::getFrequency() const
{
    return this->frequency;
}

void InterClusterAddressingMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int InterClusterAddressingMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void InterClusterAddressingMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double InterClusterAddressingMessage::getTimestamp() const
{
    return this->timestamp;
}

void InterClusterAddressingMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int InterClusterAddressingMessage::getMessageType() const
{
    return this->messageType;
}

void InterClusterAddressingMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int InterClusterAddressingMessage::getHopCount() const
{
    return this->hopCount;
}

void InterClusterAddressingMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

const char * InterClusterAddressingMessage::getSourceCluster() const
{
    return this->sourceCluster.c_str();
}

void InterClusterAddressingMessage::setSourceCluster(const char * sourceCluster)
{
    this->sourceCluster = sourceCluster;
}

const char * InterClusterAddressingMessage::getTargetCluster() const
{
    return this->targetCluster.c_str();
}

void InterClusterAddressingMessage::setTargetCluster(const char * targetCluster)
{
    this->targetCluster = targetCluster;
}

const char * InterClusterAddressingMessage::getAddressingQuery() const
{
    return this->addressingQuery.c_str();
}

void InterClusterAddressingMessage::setAddressingQuery(const char * addressingQuery)
{
    this->addressingQuery = addressingQuery;
}

class InterClusterAddressingMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_sourceCluster,
        FIELD_targetCluster,
        FIELD_addressingQuery,
    };
  public:
    InterClusterAddressingMessageDescriptor();
    virtual ~InterClusterAddressingMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(InterClusterAddressingMessageDescriptor)

InterClusterAddressingMessageDescriptor::InterClusterAddressingMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::InterClusterAddressingMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

InterClusterAddressingMessageDescriptor::~InterClusterAddressingMessageDescriptor()
{
    delete[] propertyNames;
}

bool InterClusterAddressingMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<InterClusterAddressingMessage *>(obj)!=nullptr;
}

const char **InterClusterAddressingMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *InterClusterAddressingMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int InterClusterAddressingMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int InterClusterAddressingMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_sourceCluster
        FD_ISEDITABLE,    // FIELD_targetCluster
        FD_ISEDITABLE,    // FIELD_addressingQuery
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *InterClusterAddressingMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "sourceCluster",
        "targetCluster",
        "addressingQuery",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int InterClusterAddressingMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "sourceCluster") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "targetCluster") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "addressingQuery") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *InterClusterAddressingMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "string",    // FIELD_sourceCluster
        "string",    // FIELD_targetCluster
        "string",    // FIELD_addressingQuery
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **InterClusterAddressingMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *InterClusterAddressingMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int InterClusterAddressingMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void InterClusterAddressingMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'InterClusterAddressingMessage'", field);
    }
}

const char *InterClusterAddressingMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string InterClusterAddressingMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_sourceCluster: return oppstring2string(pp->getSourceCluster());
        case FIELD_targetCluster: return oppstring2string(pp->getTargetCluster());
        case FIELD_addressingQuery: return oppstring2string(pp->getAddressingQuery());
        default: return "";
    }
}

void InterClusterAddressingMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_sourceCluster: pp->setSourceCluster((value)); break;
        case FIELD_targetCluster: pp->setTargetCluster((value)); break;
        case FIELD_addressingQuery: pp->setAddressingQuery((value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterClusterAddressingMessage'", field);
    }
}

omnetpp::cValue InterClusterAddressingMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_sourceCluster: return pp->getSourceCluster();
        case FIELD_targetCluster: return pp->getTargetCluster();
        case FIELD_addressingQuery: return pp->getAddressingQuery();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'InterClusterAddressingMessage' as cValue -- field index out of range?", field);
    }
}

void InterClusterAddressingMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sourceCluster: pp->setSourceCluster(value.stringValue()); break;
        case FIELD_targetCluster: pp->setTargetCluster(value.stringValue()); break;
        case FIELD_addressingQuery: pp->setAddressingQuery(value.stringValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterClusterAddressingMessage'", field);
    }
}

const char *InterClusterAddressingMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr InterClusterAddressingMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void InterClusterAddressingMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    InterClusterAddressingMessage *pp = omnetpp::fromAnyPtr<InterClusterAddressingMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InterClusterAddressingMessage'", field);
    }
}

Register_Class(WaveformConfigMessage)

WaveformConfigMessage::WaveformConfigMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

WaveformConfigMessage::WaveformConfigMessage(const WaveformConfigMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

WaveformConfigMessage::~WaveformConfigMessage()
{
}

WaveformConfigMessage& WaveformConfigMessage::operator=(const WaveformConfigMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void WaveformConfigMessage::copy(const WaveformConfigMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->waveformType = other.waveformType;
    this->parameterValue = other.parameterValue;
    this->configResponse = other.configResponse;
}

void WaveformConfigMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->waveformType);
    doParsimPacking(b,this->parameterValue);
    doParsimPacking(b,this->configResponse);
}

void WaveformConfigMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->waveformType);
    doParsimUnpacking(b,this->parameterValue);
    doParsimUnpacking(b,this->configResponse);
}

int WaveformConfigMessage::getSrcId() const
{
    return this->srcId;
}

void WaveformConfigMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int WaveformConfigMessage::getDestId() const
{
    return this->destId;
}

void WaveformConfigMessage::setDestId(int destId)
{
    this->destId = destId;
}

int WaveformConfigMessage::getFrequency() const
{
    return this->frequency;
}

void WaveformConfigMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int WaveformConfigMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void WaveformConfigMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double WaveformConfigMessage::getTimestamp() const
{
    return this->timestamp;
}

void WaveformConfigMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int WaveformConfigMessage::getMessageType() const
{
    return this->messageType;
}

void WaveformConfigMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int WaveformConfigMessage::getHopCount() const
{
    return this->hopCount;
}

void WaveformConfigMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

int WaveformConfigMessage::getWaveformType() const
{
    return this->waveformType;
}

void WaveformConfigMessage::setWaveformType(int waveformType)
{
    this->waveformType = waveformType;
}

double WaveformConfigMessage::getParameterValue() const
{
    return this->parameterValue;
}

void WaveformConfigMessage::setParameterValue(double parameterValue)
{
    this->parameterValue = parameterValue;
}

int WaveformConfigMessage::getConfigResponse() const
{
    return this->configResponse;
}

void WaveformConfigMessage::setConfigResponse(int configResponse)
{
    this->configResponse = configResponse;
}

class WaveformConfigMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_waveformType,
        FIELD_parameterValue,
        FIELD_configResponse,
    };
  public:
    WaveformConfigMessageDescriptor();
    virtual ~WaveformConfigMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(WaveformConfigMessageDescriptor)

WaveformConfigMessageDescriptor::WaveformConfigMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::WaveformConfigMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

WaveformConfigMessageDescriptor::~WaveformConfigMessageDescriptor()
{
    delete[] propertyNames;
}

bool WaveformConfigMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<WaveformConfigMessage *>(obj)!=nullptr;
}

const char **WaveformConfigMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *WaveformConfigMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int WaveformConfigMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 10+base->getFieldCount() : 10;
}

unsigned int WaveformConfigMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_waveformType
        FD_ISEDITABLE,    // FIELD_parameterValue
        FD_ISEDITABLE,    // FIELD_configResponse
    };
    return (field >= 0 && field < 10) ? fieldTypeFlags[field] : 0;
}

const char *WaveformConfigMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "waveformType",
        "parameterValue",
        "configResponse",
    };
    return (field >= 0 && field < 10) ? fieldNames[field] : nullptr;
}

int WaveformConfigMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "waveformType") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "parameterValue") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "configResponse") == 0) return baseIndex + 9;
    return base ? base->findField(fieldName) : -1;
}

const char *WaveformConfigMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "int",    // FIELD_waveformType
        "double",    // FIELD_parameterValue
        "int",    // FIELD_configResponse
    };
    return (field >= 0 && field < 10) ? fieldTypeStrings[field] : nullptr;
}

const char **WaveformConfigMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *WaveformConfigMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int WaveformConfigMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void WaveformConfigMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'WaveformConfigMessage'", field);
    }
}

const char *WaveformConfigMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string WaveformConfigMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_waveformType: return long2string(pp->getWaveformType());
        case FIELD_parameterValue: return double2string(pp->getParameterValue());
        case FIELD_configResponse: return long2string(pp->getConfigResponse());
        default: return "";
    }
}

void WaveformConfigMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_waveformType: pp->setWaveformType(string2long(value)); break;
        case FIELD_parameterValue: pp->setParameterValue(string2double(value)); break;
        case FIELD_configResponse: pp->setConfigResponse(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'WaveformConfigMessage'", field);
    }
}

omnetpp::cValue WaveformConfigMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_waveformType: return pp->getWaveformType();
        case FIELD_parameterValue: return pp->getParameterValue();
        case FIELD_configResponse: return pp->getConfigResponse();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'WaveformConfigMessage' as cValue -- field index out of range?", field);
    }
}

void WaveformConfigMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_waveformType: pp->setWaveformType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_parameterValue: pp->setParameterValue(value.doubleValue()); break;
        case FIELD_configResponse: pp->setConfigResponse(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'WaveformConfigMessage'", field);
    }
}

const char *WaveformConfigMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr WaveformConfigMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void WaveformConfigMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    WaveformConfigMessage *pp = omnetpp::fromAnyPtr<WaveformConfigMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'WaveformConfigMessage'", field);
    }
}

Register_Class(SpaceTimeSyncMessage)

SpaceTimeSyncMessage::SpaceTimeSyncMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

SpaceTimeSyncMessage::SpaceTimeSyncMessage(const SpaceTimeSyncMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

SpaceTimeSyncMessage::~SpaceTimeSyncMessage()
{
}

SpaceTimeSyncMessage& SpaceTimeSyncMessage::operator=(const SpaceTimeSyncMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void SpaceTimeSyncMessage::copy(const SpaceTimeSyncMessage& other)
{
    this->srcId = other.srcId;
    this->destId = other.destId;
    this->frequency = other.frequency;
    this->sequenceNumber = other.sequenceNumber;
    this->timestamp = other.timestamp;
    this->messageType = other.messageType;
    this->hopCount = other.hopCount;
    this->referenceX = other.referenceX;
    this->referenceY = other.referenceY;
    this->referenceZ = other.referenceZ;
    this->referenceTime = other.referenceTime;
    this->syncType = other.syncType;
}

void SpaceTimeSyncMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->srcId);
    doParsimPacking(b,this->destId);
    doParsimPacking(b,this->frequency);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->timestamp);
    doParsimPacking(b,this->messageType);
    doParsimPacking(b,this->hopCount);
    doParsimPacking(b,this->referenceX);
    doParsimPacking(b,this->referenceY);
    doParsimPacking(b,this->referenceZ);
    doParsimPacking(b,this->referenceTime);
    doParsimPacking(b,this->syncType);
}

void SpaceTimeSyncMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->srcId);
    doParsimUnpacking(b,this->destId);
    doParsimUnpacking(b,this->frequency);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->timestamp);
    doParsimUnpacking(b,this->messageType);
    doParsimUnpacking(b,this->hopCount);
    doParsimUnpacking(b,this->referenceX);
    doParsimUnpacking(b,this->referenceY);
    doParsimUnpacking(b,this->referenceZ);
    doParsimUnpacking(b,this->referenceTime);
    doParsimUnpacking(b,this->syncType);
}

int SpaceTimeSyncMessage::getSrcId() const
{
    return this->srcId;
}

void SpaceTimeSyncMessage::setSrcId(int srcId)
{
    this->srcId = srcId;
}

int SpaceTimeSyncMessage::getDestId() const
{
    return this->destId;
}

void SpaceTimeSyncMessage::setDestId(int destId)
{
    this->destId = destId;
}

int SpaceTimeSyncMessage::getFrequency() const
{
    return this->frequency;
}

void SpaceTimeSyncMessage::setFrequency(int frequency)
{
    this->frequency = frequency;
}

int SpaceTimeSyncMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void SpaceTimeSyncMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

double SpaceTimeSyncMessage::getTimestamp() const
{
    return this->timestamp;
}

void SpaceTimeSyncMessage::setTimestamp(double timestamp)
{
    this->timestamp = timestamp;
}

int SpaceTimeSyncMessage::getMessageType() const
{
    return this->messageType;
}

void SpaceTimeSyncMessage::setMessageType(int messageType)
{
    this->messageType = messageType;
}

int SpaceTimeSyncMessage::getHopCount() const
{
    return this->hopCount;
}

void SpaceTimeSyncMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

double SpaceTimeSyncMessage::getReferenceX() const
{
    return this->referenceX;
}

void SpaceTimeSyncMessage::setReferenceX(double referenceX)
{
    this->referenceX = referenceX;
}

double SpaceTimeSyncMessage::getReferenceY() const
{
    return this->referenceY;
}

void SpaceTimeSyncMessage::setReferenceY(double referenceY)
{
    this->referenceY = referenceY;
}

double SpaceTimeSyncMessage::getReferenceZ() const
{
    return this->referenceZ;
}

void SpaceTimeSyncMessage::setReferenceZ(double referenceZ)
{
    this->referenceZ = referenceZ;
}

double SpaceTimeSyncMessage::getReferenceTime() const
{
    return this->referenceTime;
}

void SpaceTimeSyncMessage::setReferenceTime(double referenceTime)
{
    this->referenceTime = referenceTime;
}

int SpaceTimeSyncMessage::getSyncType() const
{
    return this->syncType;
}

void SpaceTimeSyncMessage::setSyncType(int syncType)
{
    this->syncType = syncType;
}

class SpaceTimeSyncMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_srcId,
        FIELD_destId,
        FIELD_frequency,
        FIELD_sequenceNumber,
        FIELD_timestamp,
        FIELD_messageType,
        FIELD_hopCount,
        FIELD_referenceX,
        FIELD_referenceY,
        FIELD_referenceZ,
        FIELD_referenceTime,
        FIELD_syncType,
    };
  public:
    SpaceTimeSyncMessageDescriptor();
    virtual ~SpaceTimeSyncMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(SpaceTimeSyncMessageDescriptor)

SpaceTimeSyncMessageDescriptor::SpaceTimeSyncMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(wsn_simulation::SpaceTimeSyncMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

SpaceTimeSyncMessageDescriptor::~SpaceTimeSyncMessageDescriptor()
{
    delete[] propertyNames;
}

bool SpaceTimeSyncMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SpaceTimeSyncMessage *>(obj)!=nullptr;
}

const char **SpaceTimeSyncMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *SpaceTimeSyncMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int SpaceTimeSyncMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 12+base->getFieldCount() : 12;
}

unsigned int SpaceTimeSyncMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_srcId
        FD_ISEDITABLE,    // FIELD_destId
        FD_ISEDITABLE,    // FIELD_frequency
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_timestamp
        FD_ISEDITABLE,    // FIELD_messageType
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISEDITABLE,    // FIELD_referenceX
        FD_ISEDITABLE,    // FIELD_referenceY
        FD_ISEDITABLE,    // FIELD_referenceZ
        FD_ISEDITABLE,    // FIELD_referenceTime
        FD_ISEDITABLE,    // FIELD_syncType
    };
    return (field >= 0 && field < 12) ? fieldTypeFlags[field] : 0;
}

const char *SpaceTimeSyncMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "srcId",
        "destId",
        "frequency",
        "sequenceNumber",
        "timestamp",
        "messageType",
        "hopCount",
        "referenceX",
        "referenceY",
        "referenceZ",
        "referenceTime",
        "syncType",
    };
    return (field >= 0 && field < 12) ? fieldNames[field] : nullptr;
}

int SpaceTimeSyncMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "srcId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "frequency") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "timestamp") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "messageType") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "referenceX") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "referenceY") == 0) return baseIndex + 8;
    if (strcmp(fieldName, "referenceZ") == 0) return baseIndex + 9;
    if (strcmp(fieldName, "referenceTime") == 0) return baseIndex + 10;
    if (strcmp(fieldName, "syncType") == 0) return baseIndex + 11;
    return base ? base->findField(fieldName) : -1;
}

const char *SpaceTimeSyncMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_srcId
        "int",    // FIELD_destId
        "int",    // FIELD_frequency
        "int",    // FIELD_sequenceNumber
        "double",    // FIELD_timestamp
        "int",    // FIELD_messageType
        "int",    // FIELD_hopCount
        "double",    // FIELD_referenceX
        "double",    // FIELD_referenceY
        "double",    // FIELD_referenceZ
        "double",    // FIELD_referenceTime
        "int",    // FIELD_syncType
    };
    return (field >= 0 && field < 12) ? fieldTypeStrings[field] : nullptr;
}

const char **SpaceTimeSyncMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SpaceTimeSyncMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SpaceTimeSyncMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void SpaceTimeSyncMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'SpaceTimeSyncMessage'", field);
    }
}

const char *SpaceTimeSyncMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SpaceTimeSyncMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return long2string(pp->getSrcId());
        case FIELD_destId: return long2string(pp->getDestId());
        case FIELD_frequency: return long2string(pp->getFrequency());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_timestamp: return double2string(pp->getTimestamp());
        case FIELD_messageType: return long2string(pp->getMessageType());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_referenceX: return double2string(pp->getReferenceX());
        case FIELD_referenceY: return double2string(pp->getReferenceY());
        case FIELD_referenceZ: return double2string(pp->getReferenceZ());
        case FIELD_referenceTime: return double2string(pp->getReferenceTime());
        case FIELD_syncType: return long2string(pp->getSyncType());
        default: return "";
    }
}

void SpaceTimeSyncMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(string2long(value)); break;
        case FIELD_destId: pp->setDestId(string2long(value)); break;
        case FIELD_frequency: pp->setFrequency(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_timestamp: pp->setTimestamp(string2double(value)); break;
        case FIELD_messageType: pp->setMessageType(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        case FIELD_referenceX: pp->setReferenceX(string2double(value)); break;
        case FIELD_referenceY: pp->setReferenceY(string2double(value)); break;
        case FIELD_referenceZ: pp->setReferenceZ(string2double(value)); break;
        case FIELD_referenceTime: pp->setReferenceTime(string2double(value)); break;
        case FIELD_syncType: pp->setSyncType(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SpaceTimeSyncMessage'", field);
    }
}

omnetpp::cValue SpaceTimeSyncMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: return pp->getSrcId();
        case FIELD_destId: return pp->getDestId();
        case FIELD_frequency: return pp->getFrequency();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_timestamp: return pp->getTimestamp();
        case FIELD_messageType: return pp->getMessageType();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_referenceX: return pp->getReferenceX();
        case FIELD_referenceY: return pp->getReferenceY();
        case FIELD_referenceZ: return pp->getReferenceZ();
        case FIELD_referenceTime: return pp->getReferenceTime();
        case FIELD_syncType: return pp->getSyncType();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'SpaceTimeSyncMessage' as cValue -- field index out of range?", field);
    }
}

void SpaceTimeSyncMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        case FIELD_srcId: pp->setSrcId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destId: pp->setDestId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_frequency: pp->setFrequency(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_timestamp: pp->setTimestamp(value.doubleValue()); break;
        case FIELD_messageType: pp->setMessageType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_referenceX: pp->setReferenceX(value.doubleValue()); break;
        case FIELD_referenceY: pp->setReferenceY(value.doubleValue()); break;
        case FIELD_referenceZ: pp->setReferenceZ(value.doubleValue()); break;
        case FIELD_referenceTime: pp->setReferenceTime(value.doubleValue()); break;
        case FIELD_syncType: pp->setSyncType(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SpaceTimeSyncMessage'", field);
    }
}

const char *SpaceTimeSyncMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr SpaceTimeSyncMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void SpaceTimeSyncMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    SpaceTimeSyncMessage *pp = omnetpp::fromAnyPtr<SpaceTimeSyncMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SpaceTimeSyncMessage'", field);
    }
}

}  // namespace wsn_simulation

namespace omnetpp {

}  // namespace omnetpp

