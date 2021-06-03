#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <windows.h>
#include "structures.h"
#include <fft/fftw/fftw3.h>
#include <complex.h>

#include <QMainWindow>
#include <QFile>
#include <QIODevice>
#include <QDialog>
#include <QTextStream>
#include <QDataStream>
#include <QTcpSocket>
#include <QFileSystemModel>
#include <QDate>
#include <QMessageBox>
#include <QWidget>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr); // Constructeur
    ~MainWindow();                                  // Destructeur

    StrucSetting    S_Setting;                      // Structure contenant les informations utiles de l'application et de la carte contrôleur
    QTcpSocket      mainwindows_socket;             // Pointeur sur le socket connecté à la carte contrôleur

private slots:

    /****** Boutons de la fenêtre principale *********/
    void on_AddMusic_clicked    ();
    void on_StartGame_clicked   ();
    void on_SaveStat_clicked    ();
    void on_Actualiser_clicked  ();

    /*************** Onglet supplémentaire de la fenêtre principale ******************/
    void on_actionParam_tre_triggered       ();
    void on_actionContact_triggered         ();
    void on_actionEnregistrer_triggered     ();
    void on_actionEnregistrer_sous_triggered();
    void on_actionImporter_triggered        ();
    void on_actionCarte_triggered           ();

    /*************appuis sur l'arborescence*********/
    void on_treeView_clicked        (const QModelIndex &index);
    void on_treeView_doubleClicked  (const QModelIndex &index);

private:

    /************ Fonction d'affichage ********/
    void AfficheStat            (QFile &P_FichierStat)                  ;   // Affichage des statistiques
    void AfficheProgress        (QString P_Texte, const char* P_Color)  ;   // Affichage de l'état de la carte
    void AfficheCheminMusique   (QString P_CheminComplet)               ;   // Affichage du chemin de la musique dans la barre prévu pour cela
    void ActualisationStatistique   (QString CheminAnnexe = "")         ;   // Actualise l'affichage des statistiques

    void AjouterMusique             (QString P_CheminComplet)           ;   // Ajoute la musique mit en paramètre
    bool StartSetting               ()                                  ;   // Recherche le fichier paramètre et les enregistre
    void SauvegardeStatistique      (QString CheminAnnexe = "")         ;   // Sauvegarde les statistiques

    Ui::MainWindow      *ui;                        // Pointeur sur les fonctions de la fenêtre "Card.ui"
    QFileSystemModel    *dirmodel;                  // Pointeur sur le système de fichier affiché dans la fenêtre
};

QString         cheminMachine(QString CheminStandard, bool P_FileOrDir);                                                        // Adapte un chemin pour qu'il soit compréhenssible par la carte contrôleur
const char*     convertionNoteSchemat(int P_NbEchantillonCollecte, int P_PositionFondamentale,int P_Frequence_Echantillonage ); // retourne une note suivant ces paramètres

#endif // MAINWINDOW_H
