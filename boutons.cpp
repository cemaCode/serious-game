#include "mainwindow.h"


/************************************************************ SLOTS ************************************************************************/

/******************Bouton "Ajouter musique"**********************/
void MainWindow::on_AddMusic_clicked()
{
    AjouterMusique(S_Setting.Music);            // Ajoute une musique
}

/**************Bouton "Enregistrement"*************/
void MainWindow::on_SaveStat_clicked()
{
    SauvegardeStatistique();                    // Sauvegarde les statistiques dans le fichier indiqué dans les paramètres
}

/**************Bouton "Actualisation"*************/
void MainWindow::on_Actualiser_clicked()
{
    ActualisationStatistique();                 // Actualise les statistique
}
