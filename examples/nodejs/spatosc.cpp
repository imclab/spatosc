#include <node.h>
#include <v8.h>
//#include <spatosc/spatosc.h>

using namespace v8;

v8::Handle<Value> Method(const v8::Arguments& args)
{
    v8::HandleScope scope;
    return scope.Close(v8::String::New("world"));
}

void init(v8::Handle<v8::Object> target) {
    target->Set(v8::String::NewSymbol("hello"),
        v8::FunctionTemplate::New(Method)->GetFunction());
}

NODE_MODULE(hello, init)

