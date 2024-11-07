#include "../CV8Resource.h"
#include "V8Class.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, interiorId);

    std::shared_ptr<alt::IInterior> interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8Helpers::SetObjectClass(info.GetIsolate(), info.This(), V8Class::ObjectClass::INTERIOR);
    info.This()->SetInternalField(1, info[0]);
}

static void GetForInteriorID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, interiorId);
    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(interiorId) };

    extern V8Class v8Interior;
    V8_RETURN(v8Interior.New(isolate->GetEnteredOrMicrotaskContext(), args));
}

static void RoomCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8_RETURN_UINT(interior->GetRoomCount());
}

static void PortalCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8_RETURN_UINT(interior->GetPortalCount());
}

static void PositionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8_RETURN_VECTOR3(interior->GetPosition());
}

static void RotationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8_RETURN_VECTOR3(interior->GetRotation());
}

static void EntitiesExtentsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    auto extentInfo = interior->GetEntitiesExtents();
    INTERIOR_EXTENT_TO_OBJECT(extentInfo, obj);
    V8_RETURN(obj);
}

static void GetRoomByHash(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, roomHash);

    // 3 args: (interiorId: uint, value: uint, valueIsRoomIndex: bool)
    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(interiorId), V8Helpers::JSValue(roomHash), V8Helpers::JSValue(false) };

    extern V8Class v8InteriorRoom;
    V8_RETURN(v8InteriorRoom.New(isolate->GetEnteredOrMicrotaskContext(), args));
}

static void GetRoomByIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, roomIndex);

    // 3 args: (interiorId: uint, value: uint, valueIsRoomIndex: bool)
    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(interiorId), V8Helpers::JSValue(roomIndex), V8Helpers::JSValue(true) };

    extern V8Class v8InteriorRoom;
    V8_RETURN(v8InteriorRoom.New(isolate->GetEnteredOrMicrotaskContext(), args));
}

static void GetPortalByIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, portalIndex);

    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(interiorId), V8Helpers::JSValue(portalIndex) };

    extern V8Class v8InteriorPortal;
    V8_RETURN(v8InteriorPortal.New(isolate->GetEnteredOrMicrotaskContext(), args));
}

extern V8Class v8Interior("Interior",
                          Constructor,
                          [](v8::Local<v8::FunctionTemplate> tpl)
                          {
                              v8::Isolate* isolate = v8::Isolate::GetCurrent();

                              tpl->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(V8Class::InternalFields::COUNT));

                              V8Helpers::SetMethod(isolate, tpl, "getRoomByHash", &GetRoomByHash);
                              V8Helpers::SetMethod(isolate, tpl, "getRoomByIndex", &GetRoomByIndex);
                              V8Helpers::SetMethod(isolate, tpl, "getPortalByIndex", &GetPortalByIndex);

                              V8Helpers::SetAccessor(isolate, tpl, "roomCount", &RoomCountGetter);
                              V8Helpers::SetAccessor(isolate, tpl, "portalCount", &PortalCountGetter);
                              V8Helpers::SetAccessor(isolate, tpl, "pos", &PositionGetter);
                              V8Helpers::SetAccessor(isolate, tpl, "rot", &RotationGetter);
                              V8Helpers::SetAccessor(isolate, tpl, "entitiesExtents", &EntitiesExtentsGetter);

                              V8Helpers::SetStaticMethod(isolate, tpl, "getForInteriorID", &GetForInteriorID);
                          });
