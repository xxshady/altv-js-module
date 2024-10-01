#include "../CV8Resource.h"
#include "V8Class.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, interiorId);

    auto interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    V8Helpers::SetObjectClass(info.GetIsolate(), info.This(), V8Class::ObjectClass::INTERIOR);
    info.This()->SetInternalField(1, v8::External::New(isolate, interior.get()));
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

static void GetRoomIndexByHash(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, hash);

    V8_RETURN_INT(interior->GetRoomIndexByHash(hash));
}

static void GetRoomName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, roomIndex);

    V8_RETURN_STRING(interior->GetRoomName(roomIndex));
}

static void GetRoomFlag(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, roomIndex);

    V8_RETURN_INT(interior->GetRoomFlag(roomIndex));
}

static void SetRoomFlag(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_UINT(1, roomIndex);
    V8_ARG_TO_UINT(1, roomFlag);

    interior->SetRoomFlag(roomIndex, roomFlag);
}

static void GetRoomExtents(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, roomId);

    auto extentInfo = interior->GetRoomExtents(roomId);

    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    obj->Set(ctx, V8Helpers::JSValue("min"), resource->CreateVector3(extentInfo.min));
    obj->Set(ctx, V8Helpers::JSValue("max"), resource->CreateVector3(extentInfo.max));
    V8_RETURN(obj);
}

static void SetRoomExtents(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(3);
    V8_ARG_TO_UINT(1, roomIndex);
    V8_ARG_TO_VECTOR3(2, min);
    V8_ARG_TO_VECTOR3(3, max);

    interior->SetRoomExtents(roomIndex, { min, max });
}

static void GetRoomTimecycle(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, roomIndex);

    V8_RETURN_UINT(interior->GetRoomTimecycle(roomIndex));
}

static void SetRoomTimecycle(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_UINT(1, roomIndex);
    V8_ARG_TO_UINT(2, timecycleHash);

    interior->SetRoomTimecycle(roomIndex, timecycleHash);
}

static void GetPortalCornerPosition(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_UINT(1, portalIndex);
    V8_ARG_TO_UINT(2, cornerIndex);

    V8_RETURN_VECTOR3(interior->GetPortalCornerPosition(portalIndex, cornerIndex));
}

static void SetPortalCornerPosition(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(3);
    V8_ARG_TO_UINT(1, portalIndex);
    V8_ARG_TO_UINT(2, cornerIndex);
    V8_ARG_TO_VECTOR3(3, position);

    interior->SetPortalCornerPosition(portalIndex, cornerIndex, position);
}

static void GetPortalRoomFrom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, portalIndex);

    V8_RETURN_INT(interior->GetPortalRoomFrom(portalIndex));
}

static void SetPortalRoomFrom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_UINT(1, portalIndex);
    V8_ARG_TO_INT(2, roomFrom);

    interior->SetPortalRoomFrom(portalIndex, roomFrom);
}

static void GetPortalRoomTo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, portalIndex);

    V8_RETURN_INT(interior->GetPortalRoomTo(portalIndex));
}

static void SetPortalRoomTo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_UINT(1, portalIndex);
    V8_ARG_TO_INT(2, roomTo);

    interior->SetPortalRoomTo(portalIndex, roomTo);
}

static void GetPortalFlag(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, portalIndex);

    V8_RETURN_INT(interior->GetPortalFlag(portalIndex));
}

static void SetPortalFlag(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_UINT(1, portalIndex);
    V8_ARG_TO_INT(2, flag);

    interior->SetPortalFlag(portalIndex, flag);
}

static void RoomCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_RETURN_UINT(interior->GetRoomCount());
}

static void PortalCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_RETURN_UINT(interior->GetPortalCount());
}

static void PositionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_RETURN_VECTOR3(interior->GetPosition());
}

static void RotationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    V8_RETURN_VECTOR3(interior->GetRotation());
}

static void EntitiesExtentsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, interior, alt::IInterior);

    auto extentInfo = interior->GetEntitiesExtents();

    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    obj->Set(ctx, V8Helpers::JSValue("min"), resource->CreateVector3(extentInfo.min));
    obj->Set(ctx, V8Helpers::JSValue("max"), resource->CreateVector3(extentInfo.max));
    V8_RETURN(obj);
}

extern V8Class v8Interior("Interior", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    tpl->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(V8Class::InternalFields::COUNT));

    V8Helpers::SetMethod(isolate, tpl, "getRoomIndexByHash", &GetRoomIndexByHash);
    V8Helpers::SetMethod(isolate, tpl, "getRoomName", &GetRoomName);

    V8Helpers::SetMethod(isolate, tpl, "getRoomFlag", &GetRoomFlag);
    V8Helpers::SetMethod(isolate, tpl, "setRoomFlag", &SetRoomFlag);

    V8Helpers::SetMethod(isolate, tpl, "getRoomExtents", &GetRoomExtents);
    V8Helpers::SetMethod(isolate, tpl, "setRoomExtents", &SetRoomExtents);

    V8Helpers::SetMethod(isolate, tpl, "getRoomTimecycle", &GetRoomTimecycle);
    V8Helpers::SetMethod(isolate, tpl, "setRoomTimecycle", &SetRoomTimecycle);

    V8Helpers::SetMethod(isolate, tpl, "getPortalCornerPosition", &GetPortalCornerPosition);
    V8Helpers::SetMethod(isolate, tpl, "setPortalCornerPosition", &SetPortalCornerPosition);

    V8Helpers::SetMethod(isolate, tpl, "getPortalRoomFrom", &GetPortalRoomFrom);
    V8Helpers::SetMethod(isolate, tpl, "setPortalRoomFrom", &SetPortalRoomFrom);

    V8Helpers::SetMethod(isolate, tpl, "getPortalRoomTo", &GetPortalRoomTo);
    V8Helpers::SetMethod(isolate, tpl, "setPortalRoomTo", &SetPortalRoomTo);

    V8Helpers::SetMethod(isolate, tpl, "getPortalFlag", &GetPortalFlag);
    V8Helpers::SetMethod(isolate, tpl, "setPortalFlag", &SetPortalFlag);

    V8Helpers::SetAccessor(isolate, tpl, "roomCount", &RoomCountGetter);
    V8Helpers::SetAccessor(isolate, tpl, "portalCount", &PortalCountGetter);
    V8Helpers::SetAccessor(isolate, tpl, "pos", &PositionGetter);
    V8Helpers::SetAccessor(isolate, tpl, "rot", &RotationGetter);
    V8Helpers::SetAccessor(isolate, tpl, "entitiesExtents", &EntitiesExtentsGetter);

    V8Helpers::SetStaticMethod(isolate, tpl, "getForInteriorID", &GetForInteriorID);
});
