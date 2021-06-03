#include "card.h"
#include "windows.h"
#include "ui_card.h"

/***************Constructeur / Destructeur**************/
Card::Card(QWidget *parent) : QDialog(parent), ui(new Ui::Card)
{
    ui->setupUi(this);                      // Initialise l'objet de la fenêtre "Card"

    /*********Initialise et filtre le modèle de fichier pour l'arborescence************/
    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files );
    QStringList ListFiltre = {FiltreMusique};
    filemodel->setNameFilters(ListFiltre);
    filemodel->setNameFilterDisables(false);

    setWindowTitle(NameCardWindows);        // Nomme la fenêtre
}
Card::~Card()
{
    delete ui;
}


/****************** Filtre l'arborescence pour afficher que les musiques **********************/
void Card::FiltreMusic(QString CheminCard)
{
    /********** Filtre l'affichage en affichant le contenue du fichier "Music" **********/
    ui->ListMusic->setModel(filemodel);
    CheminCard.append("Music");
    ui->ListMusic->setRootIndex(filemodel->setRootPath(CheminCard));
}

/****************** Actualise l'état de la carte **********************/
void Card::ActualiseEtatCard()
{
    QString TexteEtatCarte = "\t\tConnexion\n\n\n";

    /********* Efface et actualise l'état des différentes parties de la carte contrôleur *********/
    ui->EtatCard->clear();
    TexteEtatCarte.append("\nContacte avec l'écran souple --\t-- ");
    TexteEtatCarte.append(S_Setting->EtatEcran);
    TexteEtatCarte.append("\nConnexion avec la carte controleur Jetson Nano --\t-- ");
    TexteEtatCarte.append(S_Setting->EtatConnexion);
    TexteEtatCarte.append("\nCommunication avec la carte controleur Jetson Nano --\t-- ");
    TexteEtatCarte.append(S_Setting->EtatCommunication);
    TexteEtatCarte.append("\nHaut-parleur testé --\t-- ");
    TexteEtatCarte.append(S_Setting->EtatSpeaker);
    ui->EtatCard->append(TexteEtatCarte);
}


/************************************************************ SLOTS ************************************************************************/

/****************** Demande au serveur d'émettre un son pour tester les haut-parleurs **********************/
void Card::on_SpeakerTest_clicked()
{
    QByteArray  ProtocolMessage             ;
    char        ProtocolReponseTest[200]    ;
    bool        resultat                    ;

    /****************** Si l'application n'est pas connecté au serveur **********************/
    if(card_socket->state() == card_socket->UnconnectedState)
    {
        /******************** Connexion au serveur*****************/
        card_socket->connectToHost(S_Setting->IP_Serveur,S_Setting->PortCarte.toInt(&resultat,10));
        if(card_socket->waitForConnected(3000));
    }
    /****************** Si l'application est connecté au serveur **********************/
    if(card_socket->state() == card_socket->ConnectedState)
    {
        /****************** Formulation du message **********************/
        ProtocolMessage.append(S_Setting->QSProtocolStart);     // Ecriture du début du message
        ProtocolMessage.append("0");                            // Ecriture de la difficulté
        ProtocolMessage.append(S_Setting->QSProtocolSeparate);  // Ecriture du séparateur
        ProtocolMessage.append("0");                            // Ecriture du nom de la musique
        ProtocolMessage.append(S_Setting->QSProtocolSeparate);  // Ecriture du séparateur
        ProtocolMessage.append("001");                          // Ecriture de la zone de mise à jour
        ProtocolMessage.append(S_Setting->QSProtocolSeparate);  // Ecriture du séparateur
        ProtocolMessage.append(S_Setting->QSProtocolEnd);       // Ecriture de la fin du message
        ProtocolMessage.append("\n");                           // Fin de ligne

        /******** Envoie le message au serveur************/
        if(card_socket->write(ProtocolMessage) > 0)
        {
            /******** Attend la réponse du server et vérifie le retour ************/
            card_socket->waitForBytesWritten(3000);
            if(card_socket->read(ProtocolReponseTest,200))
            {
                /******** Compare le message retour avec celui envoyé ************/
                if(strncmp(ProtocolReponseTest,ProtocolMessage.data(),ProtocolMessage.size()) == 0)
                    S_Setting->EtatSpeaker = true;                        // Actualise l'état du haut-parleur
            }
            ActualiseEtatCard();                               // Actualise l'état de la carte contrôleur
        }
    }
}

/****************** Bouton "Supprimer" **********************/
void Card::on_ButtonDelete_clicked()
{
    bool    resultat                = false                 ;
    QString CheminMusiqueComplet    = S_Setting->CheminCarte;
    QString CheminMusique                                   ;

    /********* Créer le chemin contenant la musique à supprimer *********/
    CheminMusiqueComplet.append("Music\\\\");                       // Ajoute le nom du dossier contenant les musiques
    CheminMusique = CheminMusiqueComplet;                           // Copie le chemin du fichier musique sans le nom de la musique
    CheminMusiqueComplet.append(DeleteMusic);                       // Ajoute le nom de la musique à supprimer

    /********* Supprimer le fichier musique ainsi que sa partition *********/
    resultat = QFile::remove(CheminMusiqueComplet);                 // Supprime la musique
    CheminMusiqueComplet.remove(FormatMusique,Qt::CaseInsensitive); // Retire l'extension du fichier musique
    CheminMusiqueComplet.append(FormatTexte);                       // Ajoute l'extention du fichier partition
    resultat = QFile::remove(CheminMusiqueComplet);                 // Supprime le fichier partition

    /*********Renouvelle le modèle de fichier de l'arborescence*********/
    delete(filemodel);
    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files );
    QStringList ListFiltre = {FiltreMusique};
    filemodel->setNameFilters(ListFiltre);
    filemodel->setNameFilterDisables(false);
    ui->ListMusic->setModel(filemodel);
    ui->ListMusic->setRootIndex(filemodel->setRootPath(CheminMusique));
}

/********* Affiche le nom de la musique sélectionnée pour être supprimée *********/
void Card::on_DeleteWay_textChanged(const QString &arg1)
{
    ui->DeleteWay->setText(arg1);
}

/****************** Récupère le nom de la musique sélectionné et l'affiche **********************/
void Card::on_ListMusic_doubleClicked(const QModelIndex &index)
{
    DeleteMusic = filemodel->fileName(index);
    on_DeleteWay_textChanged(filemodel->fileName(index));
}
void Card::on_ListMusic_clicked(const QModelIndex &index)
{
    on_ListMusic_doubleClicked(index);   // En cas de simple ou de double cliques, on exécute la même fonction
}
