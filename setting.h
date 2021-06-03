#ifndef SETTING_H
#define SETTING_H

#include "mainwindow.h"
#include "ui_setting.h"
#include "structures.h"
#include <QFileDialog>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:

    explicit Setting(QWidget *parent = nullptr);                        // Constructeur
    ~Setting();                                                         // Destructeur

    QString         CheminParametre       =     "\\Setting.txt";        // Fichier contenant les paramètres
    QString         ParametreAChanger           ;                       // Variable contenant le paramètre à changer
    StrucSetting*   S_Setting                   ;                       // Pointeur sur les informations utiles de l'application et de la carte contrôleur

    void            AfficheParametre            ();                     // Affiche tout les paramètres
    void            ChangeParametre             (QString P_Parametre);  // Change le paramètre qui se trouve en paramètre

    /********** Affichages des paramètres **********/
    void            on_CarLink_textChanged      (const QString &arg1);  // Chemin de la carte contrôleur
    void            on_StatLink_textChanged     (const QString &arg1);  // Chemin statistiques
    void            on_StatSaveLink_textChanged (const QString &arg1);  // Chemin de sauvegarde des statistiques
    void            on_PortCarte_textChanged    (const QString &arg1);  // Port du serveur


private slots:

    /************** Clique sur le bouton "modifier" **************/
    void            on_ChangeCardLink_clicked       ();                     // Chemin de la carte contrôleur
    void            on_ChangeStatLink_clicked       ();                     // Chemin statistiques
    void            on_ChangeStatSaveLink_clicked   ();                     // Chemin de sauvegarde des statistiques
    void            on_ChangePortCard_clicked       ();                     // Port du serveur

private:

    Ui::Setting *ui;            // Pointeur sur les fonctions de la fenêtre "Setting.ui"
};

#endif // SETTING_H
