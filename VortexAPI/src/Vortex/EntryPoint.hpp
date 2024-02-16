#pragma once

//#if defined(_MSC_VER)
//
//extern VX::Application* VX::CreateApplication();
//int main(int argc, char** argv)
//{
//    auto app = VX::CreateApplication();
//    app->Run();
//    delete app;
//    return 0;
//}
//#else
//
//extern VX::Application* VX::CreateApplication();
//int main(int argc, char** argv)
//{
//    auto app = VX::CreateApplication();
//    app->Run();
//    delete app;
//    return 0;
//}
//
//#endif

extern VX::Application* VX::CreateApplication();
int main(int argc, char** argv)
{
    VX::Log::Init();
    
    auto app = VX::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
