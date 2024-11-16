#include "../CV8Resource.h"
#include "V8Class.h"
#include "cpp-sdk/script-objects/IInteriorPortal.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_UINT(1, interiorId);
    V8_ARG_TO_UINT(2, portalIndex);

    std::shared_ptr<alt::IInterior> interior = alt::ICore::Instance().GetInterior(interiorId);
    V8_CHECK(interior, "interior doesn't exist");

    std::shared_ptr<alt::IInteriorPortal> portal = interior->GetPortalByIndex(portalIndex);
    V8_CHECK(portal, "interior portal doesn't exist");

    V8Helpers::SetObjectClass(info.GetIsolate(), info.This(), V8Class::ObjectClass::INTERIOR_PORTAL);
    info.This()->SetInternalField(1, info[0]);
    info.This()->SetInternalField(2, info[1]);
}

static void IndexGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(2, portalIndex);
    V8_RETURN_UINT(portalIndex);
}

static void RoomFromGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    // TODO: is is expected that it returns uint but setter takes int? (and same for RoomTo, same for Flag)
    V8_RETURN_INT(portal->GetRoomFrom());
}

static void RoomFromSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_TO_UINT(value, roomFrom);
    portal->SetRoomFrom(roomFrom);
}

static void RoomToGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_RETURN_INT(portal->GetRoomTo());
}

static void RoomToSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_TO_UINT(value, roomTo);
    portal->SetRoomTo(roomTo);
}

static void FlagGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_RETURN_INT(portal->GetFlag());
}

static void FlagSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_TO_UINT(value, flag);
    portal->SetFlag(flag);
}

static void CornerCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_RETURN_UINT(portal->GetCornerCount());
}

static void GetEntityArchetype(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, entityIndex);
    V8_RETURN_UINT(portal->GetEntityArcheType(entityIndex));
}

static void GetEntityFlag(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, entityIndex);
    V8_RETURN_INT(portal->GetEntityFlag(entityIndex));
}

static void SetEntityFlag(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, entityIndex);
    V8_ARG_TO_INT(2, flag);
    portal->SetEntityFlag(entityIndex, flag);
}

static void GetEntityPos(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, entityIndex);
    V8_GET_RESOURCE();
    V8_RETURN_VECTOR3(portal->GetEntityPosition(entityIndex));
}

static void GetEntityRot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, entityIndex);
    V8_GET_RESOURCE();
    V8_RETURN_VECTOR3(portal->GetEntityRotation(entityIndex));
}

static void GetCornerPos(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, cornerIndex);
    V8_GET_RESOURCE();
    V8_RETURN_VECTOR3(portal->GetCornerPosition(cornerIndex));
}


static void EntityCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_RETURN_UINT(portal->GetEntityCount());
}

static void SetCornerPos(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    GET_THIS_INTERIOR_PORTAL(portal);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, cornerIndex);
    V8_ARG_TO_VECTOR3(2, pos);
    portal->SetCornerPosition(cornerIndex, pos);
}

extern V8Class v8InteriorPortal("InteriorPortal",
                              Constructor,
                              [](v8::Local<v8::FunctionTemplate> tpl)
                              {
                                  v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                  // TODO: not sure about this one
                                  tpl->InstanceTemplate()->SetInternalFieldCount(3);

                                  V8Helpers::SetAccessor(isolate, tpl, "index", &IndexGetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "cornerCount", &CornerCountGetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "roomFrom", &RoomFromGetter, &RoomFromSetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "roomTo", &RoomToGetter, &RoomToSetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "flag", &FlagGetter, &FlagSetter);
                                  V8Helpers::SetAccessor(isolate, tpl, "entityCount", &EntityCountGetter);

                                  V8Helpers::SetMethod(isolate, tpl, "getCornerPos", &GetCornerPos);
                                  V8Helpers::SetMethod(isolate, tpl, "getEntityArchetype", &GetEntityArchetype);
                                  V8Helpers::SetMethod(isolate, tpl, "getEntityFlag", &GetEntityFlag);
                                  V8Helpers::SetMethod(isolate, tpl, "setEntityFlag", &SetEntityFlag);
                                  V8Helpers::SetMethod(isolate, tpl, "getEntityPos", &GetEntityPos);
                                  V8Helpers::SetMethod(isolate, tpl, "getEntityRot", &GetEntityRot);

                                  V8Helpers::SetMethod(isolate, tpl, "setCornerPos", &SetCornerPos);
                              });
