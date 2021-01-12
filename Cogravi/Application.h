#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include <functional>
#include <GL/glew.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
using namespace std;

#include "LibroController.h"

namespace Equipo7
{
    class Application
    {
    private:

        const int WIDTH = 1280;
        const int HEIGHT = 800;

        GLFWwindow* window;
        int cantidad;

        LibroController* libroController;

        Libro** libros;
        Libro* libro;

        bool reporteOrdenadoPorClave = false;
        bool reporteOrdenadoPorProveedor = false;

        bool localizaClave = false;
        bool localizaProveedor = false;

        bool alta = false;
        bool baja = false;

        bool salir = false;
        int find = 2;
        
        bool firstAddLibro = false;
        bool firstDeleteLibro = false;

        std::function<void(int width, int height)> redimensionar;
        std::function<void(int key, int scancode, int action, int mods)> teclado;
        std::function<void(double xpos, double ypos)> mousePos;
        std::function<void(double xoffset, double yoffset)> desplazar;
        std::function<void(int button, int action, int mods)> mouseButton;

    public:

        static Application* Instance()
        {
            static Application instance;
            return &instance;
        }

        Application()
        {

            if (glfwInit() == GL_FALSE) return;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(WIDTH, HEIGHT, "Programa", NULL, NULL);
            if (!window) return;

            glfwMakeContextCurrent(window);

            if (GLenum err = glewInit()) return;

            glfwSetWindowUserPointer(window, this);

            libroController = new LibroController();
        }

        ~Application()
        {
            glfwMakeContextCurrent(window);
            glfwDestroyWindow(window);
            glfwTerminate();
        }

       

        void inicializarImgui()
        {
            const char* glsl_version = "#version 460";

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;          
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      
            io.ConfigFlags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;

            ImGui::StyleColorsDark();

            ImGuiStyle& style = ImGui::GetStyle();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                style.WindowRounding = 0.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            }

