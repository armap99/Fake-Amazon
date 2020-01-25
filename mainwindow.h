#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <algorithm>
#include <QMessageBox>
#include<QRegularExpressionValidator>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include <queue>
//Grafo
#include <QDebug>
#include <QHash> //map o equivalante a un diccionario
//JavaScriptObjectNotation
#include <QFile>
#include "user.h"
#include "product.h"
#include "productform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_emailLE_textChanged(const QString &arg1);

    void on_passwordLE_textChanged(const QString &arg1);

    void on_createPB_clicked();

    void on_newUserPB_clicked();

    void on_newNameLE_textChanged(const QString &arg1);

    void on_newMailLE_textChanged(const QString &arg1);

    void on_newPasswordLE_textChanged(const QString &arg1);

    void on_newPasswordLEC_textChanged(const QString &arg1);

    void on_loginPB_clicked();

    void openFile();

    void on_TipoBox_currentIndexChanged(int index);

    void on_OrdenBox_currentIndexChanged(int index);

    void on_lineEditBuscar_textChanged(const QString &arg1);

    //Compras
    void Comprar(int,QString);

private:
    Ui::MainWindow *ui;

/*Grafo*/
    QHash<QString, QHash<QString, int> > grafo;

/*Para Mostra en ventana */
    std::vector<User> Users;
    std::vector<Product> Inventory;
    std::vector<Product> CInventory;
    std::priority_queue <Product,std::vector<Product>,Product::comparador> MasComprado;

/*Manejo Archivos*/
    QMenu* fileMenu;
    QAction* openFileAction;
    QString fileName;
    QJsonArray UsersJsonArray;
    QJsonArray ProductJsonArray;
    QFile dbFile;
    QJsonObject CarroDeCompras;

/*Para las compras*/
    User Comprador;
    QString Fecha;

    //Archivos
    void loadDB();
    void saveDB();
    void enableLoginPB();
    void enableCreatePB();

    //Mostrar
    void setTiendita();

    //Grafo
    void LlenarGrafo();

};

#endif // MAINWINDOW_H
