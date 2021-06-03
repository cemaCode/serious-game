#include "mainwindow.h"
#include "card.h"
#include "setting.h"
#include "prestartgame.h"

/************************Fenetre 'Paramètre'************************/
void MainWindow::on_actionParam_tre_triggered()
{
    /******* Initialisation de la fenêtre "Setting" ******/
    Setting SettingWindows;                 // Créer l'objet de la fenêtre
    SettingWindows.setModal(true);          // Rend visible la fenêtre
    SettingWindows.S_Setting = &S_Setting;  // Récupère l'adresse de la structure paramètre
    SettingWindows.AfficheParametre();      // Affiche les paramètres
    SettingWindows.exec();                  // Ouvre la fenêtre
}

/************************Fenetre 'Enregistrer'************************/
void MainWindow::on_actionEnregistrer_triggered()
{
    SauvegardeStatistique();                // Sauvegarde les statistiques
}

/************************Fenetre 'Enregistrer Sous'************************/
void MainWindow::on_actionEnregistrer_sous_triggered()
{
    /************* Ouvre un explorateur de fichier pour récupérer l'emplacement sélectionné ********/
    QString dossierCourant = QFileDialog::getExistingDirectory(this);

    /************* Si un dossier est sélectionné ********/
    if(!dossierCourant.isNull())
    {
        /************* Sauvegarde les statistiques à l'emplacement sélectionné ********/
        dossierCourant = cheminMachine(dossierCourant, Dossier);
        SauvegardeStatistique(dossierCourant);
    }
}

/************************Fenetre 'Importer'************************/
void MainWindow::on_actionImporter_triggered()
{
    /************* Ouvre un explorateur de fichier pour récupérer un fichier sélectionné ********/
    QStringList dossierCourant = QFileDialog::getOpenFileNames(this, tr("Open File"), "",tr("Texte (*.txt)"));

    /************* Si un fichier est sélectionné ********/
    if(dossierCourant.length() > 0)
    {
        /************* Récupère le nom du fichier et l'ouvre ********/
        dossierCourant[0] = cheminMachine(dossierCourant[0], Fichier);
        ActualisationStatistique(dossierCourant[0]);
    }
}

/************************Fenetre 'Contact'************************/
void MainWindow::on_actionContact_triggered()
{
    /***** Ouvre une fenêtre affichant l'adresse mail du développeur *****/
    QMessageBox msgBox;
    msgBox.setText("Développeur : lemiere.erwan@gmail.com");
    msgBox.exec();
}

/************************Fenetre 'Card'************************/
void MainWindow::on_actionCarte_triggered()
{
    QByteArray  ProtocolMessage;
    bool        resultat;
    char        ProtocolReponseTest[200];
    Card        CardWindows;                        // Créer l'objet de la fenêtre

    AfficheProgress("Connexion avec la carte en cour...","GREEN");

    CardWindows.setModal(true);                     // Rend visible la fenêtre
    CardWindows.S_Setting = &S_Setting;             // Récupère l'adresse de la structure paramètre
    CardWindows.card_socket = &mainwindows_socket;  // Récupère l'adresse du socket du serveur
    CardWindows.FiltreMusic(S_Setting.CheminCarte); // Filtre les fichiers musique

    /****************** Si l'application n'est pas connecté au serveur **********************/
    if(mainwindows_socket.state() == mainwindows_socket.UnconnectedState)
    {
        /******************** Connexion au serveur*****************/
        mainwindows_socket.connectToHost(S_Setting.IP_Serveur,S_Setting.PortCarte.toInt(&resultat,10));
        if(mainwindows_socket.waitForConnected(3000))
            AfficheProgress("Connexion avec la carte reussi","GREEN");
    }

    /****************** Si l'application est connecté au serveur **********************/
    if(CardWindows.card_socket->state() == CardWindows.card_socket->ConnectedState)
    {
        CardWindows.S_Setting->EtatConnexion = "true";         // Actualisation de l'état de la connexion

        /****************** Formulation du message de test **********************/
        ProtocolMessage.append(S_Setting.QSProtocolStart);
        ProtocolMessage.append(S_Setting.QSProtocolMessageTest);
        ProtocolMessage.append(S_Setting.QSProtocolEnd);
        ProtocolMessage.append("\n");

        /******** Envoie le message au serveur************/
        if(CardWindows.card_socket->write(ProtocolMessage) != 0)
        {
            AfficheProgress("Communication avec la carte réussi","GREEN");
            CardWindows.S_Setting->EtatCommunication = "true";// Actualisation de l'état de la communication
        }

        /**********Vérification de la réponse du serveur*******/
        if(CardWindows.card_socket->read(ProtocolReponseTest,200) < 0)
            AfficheProgress("Pas de réponse de la carte...","RED");

        /******** Compare le message retour avec celui envoyé ************/
        if(strncmp(ProtocolReponseTest,ProtocolMessage.data(),static_cast<size_t>(ProtocolMessage.size())) != 0)
            AfficheProgress("La carte répond une valeur errone... Vérifier la connexion","RED");

        /********** Actualisation de l'état de la carte *******/
        CardWindows.ActualiseEtatCard();
        CardWindows.exec();                         // Ouvre la fenêtre
    }
    else
        AfficheProgress("Connexion avec la carte echoue...\nVérifier la connexion","RED");
}