            ImGuiWindowFlags window_flags = 0;
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);
        }

        void interfazMenu()
        {
            ImGui::Begin("Menu", NULL);

            ImGui::Text("1. Alta de registro");
            if (ImGui::Checkbox("Registrar", &alta))
            {
                this->firstAddLibro = false;
                this->libros = libroController->getLibros();
                this->cantidad = libroController->getCantidad();
            }
            ImGui::Separator();

            ImGui::Text("2. Baja de registro");

            if (ImGui::Checkbox("Eliminar", &baja))
            {
                this->firstDeleteLibro = false;
                this->libros = libroController->getLibros();
                this->cantidad = libroController->getCantidad();
            }

            ImGui::Separator();

            ImGui::Text("3. Localiza registro");
            if (ImGui::Checkbox("Clave", &localizaClave))
            {
                find = 2;
                if (localizaProveedor)
                    localizaProveedor = false;
                
                this->libros = libroController->getLibros();
                this->cantidad = libroController->getCantidad();
            }

            if (ImGui::Checkbox("Proveedor", &localizaProveedor))
            {
                find = 2;
                if (localizaClave)
                    localizaClave = false;

                this->libros = libroController->getLibros();
                this->cantidad = libroController->getCantidad();
            }

            ImGui::Separator();

            ImGui::Text("4. Reportes");
            if (ImGui::Checkbox("Reporte ordenado por clave", &reporteOrdenadoPorClave))
            {
                if (reporteOrdenadoPorProveedor)
                    reporteOrdenadoPorProveedor = false;

                libroController->Reportes(1);
                this->libros = libroController->getLibros();
                this->cantidad = libroController->getCantidad();
            }
            if (ImGui::Checkbox("Reporte ordenado por proveedor", &reporteOrdenadoPorProveedor))
            {
                if (reporteOrdenadoPorClave)
                    reporteOrdenadoPorClave = false;

                libroController->Reportes(2);
                this->libros = libroController->getLibros();
                this->cantidad = libroController->getCantidad();
            }
            ImGui::Separator();

            ImGui::Text("5. Archivos");
            if (ImGui::Button("Archivo Clave"))
            {
                reporteOrdenadoPorClave = false;
                reporteOrdenadoPorProveedor = false;
                libroController->Archivoclave();
            }

            if (ImGui::Button("Archivo Proveedor"))
            {
                reporteOrdenadoPorClave = false;
                reporteOrdenadoPorProveedor = false;
                libroController->Archivoprov();
            }

            ImGui::Separator();

            if (ImGui::Button("Salir")) salir = true;

            ImGui::End();
        }

        void interfazAddLibro()
        {
            static string lib;
            static int index = 3;

            static int clave_libro;
            static char nombre_autor[40];
            static char nombre_libro[40];
            static char genero[40];
            static char nombre_edit[40];
            static char nombre_prov[40];
            static char numero_celular[15];
            static char correo[40];

            ImGui::Begin("Registrar", NULL);

            ImGui::InputInt("Clave", &clave_libro);
            ImGui::InputText("Libro", nombre_libro, sizeof(nombre_libro));
            ImGui::InputText("Autor", nombre_autor, sizeof(nombre_autor));
            ImGui::InputText("Genero", genero, sizeof(genero));
            ImGui::InputText("Editor", nombre_edit, sizeof(nombre_edit));
            ImGui::InputText("Proveedor", nombre_prov, sizeof(nombre_prov));
            ImGui::InputText("Celular", numero_celular, sizeof(numero_celular));
            ImGui::InputText("Correo", correo, sizeof(correo));

            if (ImGui::Button("Registrar"))
            {
                reporteOrdenadoPorClave = false;
                reporteOrdenadoPorProveedor = false;
                firstAddLibro = true;
                Libro* libro = new Libro(clave_libro, nombre_libro, nombre_autor, genero, nombre_edit, nombre_prov, numero_celular, correo);

                if (libro->validarClave() && libro->validarCorreo() && libro->validarNombreLibro() && libro->validarNombreProveedor() && libro->validarNumeroCelular())
                {
                    if (libroController->addLibro(libro))
                    {
                        lib = libro->nombre_libro;
                        index = true;

                        clave_libro = 0;
                        strcpy(nombre_libro, "");
                        strcpy(nombre_autor, "");
                        strcpy(genero, "");
                        strcpy(nombre_edit, "");
                        strcpy(nombre_prov, "");
                        strcpy(numero_celular, "");
                        strcpy(correo, "");
                    }
                    else
                        index = 2;
                }
                else
                    index = false;
            }

            if (firstAddLibro)
            {
                if (index == 1)
                    ImGui::Text("%s registrado correctamente", lib.c_str());
                else if (index == 0)
                    ImGui::Text("datos ingresados incorrectos");
                else if (index == 2)
                    ImGui::Text("clave duplicada");
            }

            ImGui::End();
        }

        void interfazDeleteLibro()
        {
            static int clave;
            static int index = 2;

            ImGui::Begin("Eliminar", NULL);

            if (ImGui::InputInt("Clave", &clave))
            {
                firstDeleteLibro = false;
            }

            if (ImGui::Button("Eliminar"))
            {
                reporteOrdenadoPorClave = false;
                reporteOrdenadoPorProveedor = false;
                firstDeleteLibro = true;
                if (libroController->validarClave(clave))
                {
                    index = true;
                    libroController->Baja(clave);
                }
                else
                {
                    index = false;
                }
            }

            if (firstDeleteLibro)
            {
                if (index == 1)
                    ImGui::Text("%d eliminado correctamente", clave);
                else if (index == 0)
                    ImGui::Text("%d no se encuentra registrado", clave);
            }

            ImGui::End();
        }

        void interfazLocalizaPorClave()
        {
            static int clave = 0;
            
            ImGui::Begin("Localizar", NULL);

            if (ImGui::InputInt("Clave", &clave)) find = 2;

            if (ImGui::Button("Buscar"))
            {
                this->libro = libroController->buscarPorClave(clave);
                if (this->libro != NULL)
                {
                    find = true;
                }
                else
                    find = false;
            }

            if (find == 1)
            {
                ImGui::Text("Clave: %d", libro->clave_libro);
                ImGui::Text("Libro: %s", libro->nombre_libro);
                ImGui::Text("Autor: %s", libro->nombre_autor);
                ImGui::Text("Genero: %s", libro->genero);
                ImGui::Text("Editor: %s", libro->nombre_edit);
                ImGui::Text("Proveedor: %s", libro->nombre_prov);
                ImGui::Text("Correo: %s", libro->correo);
            }
            else if (!find)
                ImGui::Text("la clave %d no se encuentra registrada", clave);
           
            ImGui::End();
        }

        void interfazLocalizaPorProveedor()
        {
            static char proveedor[30];
            static vector<Libro*> librosReportados;

            ImGui::Begin("Localizar", NULL);

            if (ImGui::InputText("Proveedor", proveedor, sizeof(proveedor))) find = 2;

            if (ImGui::Button("Buscar"))
            {
                librosReportados = libroController->buscarPorProveedor(proveedor);
                if (librosReportados.size() > 0)
                {
                    find = true;
                }
                else
                    find = false;
            }

            if (find == 1)
            {
                for (auto& libro : librosReportados)
                {
                    ImGui::Text("Clave: %d", libro->clave_libro);
                    ImGui::Text("Libro: %s", libro->nombre_libro);
                    ImGui::Text("Autor: %s", libro->nombre_autor);
                    ImGui::Text("Genero: %s", libro->genero);
                    ImGui::Text("Editor: %s", libro->nombre_edit);
                    ImGui::Text("Proveedor: %s", libro->nombre_prov);
                    ImGui::Text("Celular: %s", libro->numero_celular);
                    ImGui::Text("Correo: %s", libro->correo);
                    ImGui::Separator();
                }
                
            }
            else if (!find)
                ImGui::Text("el proveedor %s no se encuentra registrada", proveedor);

            ImGui::End();
        }

        void interfazReporteOrdenadoPorClave(Libro** libros, int cantidad)
        {
            ImGui::Begin("Reporte", NULL);

            for (int i = 0; i < cantidad; i++)
            {
                ImGui::Text("Clave: %d", libros[i]->clave_libro);
                ImGui::Text("Libro: %s", libros[i]->nombre_libro);
                ImGui::Text("Autor: %s", libros[i]->nombre_autor);
                ImGui::Text("Genero: %s", libros[i]->genero);
                ImGui::Text("Editor: %s", libros[i]->nombre_edit);
                ImGui::Text("Proveedor: %s", libros[i]->nombre_prov);
                ImGui::Text("Celular: %s", libros[i]->numero_celular);
                ImGui::Text("Correo: %s", libros[i]->correo);
                ImGui::Separator();
            }

            ImGui::End();
        }

        void interfazReporteOrdenadoPorProveedor(Libro** libros, int cantidad)
        {
            ImGui::Begin("Reporte", NULL);

            for (int i = 0; i < cantidad; i++)
            {
                ImGui::Text("Clave: %d", libros[i]->clave_libro);
                ImGui::Text("Libro: %s", libros[i]->nombre_libro);
                ImGui::Text("Autor: %s", libros[i]->nombre_autor);
                ImGui::Text("Genero: %s", libros[i]->genero);
                ImGui::Text("Editor: %s", libros[i]->nombre_edit);
                ImGui::Text("Proveedor: %s", libros[i]->nombre_prov);
                ImGui::Text("Celular: %s", libros[i]->numero_celular);
                ImGui::Text("Correo: %s", libros[i]->correo);
                ImGui::Separator();
            }

            ImGui::End();
        }

        int loop()
        {
            inicializarImgui();

            libroController->Captura();

            while (!glfwWindowShouldClose(window) && !this->salir)
            {

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                dockSpace();

                interfazMenu();

                if (alta)
                {
                    interfazAddLibro();
                }

                if (baja)
                {
                    interfazDeleteLibro();
                }

                if (localizaClave)
                {
                    interfazLocalizaPorClave();
                }

                if (localizaProveedor)
                {
                    interfazLocalizaPorProveedor();
                }

                if (reporteOrdenadoPorClave)
                {
                    interfazReporteOrdenadoPorClave(this->libros, this->cantidad);
                }
                
                if (reporteOrdenadoPorProveedor)
                {
                    interfazReporteOrdenadoPorProveedor(this->libros, this->cantidad);
                }
               
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                ImGuiIO& io = ImGui::GetIO(); (void)io;

                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    GLFWwindow* backup_current_context = glfwGetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    glfwMakeContextCurrent(backup_current_context);
                }

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            return 0;
        }

    private:

        void dockSpace()
        {
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->GetWorkPos());
                ImGui::SetNextWindowSize(viewport->GetWorkSize());
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", NULL, window_flags);
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            else
            {
                ImGuiIO& io = ImGui::GetIO();
                ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
                ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
                ImGui::SameLine(0.0f, 0.0f);
                if (ImGui::SmallButton("click here"))
                    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Options"))
                {
                    ImGui::MenuItem("Padding", NULL, &opt_padding);
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }
            ImGui::End();
        }

    };
}
#endif