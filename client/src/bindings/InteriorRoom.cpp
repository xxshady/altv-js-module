#include "../CV8Resource.h"
#include "V8Class.h"
#include "cpp-sdk/script-objects/IInteriorRoom.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR()

    V8_CHECK_ARGS_LEN(3)
    V8_ARG_TO_UINT(1, interiorId)
    V8_ARG_TO_UINT(2, value)
    V8_ARG_TO_BOOLEAN(3, isIndex)

    std::shared_ptr<alt::IInterior> interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist")

    if(isIndex)
    {
        std::shared_ptr<alt::IInteriorRoom> room = interior->GetRoomByIndex(value);
        V8_CHECK(interior, "room doesn't exist")
    }
    else
    {
        std::shared_ptr<alt::IInteriorRoom> room = interior->GetRoomByHash(value);
        V8_CHECK(interior, "room doesn't exist")
    }

    V8Helpers::SetObjectClass(info.GetIsolate(), info.This(), V8Class::ObjectClass::INTERIOR_ROOM);
    info.This()->SetInternalField(1, info[0]);
    info.This()->SetInternalField(2, info[1]);
    info.This()->SetInternalField(3, info[2]);
}

static void GetRoomName(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId)
    V8_GET_THIS_INTERNAL_FIELD_UINT32(2, value)
    V8_GET_THIS_INTERNAL_FIELD_BOOLEAN(3, isIndex)

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist")
    std::shared_ptr<alt::IInteriorRoom> room;
    if(isIndex)
    {
        room = interior->GetRoomByIndex(value);
        V8_CHECK(interior, "room doesn't exist")
    }
    else
    {
        room = interior->GetRoomByHash(value);
        V8_CHECK(interior, "room doesn't exist")
    }

    V8_RETURN_STRING(room->GetName());
}

extern V8Class v8InteriorRoom("InteriorRoom",
                              Constructor,
                              [](v8::Local<v8::FunctionTemplate> tpl)
                              {
                                  v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                  tpl->InstanceTemplate()->SetInternalFieldCount(4);
                                  V8Helpers::SetAccessor(isolate, tpl, "name", &GetRoomName);
                              });
