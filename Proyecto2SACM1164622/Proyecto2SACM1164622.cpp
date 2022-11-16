#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "List.h"
#include "LinkedList.h"
#include "Sticker.h"
#include "Category.h"
#include "AlbumReader.h"
using namespace std;

AlbumReader albumReader;
string sourceFile;
List<Sticker>* listaAlbum;
vector<string> listaCodigos;

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void menuMarcado();
void menuPrincipal();
int doesStickerExist(string input);
void menuFiltrado(string tipeFilter, string repetidasOpt);
void menuBusqueda();

void marcarListaCodigos() 
{
    int marking;
    for (int i = 0; i < listaCodigos.size(); i++) 
    {
        marking = doesStickerExist(listaCodigos[i]);
        if (marking >= 0) 
        {
            listaAlbum->get(marking)->mark();
        }
    }
}

void desMarcarListaCodigos()
{
    int marking;
    for (int i = 0; i < listaCodigos.size(); i++)
    {
        marking = doesStickerExist(listaCodigos[i]);
        if (marking >= 0)
        {
            listaAlbum->get(marking)->unmark();
        }
    }
}

void leerArchivoCodigos(string ubicacionDelArchivo) 
{
    fstream file(ubicacionDelArchivo, ios::in);
    string line, word;
    listaCodigos.clear();
    if (file.is_open()) 
    {
        while (getline(file, line)) 
        {
            stringstream str(line);

            while (getline(str,word,','))
            {
                listaCodigos.push_back(word);
            }
        }
    }
}

int doesStickerExist(string input)
{
    
    for (int i = 0; i < listaAlbum->get_size(); i++)
    {
        if (listaAlbum->get(i)->get_code().compare(input) == 0)
        {
            return i;
        }
    }
    return -1;
}

int doesStickerExistByName(string input)
{

    for (int i = 0; i < listaAlbum->get_size(); i++)
    {
        if (listaAlbum->get(i)->get_name().compare(input) == 0)
        {
            return i;
        }
    }
    return -1;
}


int contarFaltantes() 
{
    int faltantes = 0;
    for (int i = 0; i < listaAlbum->get_size(); i++) 
    {
        if (!listaAlbum->get(i)->is_marked()) 
        {
            faltantes++;
        }
    }
    return faltantes;
}

int contarMarcadas()
{
    int marcadas = 0;
    for (int i = 0; i < listaAlbum->get_size(); i++)
    {
        if (listaAlbum->get(i)->is_marked())
        {
            marcadas++;
        }
    }
    return marcadas;
}

void menuMarcado() 
{
    string selection;
    string input;
    int select;
    int index;

    do
    {
        system("cls");
        cout << "Menu de marcado y desmarcado de estampas\n";
        cout << "Seleccione una opcion\n";
        cout << "1| Marcar estampas individualmente\n";
        cout << "2| Desmarcar estampas individualmente\n";
        cout << "3| Marcar estampas por archivo\n";
        cout << "4| Desmarcar estampas por archivo\n";
        cout << "0| Regresar\n";

        getline(cin, selection);

        if (is_number(selection))
        {
            select = stoi(selection);
        }
        else { select = 100000000; }

        if (selection == "")
        {
            select = 10;
        }

        switch (select)
        {
        case 1:
            cout << "Ingrese el codigo unico de la estampa a marcar\n";
            getline(cin, input);

            index = doesStickerExist(input);
            if (index>=0) 
            {
                listaAlbum->get(index)->mark();
                cout << "El sticker se ha marcado existosamente\n";
            }
            else 
            {
                cout << "No existe un sticker con ese codigo en el album\n";
            }

            cout << "Presione una tecla para continuar\n";
            getwchar();

            break;
        case 2:
            cout << "Ingrese el codigo unico de la estampa a desmarcar\n";
            getline(cin, input);

            index = doesStickerExist(input);
            if (index >= 0)
            {
                listaAlbum->get(index)->unmark();
                cout << "El sticker se ha desmarcado existosamente\n";
            }
            else
            {
                cout << "No existe un sticker con ese codigo en el album\n";
            }

            cout << "Presione una tecla para continuar\n";
            getwchar();
            break;
        case 3:
            cout << "Ingrese la direccion del archivo con las estampas a desmarcar\n";
            cout << "EL archivo debe de contener los codigos de las estamas en el siguiente formato:\n";
            cout << "ARG1,ARG2,FWC0,BRA1,ARG3,FWC3,ARG3,ARG3,BRA1 (En una sola linea)\n";

            getline(cin, input);
            leerArchivoCodigos(input);
            marcarListaCodigos();

            cout << "Los codigos ingresados se han marcado correctamente\n";
            cout << "Presione una tecla para continuar\n";
            getwchar();
            break;
        case 4:
            cout << "Ingrese la direccion del archivo con las estampas a desmarcar\n";
            cout << "EL archivo debe de contener los codigos de las estamas en el siguiente formato:\n";
            cout << "ARG1,ARG2,FWC0,BRA1,ARG3,FWC3,ARG3,ARG3,BRA1 (En una sola linea)\n";

            getline(cin, input);
            leerArchivoCodigos(input);
            desMarcarListaCodigos();

            cout << "Los codigos ingresados se han desmarcado correctamente\n";
            cout << "Presione una tecla para continuar\n";
            getwchar();
            break;
        case 0:
            return;
        default:
            break;
        }
    } while (select != 0);
}

