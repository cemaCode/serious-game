#include "mainwindow.h"

/******************Afficheur Statistique**********************/
void MainWindow::AfficheStat(QFile &P_FichierStat)
{
    ui->textEditStat->clear();                  // Efface le contenue de l'afficheur

    /****** Affiche le contenu du fichier statistique ******/
    QByteArray Statistique;
    while(!P_FichierStat.atEnd())               // Tans qu'on est pas à la fin du fichier
    {
        Statistique = P_FichierStat.readLine(); // Lit la ligne
        QString ligne(Statistique);             // Convertit la ligne en QString
        ui->textEditStat->append(ligne);        // Affiche la ligne
    }
}

/*********************Afficheur Progress**************/
void MainWindow::AfficheProgress(QString P_Texte, const char* P_Color)
{
    /******** Affiche un message d'état en couleur ********/
    QString MessageColore;
    MessageColore="<span style=\"color: " + QString(P_Color) + "\">"+P_Texte+"</span>";
    ui->textEditProgress->append(MessageColore);
}

/*********************Afficheur Chemin Musique**************/
void MainWindow::AfficheCheminMusique(QString P_CheminComplet)
{
    /****** Affiche le chemin de la musique *********/
    ui->textEditChemin->clear();
    ui->textEditChemin->append(P_CheminComplet);
}

