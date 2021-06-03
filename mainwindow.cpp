#include "mainwindow.h"
#include "setting.h"

/***************Constructeur / Destructeur**************/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);                          // Initialise l'objet de la fenêtre principale

    /**************Affiche une arboressence pour les musique***************/
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs);
    dirmodel->setRootPath(S_Setting.MusicPath);
    ui->treeView->setModel(dirmodel);

    /***************Filtre les musiques par rapport au format définit ***************/
    const QStringList ListFiltre = {FiltreMusique};
    dirmodel->setNameFilters(ListFiltre);
    dirmodel->setNameFilterDisables(false);

    /********** Cache toutes les colonnes pour afficher que la colonne "Name" ********/
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
    ui->treeView->setColumnWidth(0,250);        // Augmente la taille de al colum name

    ui->progressBar->setValue(0);               // Mise à 0 de la barre de progression
    setWindowTitle(NameMainWindows);            // Nomme la fenêtre
    StartSetting();                             // Récupère les paramères de l'application


    /****************** Formulation du protocole de test **********************/
    S_Setting.ProtocolMessageTest.clear();
    S_Setting.ProtocolMessageTest.append(S_Setting.QSProtocolStart);             // Ecriture du début du message
    S_Setting.ProtocolMessageTest.append(S_Setting.QSProtocolMessageTest);       // Ecriture du message de test
    S_Setting.ProtocolMessageTest.append(S_Setting.QSProtocolEnd);               // Ecriture de la fin du message
    S_Setting.ProtocolMessageTest.append("\n");                                  // Fin de ligne


    /****************** Formulation du protocole Haut-Parleur **********************/
    S_Setting.ProtocolMessageHautParleur.clear();
    S_Setting.ProtocolMessageHautParleur.append(S_Setting.QSProtocolStart);      // Ecriture du début du message
    S_Setting.ProtocolMessageHautParleur.append("0");                           // Ecriture de la difficulté
    S_Setting.ProtocolMessageHautParleur.append(S_Setting.QSProtocolSeparate);   // Ecriture du séparateur
    S_Setting.ProtocolMessageHautParleur.append("0");                             // Ecriture du nom de la musique
    S_Setting.ProtocolMessageHautParleur.append(S_Setting.QSProtocolSeparate);   // Ecriture du séparateur
    S_Setting.ProtocolMessageHautParleur.append("001");                           // Ecriture de la zone de mise à jour
    S_Setting.ProtocolMessageHautParleur.append(S_Setting.QSProtocolSeparate);   // Ecriture du séparateur
    S_Setting.ProtocolMessageHautParleur.append(S_Setting.QSProtocolEnd);        // Ecriture de la fin du message
    S_Setting.ProtocolMessageHautParleur.append("\n");                            // Fin de ligne

    /****************** Formulation du protocole Haut-Parleur **********************/
    S_Setting.ProtocolMessageEcran.clear();
    S_Setting.ProtocolMessageEcran.append(S_Setting.QSProtocolStart);      // Ecriture du début du message
    S_Setting.ProtocolMessageEcran.append("0");                             // Ecriture de la difficulté
    S_Setting.ProtocolMessageEcran.append(S_Setting.QSProtocolSeparate);   // Ecriture du séparateur
    S_Setting.ProtocolMessageEcran.append("0");                             // Ecriture du nom de la musique
    S_Setting.ProtocolMessageEcran.append(S_Setting.QSProtocolSeparate);   // Ecriture du séparateur
    S_Setting.ProtocolMessageEcran.append("002");                           // Ecriture de la zone de mise à jour
    S_Setting.ProtocolMessageEcran.append(S_Setting.QSProtocolSeparate);   // Ecriture du séparateur
    S_Setting.ProtocolMessageEcran.append(S_Setting.QSProtocolEnd);        // Ecriture de la fin du message
    S_Setting.ProtocolMessageEcran.append("\n");                            // Fin de ligne
}
MainWindow::~MainWindow()
{
    delete ui;
}

/********** Adapte un chemin lambda en chemin compréhensible par l'application ********/
QString cheminMachine(QString CheminStandard, bool P_FileOrDir)
{
    /******** Remplace tous les slash et backslash par des virgules *********/
    CheminStandard.replace("/", ",",Qt::CaseInsensitive);
    CheminStandard.replace("\\", ",",Qt::CaseInsensitive);
    CheminStandard.replace(",,", ",",Qt::CaseInsensitive);

    /******** Remplace les virgules par un double slash *********/
    CheminStandard.replace(",", "\\\\",Qt::CaseInsensitive);

    /******** Si le paramètre indique un dossier, un double slash est placé à la fin *********/
    if(P_FileOrDir == Dossier && !CheminStandard.endsWith("\\\\",Qt::CaseSensitive))
        CheminStandard.append("\\\\");

    return CheminStandard;
}