/************************Fenetre "prestartgame"************************/
void MainWindow::on_StartGame_clicked()
{
    QByteArray  ProtocolMessage;
    char            ReponseServeur[200]             ;
    bool        resultat;

    AfficheProgress("Connexion avec la carte en cour...","GREEN");

    /****************** Si l'application n'est pas connecté au serveur **********************/
    if(mainwindows_socket.state() == mainwindows_socket.UnconnectedState)
    {
        /******************** Connexion au serveur*****************/
        mainwindows_socket.connectToHost(S_Setting.IP_Serveur,S_Setting.PortCarte.toInt(&resultat,10));
        if(mainwindows_socket.waitForConnected(3000))
        {
            AfficheProgress("Connexion avec la carte reussi","GREEN");

            PreStartGame PreGameWindows;                            // Créer l'objet de la fenêtre
            PreGameWindows.setModal(true);                          // Rend visible la fenêtre
            PreGameWindows.S_Setting = &S_Setting;                  // Récupère l'adresse de la structure paramètre
            PreGameWindows.prestart_socket = &mainwindows_socket;   // Récupère l'adresse du socket du serveur
            PreGameWindows.FiltreMusic(S_Setting.CheminCarte);      // Filtre les fichiers musique

            if(mainwindows_socket.write(S_Setting.ProtocolMessageTest) != 0)
                AfficheProgress("Communication avec la carte réussi","GREEN");

            /*********** Connexion de la fonction "MessageServeur" au SLOT "readyReady" **********/
            if(mainwindows_socket.waitForReadyRead(3000))
                PreGameWindows.exec();          // Ouvre la fenêtre
            else
                AfficheProgress("Connexion avec la carte echoue...\nVérifier la connexion","RED");
        }
         else
             AfficheProgress("Connexion avec la carte echoue...\nVérifier la connexion","RED");
    }
    else
    {
        /****************** Si l'application est connecté au serveur **********************/
        if(mainwindows_socket.state() == mainwindows_socket.ConnectedState)
        {
            AfficheProgress("Connexion avec la carte reussi","GREEN");

            PreStartGame PreGameWindows;                            // Créer l'objet de la fenêtre
            PreGameWindows.setModal(true);                          // Rend visible la fenêtre
            PreGameWindows.S_Setting = &S_Setting;                  // Récupère l'adresse de la structure paramètre
            PreGameWindows.prestart_socket = &mainwindows_socket;   // Récupère l'adresse du socket du serveur
            PreGameWindows.FiltreMusic(S_Setting.CheminCarte);      // Filtre les fichiers musique

            if(mainwindows_socket.write(S_Setting.ProtocolMessageTest) != 0)
                AfficheProgress("Communication avec la carte réussi","GREEN");

            /*********** Connexion de la fonction "MessageServeur" au SLOT "readyReady" **********/
            if(mainwindows_socket.waitForReadyRead(3000))
                PreGameWindows.exec();          // Ouvre la fenêtre
            else
                AfficheProgress("Connexion avec la carte echoue...\nVérifier la connexion","RED");                                // Ouvre la fenêtre
        }
        else
            AfficheProgress("Connexion avec la carte echoue...\nVérifier la connexion","RED");
    }
}
