#include "mainwindow.h"

/********* Sauvegarde les statistiques *************/
void MainWindow::SauvegardeStatistique(QString CheminAnnexe)
{
    QString NewfileName;

    ui->progressBar->setValue(0);                           // Barre de progression à 0%

    /********* Si aucun chemin n'est en paramètre, on récupère le chemin par défaut *************/
    if(CheminAnnexe.compare("",Qt::CaseInsensitive))
        NewfileName = S_Setting.LinkSaveStat;
    else
        NewfileName = CheminAnnexe;

    QString fileName = S_Setting.LinkfileStat;                  // Récupération de l'emplacement des statistiques
    fileName.append("\\Stat.txt");                              // Ajout du fichier contenant les statistiques

    /******** Définition du nom de fichier *********/
    QString Date = QDate::currentDate().toString("yyyy_MM_dd"); // Récupération de la date
    NewfileName.insert((NewfileName.size()),"StatGame_");
    NewfileName.insert((NewfileName.size()),Date);
    NewfileName.insert((NewfileName.size()),FormatTexte);

   /********* Si le fichier existe déjà, on le supprime **********/
    if (QFile::exists(NewfileName))
        QFile::remove(NewfileName);

    /*** Copie du fichier à l'emplacement spécifié *********/
    if(QFile::copy(fileName,NewfileName))
    {
        AfficheProgress("Enregistrement des stats réussi","GREEN");
        ui->progressBar->setValue(100);                     // Barre de progression à 100%
    }
    else
        AfficheProgress("Enregistrement des stats n'a pas fonctionne","RED");
}

/********* Actualise l'affichage des statistiques *************/
void MainWindow::ActualisationStatistique(QString CheminAnnexe)
{
    ui->progressBar->setValue(0);                   // Barre de progression à 0%

    /********* Si aucun chemin n'est en paramètre, on récupère le chemin par défaut *************/
    if(CheminAnnexe.compare("",Qt::CaseInsensitive))
    {
        CheminAnnexe = S_Setting.LinkfileStat;
        CheminAnnexe.append("\\Statistique.txt");
    }

    /****** Création de l'objet fichier *********/
    QFile FichierStat(CheminAnnexe);

    /****** Ouvre le fichier statistique *********/
    if(FichierStat.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        AfficheProgress("Ouverture du fichier Stat","green");
        ui->progressBar->setValue(70);              // Barre de progression à 70%
        AfficheStat(FichierStat);                   // Affiche les statistisques
        FichierStat.close();                        // Ferme le fichier statistiques
        ui->progressBar->setValue(100);             // Barre de progression à 100%
    }
    else
        AfficheProgress("Erreur lors de l'ouverture du fichier stat\nVérifier la connection avec la carte...","red");
}
