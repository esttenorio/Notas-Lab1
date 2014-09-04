#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>

#include <cppconn/statement.h>
#include <cppconn/exception.h>

using namespace std;

void readCSV(istream &input, vector< vector< string > > &output)
{ // inicio de readCSV
	string csvLine;

	while( getline(input, csvLine) )                        // lee cada linea del archivo
	{ // inicio del while que lee lineas

		istringstream csvStream(csvLine);
		vector<string> csvColumn;
		string csvElement;

		while( std::getline(csvStream, csvElement, ',') )   // lee cada elemento y separa elementos cuando hay comas
		{ // inicio del while que lee elementos             // y conforme lo hace guarda los elementos en un vector
			csvColumn.push_back(csvElement);
		} // fin del while que lee elementos
		output.push_back(csvColumn);

	} // fin del while que lee lineas
} // fin de readCSV

int main()
{

//Seccion de lectura del archivo .csv

	fstream file("notas.csv", ios::in);
	if(!file.is_open())
	{
		cout << "Archivo no encontrado!\n";
		return 1;
	}

	typedef vector< vector< string> > csvVector;    // vector de vectores con strings adentro
	csvVector csvData;

	readCSV(file, csvData);                         // se lee el archivo y se guarda todo en el vector
                                                    // de vectores csvData
	string Notas[csvData.size()][10];               // creacion de nuevo vector para guardar todo
	int x = 0;
	int y = 0;

// Seccion de guardado de la informacion del .csv en un string
	for(csvVector::iterator i = csvData.begin(); i != csvData.end(); ++i)
	{
		for(vector< string>::iterator j = i->begin(); j != i->end(); ++j)
		{
			Notas[x][y] = *j;
            //cout << Notas[x][y] << ", ";

            y++;
            if(y==10) y = 0;
            else;

        }
		cout << "\n";
		x++;
	}

// Seccion de conexion con MySQL

    sql::Driver *driver;
    sql::Connection *connection;
    sql::Statement *statement;
    sql::ResultSet *resultset;

    // Nos conectamos a nuestro servidor MySQL
    driver = get_driver_instance();
    connection = driver->connect("tcp://127.0.0.1:3306", "root", "claveS123");

    // Elegimos una base datos que ya tengamos creada ("prueba")
    connection->setSchema("notas");
    statement = connection->createStatement();
    statement->execute("DROP TABLE IF EXISTS registro");    // borra tablas ya existentes en caso que ya hayan
    statement->execute("CREATE TABLE registro(num INT NOT NULL PRIMARY KEY AUTO_INCREMENT, carne CHAR(6), apellido1 CHAR(20), apellido2 CHAR(20), nombre CHAR(20), recinto INT(2), ano INT(4), periodo INT(1), grupo INT(3), sigla CHAR(6), nota CHAR(3))"); // Contenidos de la base de datos

    for(int m = 1; m < 101 ; m++){   // csvData.size()
   statement->execute(string("INSERT INTO registro (carne, apellido1, apellido2, nombre, recinto, ano, periodo, grupo, sigla, nota) VALUES ('" + Notas[m][0] + "','" + Notas[m][1] + "','" + Notas[m][2] + "','" + Notas[m][3] + "','" + Notas[m][4] + "','" + Notas[m][5] + "','" + Notas[m][6] + "','" + Notas[m][7] + "','"+ Notas[m][8] +"','" + Notas[m][9] + "')"));
    } // fin del for ext

    string cursoParticular;
    resultset = statement->executeQuery("SELECT DISTINCT(sigla) AS nota FROM registro ORDER BY nota DESC");
     while (resultset->next()) {
        std::cout << " Sigla: " << resultset->getString(1);
        std::cout << "\n Nota: " << resultset->getString(2);
        std::cout << std::endl;
    }

    delete resultset;
    delete statement;
    delete connection;
}
