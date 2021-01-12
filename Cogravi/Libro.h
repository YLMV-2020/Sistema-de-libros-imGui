#ifndef LIBRO_HEADER
#define LIBRO_HEADER

namespace Equipo7
{
	class Libro
	{

	private:

	public:

		int clave_libro;
		char nombre_libro[30];
		char nombre_autor[30];
		char genero[30];
		char nombre_edit[30];
		char nombre_prov[30];
		char numero_celular[15];
		char correo[30];

		Libro()
		{
			this->clave_libro = 0;
			strcpy(this->nombre_libro, "");
			strcpy(this->nombre_autor, "");
			strcpy(this->genero, "");
			strcpy(this->nombre_edit, "");
			strcpy(this->nombre_prov, "");
			strcpy(this->numero_celular, "");
			strcpy(this->correo, "");
		}

		Libro(int clave_libro, const char nombre_libro[30], const char nombre_autor[30], char genero[30], const char nombre_edit[30], const char nombre_prov[30], const char numero_celular[15], const char correo[30])
		{
			this->clave_libro = clave_libro;
			strcpy(this->nombre_libro, nombre_libro);
			strcpy(this->nombre_autor, nombre_autor);
			strcpy(this->genero, genero);
			strcpy(this->nombre_edit, nombre_edit);
			strcpy(this->nombre_prov, nombre_prov);
			strcpy(this->numero_celular, numero_celular);
			strcpy(this->correo, correo );
		}

		bool validarClave()
		{
			return clave_libro > 0;
		}

		bool validarNombreLibro()
		{
			return strlen(nombre_libro) <= 30;
		}

		bool validarNombreProveedor()
		{
			bool isAlpha = true;
			int T = strlen(nombre_prov);

			for (int i = 0; i < T; i++)
			{
				if (!isalpha(nombre_prov[i]))
				{
					isAlpha = false;
					break;
				}
			}
			return strlen(nombre_prov) <= 30 && isAlpha;
		}

		bool validarNumeroCelular()
		{
			bool isDigit = true;
			int T = strlen(numero_celular);

			for (int i = 0; i < T; i++)
			{
				if (!isdigit(numero_celular[i]))
				{
					isDigit = false;
					break;
				}
			}

			return T == 10 && isDigit;
		}

		bool validarCorreo()
		{
			bool bandera = false;
			bool isArroba = false;
			bool isCom = false;

			int T = strlen(correo);
			string temp;

			for (int i = 0; i < T; i++)
			{
				if (correo[i] == '@') isArroba = true;
				if (correo[i] == '.') temp = "";
			
				temp += correo[i];

				if (temp == ".com")
				{
					isCom = true;
				}	
			}

			return T <= 30 && isArroba && isCom;
		}

		char* getNombre()
		{
			return nombre_libro;
		}

	};
	
}

#endif