#include "../CV8Resource.h"
#include "V8Class.h"
#include "cpp-sdk/script-objects/IInteriorRoom.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();

    V8_CHECK_ARGS_LEN(3);
    V8_ARG_TO_UINT(1, interiorId);
    V8_ARG_TO_UINT(2, value);
    V8_ARG_TO_BOOLEAN(3, isIndex);

    std::shared_ptr<alt::IInterior> interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    std::shared_ptr<alt::IInteriorRoom> room;
    if(isIndex)
    {
        room = interior->GetRoomByIndex(value);
        V8_CHECK(room, "interior room doesn't exist");
    }
    else
    {
        room = interior->GetRoomByHash(value);
    }
    V8_CHECK(room, "interior room doesn't exist");

    uint32_t roomIndex = room->GetIndex();

    V8Helpers::SetObjectClass(info.GetIsolate(), info.This(), V8Class::ObjectClass::INTERIOR_ROOM);
    info.This()->SetInternalField(1, info[0]);
    info.This()->SetInternalField(2, V8Helpers::JSValue(roomIndex));
}

static void IndexGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(2, roomIndex);
    V8_RETURN_UINT(roomIndex);
}

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_ROOM(room);

    V8_RETURN_STRING(room->GetName());
}

static void NameHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_ROOM(room);

    V8_RETURN_UINT(room->GetNameHash());
}

static void FlagGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_ROOM(room);

    V8_RETURN_INT(room->GetFlag());
}

static void FlagSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    GET_THIS_INTERIOR_ROOM(room);

    V8_TO_UINT(value, flag);
    room->SetFlag(flag);
}

static void TimecycleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_ROOM(room);

    V8_RETURN_UINT(room->GetTimecycle());
}

static void TimecycleSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    GET_THIS_INTERIOR_ROOM(room);

    V8_TO_UINT(value, timecycle);
    room->SetTimecycle(timecycle);
}

static void ExtentsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_ROOM(room);

    auto extentInfo = room->GetExtents();
    V8_GET_RESOURCE();
    INTERIOR_EXTENT_TO_OBJECT(extentInfo, obj);
    V8_RETURN(obj);
}

static void ExtentsSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    GET_THIS_INTERIOR_ROOM(room);
    
    V8_TO_OBJECT(value, extentInfo);

    auto minRaw = extentInfo->Get(ctx, v8::String::NewFromUtf8(isolate, "min").ToLocalChecked()).ToLocalChecked();
    V8_TO_VECTOR3(minRaw, min);

    auto maxRaw = extentInfo->Get(ctx, v8::String::NewFromUtf8(isolate, "max").ToLocalChecked()).ToLocalChecked();
    V8_TO_VECTOR3(maxRaw, max);

    room->SetExtents({ min, max });
}

extern V8Class v8InteriorRoom("InteriorRoom",
                              Constructor,
                              [](v8::Local<v8::FunctionTemplate> tpl)
                              {
                                  v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                  // TODO: not sure about this one
                                  tpl->InstanceTemplate()->SetInternalFieldCount(3);

                                  V8Helpers::SetAccessor(isolate, tpl, "index", &IndexGetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "name", &NameGetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "nameHash", &NameHashGetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "flag", &FlagGetter, FlagSetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "timecycle", &TimecycleGetter, TimecycleSetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "extents", &ExtentsGetter, ExtentsSetter);
                              });
