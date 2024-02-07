#ifndef EntryPoint_hpp
#define EntryPoint_hpp


#endif

#if defined(_MSC_VER)

extern VX::Application* VX::CreateApplication();
int main(int argc, char** argv)
{
    auto app = VX::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
#else

extern VX::Application* VX::CreateApplication();
int main(int argc, char** argv)
{
    auto app = VX::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif
