#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QImage imagen("Sin título.jpg");
    ui->Logo->setPixmap(QPixmap::fromImage(imagen).scaled(200,200,Qt::KeepAspectRatio));
    fileMenu = ui->menuBar->addMenu(tr("&Archivo"));
    openFileAction = new QAction(tr("Abrir archivo"),this);

    connect(openFileAction, SIGNAL(triggered()),this,SLOT(openFile()));
    fileMenu->addAction(openFileAction);
}

MainWindow::~MainWindow()
{
    saveDB();
    delete ui;
}

void MainWindow::enableLoginPB()
{
    QRegularExpression prueba("^[^@]+@[^@]+.[a-zA-Z]{2,}$");
    QRegularExpressionMatch match = prueba.match(ui->emailLE->text());
    if(match.hasMatch() == true && ui->passwordLE->text().length() > 0){
        ui->ErrorDeCorreo->setText("");
        ui->loginPB->setEnabled(true);
    }
    else {
        ui->loginPB->setEnabled(false);
        ui->ErrorDeCorreo->setText("Datos Invalidos");
    }
}

void MainWindow::enableCreatePB()
{
    QRegularExpression prueba("^[^@]+@[^@]+.[a-zA-Z]{2,}$");
    QRegularExpressionMatch match = prueba.match(ui->newMailLE->text());
    if(ui->newNameLE->text().length() > 0  && ui->newPasswordLE->text().length() > 0 && ui->newPasswordLEC->text().length() > 0 && ui->newPasswordLE->text() == ui->newPasswordLEC->text() && match.hasMatch() == true){
        ui->newError->setText("");
        ui->createPB->setEnabled(true);
    }
    else {
        ui->createPB->setEnabled(false);
        ui->newError->setText("Datos no validos");
    }
}


void MainWindow::on_emailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_passwordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_newUserPB_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_newNameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCreatePB();
}

void MainWindow::on_newMailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCreatePB();
}

void MainWindow::on_newPasswordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCreatePB();
}

void MainWindow::on_newPasswordLEC_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableCreatePB();
}


void MainWindow::on_createPB_clicked()
{
    QMessageBox message;
    size_t i = 0;
    for (;i < Users.size();++i) {
        if(Users.at(i).getEmail() == ui->newMailLE->text()){
            message.setText("El usuario ya existe");
            message.setIcon(QMessageBox::Warning);
            break;
        }
    }
    if(i == Users.size()){
        User u;
        u.setName(ui->newNameLE->text());
        u.setEmail(ui->newMailLE->text());
        u.setPassword(ui->newPasswordLE->text());
        Users.push_back(u);

        QJsonObject NuevoUsuario;
        QJsonArray bought;
        NuevoUsuario["name"] = u.getName();
        NuevoUsuario["email"] = u.getEmail();
        NuevoUsuario["password"] = u.getPassword();
        NuevoUsuario["purchase"] = bought;
        UsersJsonArray.append(NuevoUsuario);

        ui->newNameLE->clear();
        ui->newMailLE->clear();
        ui->newPasswordLE->clear();
        ui->newPasswordLEC->clear();

        ui->stackedWidget->setCurrentIndex(0);


        message.setText("Usuario Ingresado con exito");
        message.setIcon(QMessageBox::Information);
    }
    message.exec();
}

void MainWindow::on_loginPB_clicked()
{
    QMessageBox message;
    size_t i = 0;
    for(;i< Users.size();++i){
        if(Users.at(i).getEmail() == ui->emailLE->text()){
            if(Users.at(i).getPassword() == ui->passwordLE->text()){
                ui->stackedWidget->setCurrentIndex(2);
                Comprador.setName(Users.at(i).getName());
                Comprador.setEmail(Users.at(i).getEmail());
                Comprador.setPassword(Users.at(i).getPassword());
                ui->emailLE->clear();
                ui->passwordLE->clear();
                //
                QString h;
                QJsonArray solucion;
                h = QDate::currentDate().toString("yyyy/MM/dd") + " " + QTime::currentTime().toString("hh:mm:ss") ;
                CarroDeCompras[h] = solucion;
                Fecha = h;
                //
                setTiendita();
                break;
            }
            else{
                message.setText("Usuario o Contraseña equivocada");
                message.setIcon(QMessageBox::Critical);
                message.exec();
            }
        }
    }
    if (i == Users.size()){
        message.setText("El usuario ingresado no existe");
        message.setIcon(QMessageBox::Warning);
        message.exec();
    }
}

//Para Trabajar en Json

void MainWindow::openFile()
{
    fileName = QFileDialog::getOpenFileName(this, "DataBase", "", "Archivos JSON(*.json)");

    if(fileName.length() > 0){
        dbFile.setFileName(fileName);
        loadDB();
    }
}

void MainWindow::loadDB(){
    QByteArray data;
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;

    dbFile.open(QIODevice::ReadOnly);
    data = dbFile.readAll();
    jsonDoc = QJsonDocument(QJsonDocument::fromJson(data));
    jsonObj = jsonDoc.object();
    UsersJsonArray = jsonObj["users"].toArray();
    ProductJsonArray = jsonObj["products"].toArray();
    dbFile.close();
    int i = 0;
    //
    User Recuperado;
    for(;i< UsersJsonArray.size();++i){
        QJsonObject Prueba =  UsersJsonArray[i].toObject();
        Recuperado.setName(Prueba["name"].toString());
        Recuperado.setEmail(Prueba["email"].toString());
        Recuperado.setPassword(Prueba["password"].toString());
        Users.push_back(Recuperado);
        }
    //para productos
    Product Sacado;
    for (int y = 0;y < ProductJsonArray.size();++y) {
        QJsonObject Prod = ProductJsonArray[y].toObject();
        Sacado.setId(Prod["id"].toString());
        Sacado.setName(Prod["name"].toString());
        Sacado.setPrice(Prod["price"].toDouble());
        Sacado.setSold(Prod["sold"].toInt());
        Inventory.push_back(Sacado);
        MasComprado.push(Sacado);
    }
    CInventory = Inventory;
    LlenarGrafo();
}

void MainWindow::saveDB(){
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    //Abrir archivo
    dbFile.open(QIODevice::WriteOnly);
    //Guardando arreglo de objetos
    //Objeto json
    int j = 0;
        while (j < UsersJsonArray.size()) {
            if(UsersJsonArray[j].toObject()["email"].toString() == Comprador.getEmail()){
                QJsonObject NuevoArt;
                QJsonObject o;
                QJsonArray NuevasCompras;
                if(!CarroDeCompras[Fecha].toArray().empty()){
                    o = UsersJsonArray[j].toObject();
                    NuevasCompras = o["purchase"].toArray();
                    NuevasCompras.append(CarroDeCompras);
                    o["purchase"] = NuevasCompras;
                    UsersJsonArray[j] = o;
                }
            }

            j++;
        }

    jsonObj["users"] = UsersJsonArray;
    jsonObj["products"] = ProductJsonArray;
    //creamos un documento json a partir de un objeto
    jsonDoc = QJsonDocument(jsonObj);
    //escribimos en formato json
    dbFile.write(jsonDoc.toJson());
    dbFile.close();
}

void MainWindow::setTiendita()
{
    ProductForm *VentanaMamalona;
    Product Find;
    for (size_t i = 0;i < Inventory.size();i++) {
        Find = Inventory[i];
        VentanaMamalona = new ProductForm(ui->scrollArea);
        VentanaMamalona->SetProduct(Find);
        connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
        ui->gridLayout_5->addWidget(VentanaMamalona,i/2,i%2);
    }
    for (int j = 0;j < 3;j++) {
        Find = MasComprado.top();
        MasComprado.pop();
        VentanaMamalona = new ProductForm(ui->scrollDestacados);
        VentanaMamalona->SetProduct(Find);
        connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
        ui->gridLayoutNuevo->addWidget(VentanaMamalona,j/3,j%3);
    }
}



void MainWindow::on_TipoBox_currentIndexChanged(int index)
{
    ui->scrollArea->scroll(0,0);
    if(index >= 0){
        QLayoutItem *Eliminar;
        while ((Eliminar = ui->gridLayout_5->takeAt(0)) != NULL) {
            delete Eliminar->widget();
            delete Eliminar;
        }
    }
    if(index == 0){
        ProductForm *VentanaMamalona;
        int k = 0;
        Product Find;
        for (size_t i = 0;i < Inventory.size();i++) {
            Find = Inventory[i];
            if((Find.getName().contains(ui->lineEditBuscar->text(),Qt::CaseInsensitive)) or ui->lineEditBuscar->text().length() == 0){
                VentanaMamalona = new ProductForm(ui->scrollArea);
                VentanaMamalona->SetProduct(Find);
                connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
                ui->gridLayout_5->addWidget(VentanaMamalona,k/2,k%2);
                k++;
            }
        }
    }
    if(index == 1){
        ProductForm *VentanaMamalona;
        int k = 0;
        Product Find;
        for (size_t i = 0;i < Inventory.size();i++) {
            Find = Inventory[i];
            if(Find.getId()[0] == 'A' and ((Find.getName().contains(ui->lineEditBuscar->text(),Qt::CaseInsensitive)) or ui->lineEditBuscar->text().length() == 0)){
                VentanaMamalona = new ProductForm(ui->scrollArea);
                VentanaMamalona->SetProduct(Find);
                connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
                ui->gridLayout_5->addWidget(VentanaMamalona,k/2,k%2);
                k++;
            }
        }
    }
    if(index == 2){
        ProductForm *VentanaMamalona;
        int k = 0;
        Product Find;
        for (size_t i = 0;i < Inventory.size();i++) {
            Find = Inventory[i];
            if(Find.getId()[0] == 'D' and ((Find.getName().contains(ui->lineEditBuscar->text(),Qt::CaseInsensitive)) or ui->lineEditBuscar->text().length() == 0)){
                VentanaMamalona = new ProductForm(ui->scrollArea);
                VentanaMamalona->SetProduct(Find);
                connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
                ui->gridLayout_5->addWidget(VentanaMamalona,k/2,k%2);
                k++;
            }
        }
    }
    if(index == 3){
        ProductForm *VentanaMamalona;
        int k = 0;
        Product Find;
        for (size_t i = 0;i < Inventory.size();i++) {
            Find = Inventory[i];
            if(Find.getId()[0] == 'E' and ((Find.getName().contains(ui->lineEditBuscar->text(),Qt::CaseInsensitive)) or ui->lineEditBuscar->text().length() == 0)){
                VentanaMamalona = new ProductForm(ui->scrollArea);
                VentanaMamalona->SetProduct(Find);
                connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
                ui->gridLayout_5->addWidget(VentanaMamalona,k/2,k%2);
                k++;
            }
        }
    }
    if(index == 4){
        ProductForm *VentanaMamalona;
        int k = 0;
        Product Find;
        for (size_t i = 0;i < Inventory.size();i++) {
            Find = Inventory[i];
            if(Find.getId()[0] == 'H' and ((Find.getName().contains(ui->lineEditBuscar->text(),Qt::CaseInsensitive)) or ui->lineEditBuscar->text().length() == 0)){
                VentanaMamalona = new ProductForm(ui->scrollArea);
                VentanaMamalona->SetProduct(Find);
                connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
                ui->gridLayout_5->addWidget(VentanaMamalona,k/2,k%2);
                k++;
            }
        }
    }
    if(index == 5){
        ProductForm *VentanaMamalona;
        int k = 0;
        Product Find;
        for (size_t i = 0;i < Inventory.size();i++) {
            Find = Inventory[i];
            if(Find.getId()[0] == 'L' and ((Find.getName().contains(ui->lineEditBuscar->text(),Qt::CaseInsensitive)) or ui->lineEditBuscar->text().length() == 0)){
                VentanaMamalona = new ProductForm(ui->scrollArea);
                VentanaMamalona->SetProduct(Find);
                connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
                ui->gridLayout_5->addWidget(VentanaMamalona,k/2,k%2);
                k++;
            }
        }
    }
}

void MainWindow::on_OrdenBox_currentIndexChanged(int index)
{
    if(index == 0){
        Inventory = CInventory;
        on_TipoBox_currentIndexChanged(ui->TipoBox->currentIndex());
    }
    if(index == 1){
        sort(Inventory.begin(),Inventory.end(),std::greater<Product>());
        on_TipoBox_currentIndexChanged(ui->TipoBox->currentIndex());
    }
    if(index == 2){
        sort(Inventory.begin(),Inventory.end());
        on_TipoBox_currentIndexChanged(ui->TipoBox->currentIndex());
    }
}

void MainWindow::on_lineEditBuscar_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    on_TipoBox_currentIndexChanged(ui->TipoBox->currentIndex());
}

void MainWindow::Comprar(int pruebas,QString desComprado)
{
    int i = 0;
    while (i <= ProductJsonArray.size()) {
        if(ProductJsonArray[i].toObject()["id"].toString() == desComprado){
            QJsonObject otro;
            otro["id"] = ProductJsonArray[i].toObject()["id"].toString();
            otro["name"] = ProductJsonArray[i].toObject()["name"].toString();
            otro["price"] = ProductJsonArray[i].toObject()["price"];
            otro["sold"] = ProductJsonArray[i].toObject()["sold"].toInt() + pruebas;
            ProductJsonArray[i] = otro;
            break;
        }
        i++;
    }
    QJsonObject o;
    QJsonArray h;
    o["id"] = desComprado;
    h = CarroDeCompras[Fecha].toArray();
    h.append(o);
    CarroDeCompras[Fecha] = h;
    //limpiar ventanas
    QLayoutItem *Eliminar;
    while ((Eliminar = ui->gridLayoutNuevo->takeAt(0)) != NULL) {
        delete Eliminar->widget();
        delete Eliminar;
    }
    //Mostra tambien comprado
    ProductForm *VentanaMamalona;
    Product Find;
        QHash<QString, int > aristas;
        aristas = grafo[desComprado];
        QString id;
        int menor = 0;
        for (int j = 0;j < 3;) {
            id = "";
            menor = 0;
                QHash<QString,int>::iterator origen;
                origen = aristas.begin();
                while (origen != aristas.end()) {
                    if(origen.value() >= menor){
                        id = origen.key();
                        menor = origen.value();
                    }
                    qDebug() << id
                             << ":" << menor;
                    ++origen;
                }
                aristas.remove(id);
                for (int i = 0;i < Inventory.size();i++) {
                    if(Inventory[i].getId() == id){
                        Find = Inventory[i];
                        VentanaMamalona = new ProductForm(ui->scrollDestacados);
                        VentanaMamalona->SetProduct(Find);
                        connect(VentanaMamalona, SIGNAL(valueChanged(int,QString)), this, SLOT(Comprar(int,QString)));
                        ui->gridLayoutNuevo->addWidget(VentanaMamalona,j/3,j%3);
                        break;
                    }
                }
                j++;
                qDebug() << endl;
        }
}

void MainWindow::LlenarGrafo()
{
    //sacar id de cada compra
    QJsonObject Obj,Prod,json;
    QJsonArray grafoJsonArray,prod;
    QJsonObject::iterator iterador;
    QString idCompra;
    for (int i = 0;i < UsersJsonArray.size();i++) {//sacar compras de cada usuario
        std::vector<QString> vector;
        Obj = UsersJsonArray.at(i).toObject();
        grafoJsonArray = Obj["purchase"].toArray();

        for (int j = 0;j < grafoJsonArray.size();j++) {
            Prod = grafoJsonArray.at(j).toObject();
            iterador = Prod.begin();

            while (iterador != Prod.end()) {
                idCompra = iterador.key();
                prod = Prod[idCompra].toArray();

                for (int k = 0;k < prod.size();k++) {
                    json = prod.at(k).toObject();
                    vector.push_back(json["id"].toString());
                }
                iterador++;
                //Llenado de grafo
                QHash<QString, int > aristas;
                QHash<QString, int > aristasRep;
                for (size_t m = 0;m < vector.size();m++) {//recorrer todos los productos de una compra
                    for (size_t n = 0;n < vector.size();n++) {//conectar todos con todos
                        if(m != n and vector.size() > 1){//menos con el mismo y si solo es un producto
                            aristas.insert(vector.at(n),1);
                        }
                    }
                    if(grafo.contains(vector.at(m))){//si ya esta aumentar pesos
                        aristasRep = grafo[vector.at(m)];
                        QHash<QString,int>::iterator origen;
                        origen = aristasRep.begin();
                        while (origen != aristasRep.end()) {//aumento de peso a la conexiones ya
                            //origen.value() = origen.value() + 1;
                            ++origen;
                        }
                        origen = aristas.begin();
                        while (origen != aristas.end()) {//verificar si las otras compras ya estaban para no agregarlas
                                                         //y solo aumentar peso
                            if(aristasRep.contains(origen.key())){//si los nuevos tambien estan les agrega uno a su conexion
                                aristasRep.find(origen.key()).value() += 1;
                            }
                            else {//si no los agrega a la lista
                                aristasRep.insert(origen.key(),origen.value());
                            }
                            ++origen;
                        }
                        grafo[vector.at(m)].swap(aristasRep);//cambia las viejas conexiones con las nuevas
                    }
                    else {//si no esta agegar al grafo
                        if(grafo.empty()){//si esta vacio solo agregar no hya con que conectar
                            grafo.insert(vector.at(m),aristas);
                        }
                        else {
                            grafo.insert(vector[m],aristas);//agrgar grafo
                        }
                    }
                    aristas.clear();//limpiar aristas
                }
                vector.clear();
            }
        }
    }
}
