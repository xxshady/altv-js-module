#include "Log.h"

#ifdef ALT_CLIENT_API
    #include "CV8ScriptRuntime.h"
#else
    #include "CNodeScriptRuntime.h"
#endif

Log& Log::Endl(Log& log)
{
    auto& core = alt::ICore::Instance();

    v8::Isolate* isolate = nullptr;
#ifdef ALT_CLIENT_API
    isolate = CV8ScriptRuntime::Instance().GetIsolate();
#else
    isolate = v8::Isolate::GetCurrent();
#endif

    v8::Local<v8::Context> ctx;
    if(isolate) ctx = isolate->GetEnteredOrMicrotaskContext();
    V8ResourceImpl* v8Resource = !ctx.IsEmpty() ? V8ResourceImpl::Get(ctx) : nullptr;
    alt::IResource* resource = v8Resource ? v8Resource->GetResource() : nullptr;

    switch(log.type)
    {
        case INFO: core.LogInfo(LOG_PREFIX, log.buf.str(), resource); break;
        case DEBUG: core.LogDebug(LOG_PREFIX, log.buf.str().c_str(), resource); break;
        case WARNING: core.LogWarning(LOG_PREFIX, log.buf.str().c_str(), resource); break;
        case ERR: core.LogError(LOG_PREFIX, log.buf.str().c_str(), resource); break;
        case COLORED: core.LogColored(LOG_PREFIX, log.buf.str().c_str(), resource); break;
    }

    log.buf.str("");
    return log;
}
