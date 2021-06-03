#ifndef PRESTARTGAME_H
#define PRESTARTGAME_H

#include "mainwindow.h"
#include "ui_prestartgame.h"

namespace Ui {
class PreStartGame;
}

class PreStartGame : public QDialog
{
    Q_OBJECT

public:

    explicit    PreStartGame(QWidget *parent = nullptr);
    ~PreStartGame();

    bool        LancemenJeu     ();                                     // Démarre la fenêtre du choix de la musique
    void        FiltreMusic     (QString CheminCard);                   // Filtre pour afficher que les musiques
    void        AfficheProgress (QString P_Texte, const char* P_Color); // Affichage de l'état de la carte

    QTcpSocket*     prestart_socket;    // Pointeur sur le socket connecté à la carte contrôleur
    StrucSetting*   S_Setting;          // Pointeur sur les informations utiles de l'application et de la carte contrôleur

private slots:

    void MessageServeur             (); // Envoie le nom de la musique et la difficulté à la carte contrôleur

    /*****Clique sur la musique******/
    void on_ListMusic_clicked       (const QModelIndex &index);
    void on_ListMusic_doubleClicked (const QModelIndex &index);

    /*****Clique sur "lancer la partie" ******/
    void on_StartGame_clicked       ();

private:

    Ui::PreStartGame*   ui;             // Pointeur sur les fonctions de la fenêtre "Prestartgame.ui"
    QFileSystemModel*   filemodel;      // Pointeur sur le système de fichier affiché dans la fenêtre
    QFileSystemModel*   dirmodel;       // Pointeur sur le système de fichier affiché dans la fenêtre
};

#endif // PRESTARTGAME_H
