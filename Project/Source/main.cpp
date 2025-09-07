#include "Core.h"

int main()
{
    Core::Debug::Logger::Get();


    delete Core::Debug::Logger::Get();
}
