#include "prestartgame.h"

/***************Constructeur / Destructeur**************/
PreStartGame::PreStartGame(QWidget *parent) : QDialog(parent), ui(new Ui::PreStartGame)
{
    ui->setupUi(this);                          // Initialise l'objet de la fenêtre "PreStartGame"

    /************** Affiche une arboressence pour les musique ***************/
    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files );

    /*************** Filtre les musiques par rapport au format définit ***************/
    QStringList ListFiltre = {FiltreMusique};
    filemodel->setNameFilters(ListFiltre);
    filemodel->setNameFilterDisables(false);

    setWindowTitle(NamePrestartWindows);        // Nomme la fenêtre
}
PreStartGame::~PreStartGame()
{
    delete ui;
}

/************* Affiche la progression de la carte contrôleur **********/
void PreStartGame::AfficheProgress(QString P_Texte, const char* P_Color)
{
    /******** Affiche un message d'état en couleur ********/
    QString MessageColore;
    MessageColore="<span style=\"color: " + QString(P_Color) + "\">"+P_Texte+"</span>";
    ui->textEditProgress->clear();
    ui->textEditProgress->append(MessageColore);
}

/****************** Filtre l'arborescence pour afficher que les musiques **********************/
void PreStartGame::FiltreMusic(QString CheminCard)
{
    /********** Filtre l'affichage en affichant le contenue du fichier "Music" **********/
    ui->ListMusic->setModel(filemodel);
    CheminCard.append("Music");
    ui->ListMusic->setRootIndex(filemodel->setRootPath(CheminCard));
}

/********** Fonction éxectuté lorsque l'application reçoit un message du serveur **********/
void PreStartGame::MessageServeur()
{
    QByteArray      ProtocolMessageMusique          ;
    char            ReponseServeur[200]             ;

    /****************** Formulation du message Musique **********************/
    ProtocolMessageMusique.append(S_Setting->QSProtocolStart);          // Ecriture du début du message
    if(ui->RadioFacile->isChecked())                                    // Ecriture de la difficulté
        ProtocolMessageMusique.append("1");
    if(ui->RadioNormal->isChecked())
        ProtocolMessageMusique.append("2");
    if(ui->RadioDifficile->isChecked())
        ProtocolMessageMusique.append("3");
    ProtocolMessageMusique.append(S_Setting->QSProtocolSeparate);       // Ecriture du séparateur
    ProtocolMessageMusique.append(S_Setting->MusicName);                // Ecriture du nom de la musique
    ProtocolMessageMusique.append(S_Setting->QSProtocolSeparate);       // Ecriture du séparateur
    ProtocolMessageMusique.append(S_Setting->QSProtocolBufferCom);      // Ecriture de la zone de mise à jour
    ProtocolMessageMusique.append(S_Setting->QSProtocolSeparate);       // Ecriture du séparateur
    ProtocolMessageMusique.append(S_Setting->QSProtocolEnd);            // Ecriture de la fin du message
    ProtocolMessageMusique.append("\n");                                // Fin de ligne

    /****************** Vérification de la réception du message de test **********************/
    if(prestart_socket->read(ReponseServeur,200))
    {
        /******** Compare le message retour avec le protocole de test ************/
        if(strncmp(ReponseServeur,S_Setting->ProtocolMessageTest.data(),static_cast<size_t>(S_Setting->ProtocolMessageTest.size())) == 0)
            AfficheProgress("Connexion avec la carte réussi","GREEN");

        /******** Compare le message retour avec le message Musique ************/
        if(strncmp(ReponseServeur,ProtocolMessageMusique.data(),static_cast<size_t>(ProtocolMessageMusique.size())) == 0)
            AfficheProgress("Lancement de la partie","GREEN");

        /******** Compare le message retour avec le protocole haut-parleur ************/
        if(strncmp(ReponseServeur,S_Setting->ProtocolMessageHautParleur.data(),static_cast<size_t>(S_Setting->ProtocolMessageHautParleur.size())) == 0)
            S_Setting->EtatSpeaker = "true";

        /******** Compare le message retour avec le protocole de contact ************/
        if(strncmp(ReponseServeur,S_Setting->ProtocolMessageEcran.data(),static_cast<size_t>(S_Setting->ProtocolMessageEcran.size())) == 0)
            S_Setting->EtatEcran = "true";
    }
    else
        AfficheProgress("La carte ne répond plus...\nVerrifier la connexion...","RED");
}

/****************** Lance une partie **********************/
bool PreStartGame::LancemenJeu()
{
    bool        resultat            =   false   ;
    QByteArray  ProtocolMessageMusique          ;

    /****************** Formulation du message Musique **********************/
    ProtocolMessageMusique.append(S_Setting->QSProtocolStart);          // Ecriture du début du message
    if(ui->RadioFacile->isChecked())                                    // Ecriture de la difficulté
        ProtocolMessageMusique.append("1");
    if(ui->RadioNormal->isChecked())
        ProtocolMessageMusique.append("2");
    if(ui->RadioDifficile->isChecked())
        ProtocolMessageMusique.append("3");
    ProtocolMessageMusique.append(S_Setting->QSProtocolSeparate);       // Ecriture du séparateur
    ProtocolMessageMusique.append(S_Setting->MusicName);                // Ecriture du nom de la musique
    ProtocolMessageMusique.append(S_Setting->QSProtocolSeparate);       // Ecriture du séparateur
    ProtocolMessageMusique.append(S_Setting->QSProtocolBufferCom);      // Ecriture de la zone de mise à jour
    ProtocolMessageMusique.append(S_Setting->QSProtocolSeparate);       // Ecriture du séparateur
    ProtocolMessageMusique.append(S_Setting->QSProtocolEnd);            // Ecriture de la fin du message
    ProtocolMessageMusique.append("\n");                                // Fin de ligne

    /******** Envoie le message au serveur************/
    if(prestart_socket->write(ProtocolMessageMusique) != 0)
    {
        AfficheProgress("Lancement de la séance en cour...\Attente d'une réponse du serveur","GREEN");
        resultat = true;
    }

     return resultat;
}

/************************************************************ SLOTS ************************************************************************/

/************* Bouton "Lancer la partie" **********/
void PreStartGame::on_StartGame_clicked()
{
    /*********** Connexion de la fonction "MessageServeur" au SLOT "readyReady" **********/
    connect(prestart_socket, SIGNAL(readyRead()), this, SLOT(MessageServeur()));

    /************* Si une musique est sélectionné **********/
    if(S_Setting->MusicName != "")
    {
        /************* Si une difficulté est sélectionné **********/
        if( ui->RadioFacile->isChecked() ||ui->RadioNormal->isChecked() || ui->RadioDifficile->isChecked() )
            LancemenJeu();                  // Lance une partie
        else
            AfficheProgress("Aucune difficulte n'est selectionne","RED");
    }
    else
        AfficheProgress("Aucune musique n'est selectionne","RED");
}

/****************** Récupère le nom de la musique sélectionné et l'affiche **********************/
void PreStartGame::on_ListMusic_doubleClicked(const QModelIndex &index)
{
    S_Setting->MusicName = dirmodel->fileName(index);
    ui->textEditChemin->clear();
    ui->textEditChemin->append(S_Setting->MusicName);
}
void PreStartGame::on_ListMusic_clicked(const QModelIndex &index)
{
    on_ListMusic_doubleClicked(index);          // En cas de simple ou de double cliques, on exécute la même fonction
}
