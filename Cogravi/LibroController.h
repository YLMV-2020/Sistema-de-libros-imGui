#ifndef LIBRO_CONTROLLER_HEADER
#define LIBRO_CONTROLLER_HEADER

#include "Libro.h"
#include <fstream>
#include <string>
#include <vector>

namespace Equipo7
{
	class LibroController
	{

	private:

		unsigned int MAX = 50;
		Libro** libros;
		int cantidad;

	public:

		LibroController()
		{
			this->libros = new Libro*[MAX];
			this->cantidad = 0;
		}

		~LibroController()
		{

		}

		bool addLibro(int clave_libro, const char nombre_libro[30], const char nombre_autor[30], char genero[30], const char nombre_edit[30], const char nombre_prov[30], const char numero_celular[15], const char correo[30])
		{
			if (cantidad >= 50 || validarClave(clave_libro)) return false;
			Alta(new Libro(clave_libro, nombre_libro, nombre_autor, genero, nombre_edit, nombre_prov, numero_celular, correo));
			return true;
		}

		bool addLibro(Libro* libro)
		{
			if (cantidad >= 50 || validarClave(libro->clave_libro)) return false;
			Alta(libro);
			return true;
		}
		
		//funcion para capturar del archivo
		void Captura()
		{
			std::ifstream IN;
			IN.open("archivos/registros.txt", std::ifstream::in);

			string clave;

			if (IN.fail())
			{
				cout << "No se pudo abrir el archivo\n";
				return;
			}

			while (!IN.eof())
			{
				libros[cantidad] = new Libro();
				getline(IN, clave);

				if (clave != "")
				{
					libros[cantidad]->clave_libro = atoi(clave.c_str());
					IN.getline(libros[cantidad]->nombre_libro, 30);
					IN.getline(libros[cantidad]->nombre_autor, 30);
					IN.getline(libros[cantidad]->genero, 30);
					IN.getline(libros[cantidad]->nombre_edit, 30);
					IN.getline(libros[cantidad]->nombre_prov, 30);
					IN.getline(libros[cantidad]->numero_celular, 30);
					IN.getline(libros[cantidad]->correo, 30);
					cantidad++;
				}
			}
		}

		//funcion para agregar datos
		void Alta(Libro *libro)
		{
			std::ofstream OF;
			OF.open("archivos/registros.txt", ios::app);

			if (OF.fail())
			{
				cout << "Error en archivo";
				return;
			}

			if (cantidad > 0)
				OF << "\n";
			OF << libro->clave_libro << "\n";
			OF << libro->nombre_libro << "\n";
			OF << libro->nombre_autor << "\n";
			OF << libro->genero << "\n";
			OF << libro->nombre_edit << "\n";
			OF << libro->nombre_prov << "\n";
			OF << libro->numero_celular << "\n";
			OF << libro->correo;
			OF.close();

			this->cantidad = 0;

			Captura();
			
		}

		//funcion para borrar datos
		void Baja(int clave) 
		{
			std::ofstream OF;
			OF.open("archivos/registros.txt");

			if (OF.fail())
			{
				cout << "Error en archivo";
				return;
			}
			if (cantidad != 2)
			{
				for (int i = 0; i < cantidad; i++)
				{
					if (libros[i]->clave_libro != clave)
					{
						OF << libros[i]->clave_libro << "\n";
						OF << libros[i]->nombre_libro << "\n";
						OF << libros[i]->nombre_autor << "\n";
						OF << libros[i]->genero << "\n";
						OF << libros[i]->nombre_edit << "\n";
						OF << libros[i]->nombre_prov << "\n";
						OF << libros[i]->numero_celular << "\n";
						if (cantidad - 1 == i)
							OF << libros[i]->correo;
						else
							OF << libros[i]->correo << "\n";
					}
				}
			}
			else
			{
				for (int i = 0; i < 2; i++)
				{
					if (libros[i]->clave_libro != clave)
					{
						OF << libros[i]->clave_libro << "\n";
						OF << libros[i]->nombre_libro << "\n";
						OF << libros[i]->nombre_autor << "\n";
						OF << libros[i]->genero << "\n";
						OF << libros[i]->nombre_edit << "\n";
						OF << libros[i]->nombre_prov << "\n";
						OF << libros[i]->numero_celular << "\n";
						OF << libros[i]->correo;
					}
				}

			}
			
			OF.close();

			if (this->cantidad != 0)
			{
				this->cantidad = 0;
				Captura();
			}

		}

