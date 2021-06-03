#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "mainwindow.h"
#include <windows.h>


enum Type {Fichier = 0,Dossier = 1};    // Enumération pour informer si le chemin est celui d'un fichier ou d'un dossier

/**************  Nom des fenêtres ***************/
static QString NameMainWindows      = "Fenetre Principale";
static QString NameCardWindows      = "Carte";
static QString NameSettingWindows   = "Parametre";
static QString NamePrestartWindows  = "Configuration";
static QString NameContactWindows   = "Contact";
static QString NameCreditWindows    = "Credit";

/**************  format des fichiers partitions et musique ***************/
static QString FiltreMusique        = "*.WAV";
static QString FormatMusique        = ".WAV";
static QString FormatTexte          = ".txt";

/**************  Structure contenant toutes les informations utilent entre fenêtre ***************/
typedef struct{

    /************ Informations utile pour l'application ***************/
    QString     CheminCarte;            // Chemin de la carte contrôleur
    QString     LinkfileStat;           // Chemin du fichier statistiques
    QString     LinkSaveStat;           // Chemin d'enregistrement des statistiques
    QString     PortCarte;              // Port du serveur
    QString     IP_Serveur;             // IP du serveur
    QString     MusicPath;              // Chemin du fichier musique
    QString     MusicName;              // Nom de la musique
    QString     Music;                  // Chemin complet avec le nom de la musique

    /************ Protocol de communication ***************/
    QString     QSProtocolStart             =   "$%$--$%$"  ;   // Protocole définissant le début du message
    QString     QSProtocolEnd               =   "$%$..$%$"  ;   // Protocole définissant la fin du message
    QString     QSProtocolSeparate          =   "$%$//$%$"  ;   // Protocole définissant la séparation du message
    QString     QSProtocolMessageTest       =   "OK"        ;   // Protocole définissant le test du message
    QString     QSProtocolBufferCom         =   "000"       ;   // Protocole définissant la zone de mise à jour du message

    /************ Protocol de communication adapté en QByteArray ***************/
    QByteArray  QBBufferToStart             = QSProtocolStart.toLocal8Bit()       ;
    QByteArray  QBBufferToSeparate          = QSProtocolSeparate.toLocal8Bit()    ;
    QByteArray  QBBufferToEnd               = QSProtocolEnd.toLocal8Bit()         ;
    QByteArray  QBBufferToMusicName         = MusicName.toLocal8Bit()             ;
    QByteArray  QBBufferMessageMAJ          = QSProtocolBufferCom.toLocal8Bit()   ;
    QByteArray  QBBufferMessageTest         = QSProtocolMessageTest.toLocal8Bit() ;

    /************** Variable définissant l'états de fonctionnement des différentes parties du système ***************/
    QString EtatStatistique     = "false";
    QString EtatEcran           = "false";
    QString EtatConnexion       = "false";
    QString EtatCommunication   = "false";
    QString EtatSpeaker         = "false";

    /************** Variable définissant les protocoles de communication ***************/
    QByteArray      ProtocolMessageTest             ;
    QByteArray      ProtocolMessageHautParleur      ;
    QByteArray      ProtocolMessageEcran            ;


}StrucSetting;

/******************* Protocol de comunication, note de musique *************************/
static const char* note[12] =
{
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C",
};

/******************* Tableau de toute les fréquences associer aux notes de musique *************************/
static const float notefreq[120]
{
      16.35f,   17.32f,   18.35f,   19.45f,    20.60f,    21.83f,    23.12f,    24.50f,    25.96f,    27.50f,    29.14f,    30.87f,
      32.70f,   34.65f,   36.71f,   38.89f,    41.20f,    43.65f,    46.25f,    49.00f,    51.91f,    55.00f,    58.27f,    61.74f,
      65.41f,   69.30f,   73.42f,   77.78f,    82.41f,    87.31f,    92.50f,    98.00f,   103.83f,   110.00f,   116.54f,   123.47f,
     130.81f,  138.59f,  146.83f,  155.56f,   164.81f,   174.61f,   185.00f,   196.00f,   207.65f,   220.00f,   233.08f,   246.94f,
     261.63f,  277.18f,  293.66f,  311.13f,   329.63f,   349.23f,   369.99f,   392.00f,   415.30f,   440.00f,   466.16f,   493.88f,
     523.25f,  554.37f,  587.33f,  622.25f,   659.26f,   698.46f,   739.99f,   783.99f,   830.61f,   880.00f,   932.33f,   987.77f,
    1046.50f, 1108.73f, 1174.66f, 1244.51f,  1318.51f,  1396.91f,  1479.98f,  1567.98f,  1661.22f,  1760.00f,  1864.66f,  1975.53f,
    2093.00f, 2217.46f, 2349.32f, 2489.02f,  2637.02f,  2793.83f,  2959.96f,  3135.96f,  3322.44f,  3520.00f,  3729.31f,  3951.07f,
    4186.01f, 4434.92f, 4698.64f, 4978.03f,  5274.04f,  5587.65f,  5919.91f,  6271.92f,  6644.87f,  7040.00f,  7458.62f,  7902.13f,
    8372.01f, 8869.84f, 9397.27f, 9956.06f, 10548.08f, 11175.30f, 11839.82f, 12543.85f, 13289.75f, 14080.00f, 14917.24f, 15804.26f
};

/******************* Structure en-tête WAV *************************/
typedef struct WAV
{
/**********************RIFF******************/
    char    RIFF_ID[4]   = {0,0,0,0};                 // Identifiant RIFF
    DWORD   taille_Du_Fichier;                        // Taille du fichier
/**********************WAVE******************/
    char    WAVE_ID[4]   = {0,0,0,0};                 // Identifiant WAVE
/**********************fmt******************/
    char    fmt_ID[4]           = {0,0,0,0};          // Identifiant fmy
    DWORD   taille_Du_Bloc      = 0 ;                 // Taille du bloc fmt
    WORD    type_De_Compression = 0 ;                 // Type de compression :1 = PCM
    WORD    nb_voies            = 0 ;                 // Nombre de voies 2 = stéréo
    DWORD   frequence_Echantillonage = 0;             // Fréquence d'échantillonnage.(hz)
    DWORD   octetPerSecond      = 0 ;                 // Nombre d’octets par seconde.
    WORD    octetPerSample      = 0 ;                 // Nombre d’octets par échantillon
    WORD    bitPerSample        = 0 ;                 // Nombre de bits par échantillon.
    char    fact_ID[4]          = {0,0,0,0};
    DWORD   taille_Info         = 0;                  // Fréquence d'échantillonnage.(hz)
    DWORD   nombre_Sample       = 0 ;                 // Nombre d’octets par seconde.
/**********************data******************/
    char    data_ID[4]  = {0,0,0,0} ;                 // Identifiant  DATA
    DWORD   dataSize    = 0         ;                 // Taille des données
}WAV_S;

#endif // STRUCTURES_H
