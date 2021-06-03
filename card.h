#ifndef CARD_H
#define CARD_H

#include "mainwindow.h"
#include "ui_card.h"

namespace Ui {
class Card;
}

class Card : public QDialog
{
    Q_OBJECT

public:

    explicit Card(QWidget *parent = nullptr);   // Constructeur
    QTcpSocket      *card_socket;               // Pointeur sur le socket connecté à la carte contrôleur
    StrucSetting    *S_Setting;                 // Pointeur sur les informations utiles de l'application et de la carte contrôleur


    QString DeleteMusic;                        // Variable stockant le nom de la musique à supprimer
    //QString AdressCard ;                      //
    void    FiltreMusic(QString CheminCard);    // Filtre les musiques à partir du chemin en paramètre
    void    ActualiseEtatCard();                // Actualise l'afffichage des états de la carte contrôleur

    ~Card();                                    // Destructeur

private slots:

    void on_ButtonDelete_clicked    ();                         // Bouton "Supprimer"
    void on_DeleteWay_textChanged   (const QString &arg1);      // Affiche le nom de la musique sélectionnée
    void on_ListMusic_clicked       (const QModelIndex &index); // Appuis sur l'afficheur de musiques
    void on_ListMusic_doubleClicked (const QModelIndex &index); // Double appuis sur l'afficheur de musiques

    void on_SpeakerTest_clicked();

private:
    Ui::Card*           ui;                     // Pointeur sur les fonctions de la fenêtre "Card.ui"
    QFileSystemModel*   filemodel;              // Pointeur sur le système de fichier affiché dans la fenêtre
};

#endif // CARD_H