		//funcion que organiza por clave o por proveedor
		void Reportes(int index) {

			Libro *temp;

			switch (index)
			{
			case 1:
				for (int i = 0; i < cantidad - 1; i++)
				{
					for (int j = 0; j < cantidad - i - 1; j++)
					{
						if (libros[j]->clave_libro > libros[j + 1]->clave_libro)
						{
							temp = libros[j];
							libros[j] = libros[j + 1];
							libros[j + 1] = temp;
						}
					}
				}
				break;

			case 2:
				for (int i = 0; i < cantidad - 1; i++)
				{
					for (int j = 0; j < cantidad - i - 1; j++)
					{
						if (strcmp(libros[j]->nombre_prov, libros[j + 1]->nombre_prov) > 0)
						{
							temp = libros[j];
							libros[j] = libros[j + 1];
							libros[j + 1] = temp;
						}

					}
				}
				break;
			}
			
		}

		//funcion para el archivo ordenado por clave
		void Archivoclave() 
		{
			
			std::ofstream OF;
			OF.open("archivos/Clave_libro.txt", ios::out);
			if (OF.is_open()) 
			{
				Reportes(1);
				cout << "Se creo Clave_libro.txt\n";
				for (int i = 0; i < cantidad; i++) 
				{
					OF << libros[i]->clave_libro << "\t" << libros[i]->nombre_libro << "\t" << libros[i]->nombre_autor << "\t" << libros[i]->genero << "\t" << libros[i]->nombre_edit << "\t" << libros[i]->nombre_prov << "\t" << libros[i]->numero_celular << "\t" << libros[i]->correo << "\n";
				}
			}
			else
				cout << "Error en archivo";
			OF.close();
		}

		//funcion para el archivo ordenado por registro
		void Archivoprov() 
		{
			std::ofstream OF{ "archivos/Nombre_prov.txt" };
			if (OF.is_open()) 
			{
				Reportes(2);
				cout << "Se creo Nombre_prov.txt\n";
				for (int i = 0; i < cantidad; i++) 
				{
					OF << libros[i]->clave_libro << "\t" << libros[i]->nombre_libro << "\t" << libros[i]->nombre_autor << "\t" << libros[i]->genero << "\t" << libros[i]->nombre_edit << "\t" << libros[i]->nombre_prov << "\t" << libros[i]->numero_celular << "\t" << libros[i]->correo << "\n";
				}
			}
			else
				cout << "Error en archivo";
			OF.close();
		}

		void debug()
		{
			for (int i = 0; i < cantidad; i++)
			{
				cout << libros[i]->clave_libro << "\n";
				cout << libros[i]->nombre_libro << "\n";
				cout << libros[i]->nombre_autor << "\n";
				cout << libros[i]->genero << "\n";
				cout << libros[i]->nombre_edit << "\n";
				cout << libros[i]->nombre_prov << "\n";
				cout << libros[i]->correo << "\n";
			}
		}

		Libro* buscarPorClave(int clave)
		{
			for (int i = 0; i < cantidad; i++)
			{
				if (libros[i]->clave_libro == clave)
					return libros[i];
			}
			return NULL;
		}

		vector<Libro*> buscarPorProveedor(char proveedor[])
		{
			vector<Libro*> librosReportados;
			for (int i = 0; i < cantidad; i++)
			{
				if (strcmp(libros[i]->nombre_prov, proveedor) == 0)
					librosReportados.push_back(libros[i]);
			}
			return librosReportados;
		}

		Libro** getLibros()
		{
			return libros;
		}

		int getCantidad()
		{
			return cantidad;
		}

		bool validarClave(int clave)
		{
			for (int i = 0; i < cantidad; i++)
			{
				if (libros[i]->clave_libro == clave)
					return true;
			}
			return false;
		}

	};


}

#endif