add_requires("glfw","glad","glm","assimp")
set_defaultarchs("x64")

target("LearnOpenGL")
    add_packages("glfw","glad","glm","assimp")
    add_includedirs("includes/")
    set_kind("binary")
    add_files("src/*.cpp")
    set_targetdir("./build")