string getLineWrite(Sticker* looking) 
{
    string code = looking->get_code();
    string marcada = "No";
    if (looking->is_marked()) 
    {
        marcada = "Si";
    }
    int apariciones = looking->get_appeareances() - 1;
    if (apariciones < 0) 
    {
        apariciones = 0;
    }
    string result = code + "," + looking->get_name() + "," + marcada + "," + to_string(apariciones) + "\n";
    return result;
}

void guardarEnArchivo(string direccionArchivo, string toSave) 
{
    ofstream archivo;
    archivo.open(direccionArchivo);
    archivo << toSave;
    archivo.close();
}

string EjecutarFiltrado(string tipeFilter, string repetidasOpt) 
{
    string result = "Codigo,Nombre,Marcada,Repeticiones\n";
    if (tipeFilter.compare("Ninguno") != 0 && repetidasOpt.compare("Si")==0)
    {
        for (int i = 0; i < listaAlbum->get_size(); i++) 
        {
            Sticker* looking = listaAlbum->get(i);
            if (looking->get_category()->get_code().compare(tipeFilter) == 0 && looking->is_repeated()) 
            {
                result += getLineWrite(looking);
            }
        }
        
    }
    else if (tipeFilter.compare("Ninguno") != 0) 
    {
        for (int i = 0; i < listaAlbum->get_size(); i++)
        {
            Sticker* looking = listaAlbum->get(i);
            if (looking->get_category()->get_code().compare(tipeFilter) == 0)
            {
                result += getLineWrite(looking);
            }
        }
    }
    else if (repetidasOpt.compare("Si") == 0)
    {
        for (int i = 0; i < listaAlbum->get_size(); i++)
        {
            Sticker* looking = listaAlbum->get(i);
            if (looking->is_repeated())
            {
                result += getLineWrite(looking);
            }
        }
    }
    return result;
}

void menuFiltrado(string tipeFilter, string repetidasOpt) 
{
    string filter = tipeFilter;
    string reps = repetidasOpt;
    string selection, input, archv;
    int select;
    string searchResult;

    do
    {

        system("cls");
        cout << "Menu de filtrado\n";
        cout << "Seleccione una opcion\n";
        cout << "1| Filtrar por tipo/seleccion: " <<filter<< "\n";
        cout << "2| Filtrar por repetidas: " <<reps <<"\n";
        cout << "3| Ejecutar filtrado\n";
        cout << "0| Regresar\n";

        getline(cin, selection);

        if (is_number(selection))
        {
            select = stoi(selection);
        }
        else { select = 100000000; }

        if (selection == "")
        {
            select = 10;
        }

        switch (select)
        {
        case 1:
            cout << "Ingrese el codigo de la categoria que desea filtrar\n";
            getline(cin, filter);
            break;
        case 2:
            if (reps.compare("Si") == 0) 
            {
                reps = "No";
            }
            else if (reps.compare("No") == 0)
            {
                reps = "Si";
            }
            break;
        case 3:
            searchResult = EjecutarFiltrado(filter, reps);
            cout << searchResult;
            cout << "Presione una tecla para continuar\n";
            getwchar();

            cout << "Desea guardar el resultado a un archivo csv?\n";
            cout << "1| Si\n";
            cout << "0| No\n";

            getline(cin, input);

            if (input.compare("1") == 0) 
            {
                cout << "Ingrese la ubicacion del archivo\n";
                getline(cin, archv);
                guardarEnArchivo(archv, searchResult);
                
                cout << "El resultado de busqueda se ha guardado exitosamente";
                cout << "Presione una tecla para continuar\n";
                getwchar();
            }
            
            break;
        case 0:
            menuPrincipal();
            return;
        default:
            break;
        }
    } while (select != 0);
}

void menuBusqueda()
{
    string selection, input, result;
    int select, index;

    do
    {
        system("cls");
        cout << "Menu de busqueda\n";
        cout << "Seleccione una opcion\n";
        cout << "1| Buscar estampa por codigo unico\n";
        cout << "2| Buscar estampa por nombre\n";
        cout << "0| Regresar\n";

        getline(cin, selection);

        if (is_number(selection))
        {
            select = stoi(selection);
        }
        else { select = 100000000; }

        if (selection == "")
        {
            select = 10;
        }

        switch (select)
        {
        case 1:
            cout << "Ingrese el codigo unico de la estampa a buscar\n";
            getline(cin, input);
            index = doesStickerExist(input);
            if (index >= 0)
            {
                result = "Codigo,Nombre,Marcada,Repeticiones\n";
                result += getLineWrite(listaAlbum->get(index));
                cout << result;
            }
            else
            {
                cout << "No existe un sticker con ese codigo en el album\n";
            }

            cout << "Presione una tecla para continuar\n";
            getwchar();

            break;
        case 2:
            cout << "Ingrese el nombre de la estampa a buscar\n";
            getline(cin, input);
            index = doesStickerExistByName(input);
            if (index >= 0)
            {
                result = "Codigo,Nombre,Marcada,Repeticiones\n";
                result += getLineWrite(listaAlbum->get(index));
                cout << result;
            }
            else
            {
                cout << "No existe un sticker con ese codigo en el album\n";
            }
            cout << "Presione una tecla para continuar\n";
            getwchar();
            break;
        
        case 0:
            menuPrincipal();
            return;
        default:
            break;
        }
    } while (select != 0);
}

void menuPrincipal()
{
    string selection;
    int select;
    int count;

    do
    {
        system("cls");
        cout << "Menu Principal\n";
        cout << "Seleccione una opcion\n";
        cout << "1| Marcar o Desmarcar estampas\n";
        cout << "2| Mostrar cuantas estampas me faltan\n";
        cout << "3| Mostrar cuantas estampas tengo marcadas\n";
        cout << "4| Filtrar estampas\n";
        cout << "5| Buscar una estampa\n";
        cout << "0| Salir del programa\n";

        getline(cin, selection);

        if (is_number(selection))
        {
            select = stoi(selection);
        }
        else { select = 100000000; }

        if (selection == "")
        {
            select = 10;
        }

        switch (select)
        {
        case 1:
            menuMarcado();
            break;
        case 2:

            count = contarFaltantes();
            if (count > 1) 
            {
                cout << "Te faltan " << count << " estampas\n";
            }
            else if (count == 1)
            {
                cout << "Te falta " << count << " estampa\n";
            }
            else 
            {
                cout << "No te falta ninguna estampa\n";
            }
            
            cout << "Presione una tecla para continuar\n";
            getwchar();
            break;
        case 3:
            count = contarMarcadas();

            if (count > 1) 
            {
                cout << "Tienes " << count << " estampas marcadas\n";
            }
            else if (count == 1) 
            {
                cout << "Tienes " << count << " estampa marcada\n";
            }
            else 
            {
                cout << "No tienes estampas marcadas\n";
            }

            cout << "Presione una tecla para continuar\n";
            getwchar();
            break;
        case 4:
            menuFiltrado("Ninguno", "No");
            break;
        case 5:
            menuBusqueda();
            break;
        case 0:
            exit(0);
            return;
        default:
            break;
        }
    } while (select != 0);
}

int main()
{
    cout << "Proyecto 2 SACM1164622\n";
    cout << "Ingrese la ubicacion del archivo de Album para comenzar\n";
    getline(cin, sourceFile);
    listaAlbum = albumReader.read_album(sourceFile);

    menuPrincipal();
    
}
