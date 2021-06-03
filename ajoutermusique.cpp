#include "mainwindow.h"

void MainWindow::AjouterMusique(QString P_CheminComplet)
{
    QString     MessageProgress         ;                           // QString contenant l'état de la carte contrôleur
    QString     CheminCarteMusique      ;                           // Chemin complet de la musique
    QByteArray  PartitionComplet        ;                           // Tableau contenant la partition
    QByteArray  FoundData       = "data";                           // Nom 'data' pour trouver le début des informations du signal de la musique
    WAV_S       StructureFichierMusique ;                           // Structure en-tête

    /******* Variable pour la transformation de fourrier ********/
    fftw_complex*   TableauComplexOut               ;               // Tableau de complex - en sortie de la transformation de fourrier
    fftw_plan       planfft                         ;               // Plan de la FFT
    ulong           indicePositionData      = 0     ;               // Indice de positionnement dans le fichier musique
    unsigned int    indiceTableauDatafftIn  = 0     ;               // Indice de positionnement dans le tableau d'entrer des données

    /******* Variable pour les données de la musique ********/
    unsigned int    NbEchantillonCollecte   = 0     ;               // Nombre d'échantillion collecté
    int             PositionFondamentale    = 0     ;               // Position de la fondamentale
    int             PositionActuel          = 0     ;               // Position actuel pour la recherche de ma fondamentale
    double          ValeurLaPlusHaute       = 0.0   ;               // Valeur la plus haut pour trouver la fondamentale
    ulong           NbSecondeMusique        = 0     ;               // Nombre de seconde de musique
    ulong           indiceForNbSecondeMusique       = 0     ;       // Indice pour le nom de seconde de musique
    ulong           indiceForRechercheFondamental   = 0     ;       // Indice pour trouver la note fondamentale

    ulong           AntiBoucleInfini        = 0     ;               // Variable pour éviter la boucle infinie
    qint8           oct1                            ;               // Variable pour contenir un octet
    qint8           oct2                            ;               // Variable pour contenir un octet
    short int       PuissanceDe2            = 10    ;               // Puissance de 2 pour définir le nombre d'échantillionnage

    ui->progressBar->setValue(0);                               // Barre de progression à 0
    if(S_Setting.MusicName.isNull())                            // Si le nom de la musique n'est pas vide
        AfficheProgress("Aucune musique sélectioné","RED");     // Affiche l'état de la carte contrôleur
    else
    {
        if(S_Setting.MusicName.endsWith(FormatMusique,Qt::CaseInsensitive))             // Si la musique est bien conforme au format définie
        {
            /***** Créer le chemin complet de la musique ******/
            CheminCarteMusique.insert(CheminCarteMusique.size(),S_Setting.CheminCarte);
            CheminCarteMusique.insert(CheminCarteMusique.size(),"Music\\");
            CheminCarteMusique.insert(CheminCarteMusique.size(),S_Setting.MusicName);

            if (QFile::exists(CheminCarteMusique))                                      // Vérifie si la musique est déjà présente sur la carte
                AfficheProgress("La musique est déjà présente dans la carte","RED");    // Affiche l'état de la carte contrôleur
            else
            {
                /***** Affiche l'état de la carte contrôleur ******/
                MessageProgress.insert(MessageProgress.size(),"Ajout de la musique ");
                MessageProgress.insert(MessageProgress.size(),S_Setting.MusicName);
                MessageProgress.insert(MessageProgress.size()," en cour...");
                AfficheProgress(MessageProgress, "Green");

                ui->progressBar->setValue(20);                                          // Barre de progression à 20%

                /********* Adapte les noms de chemins *********/
                P_CheminComplet     = cheminMachine(P_CheminComplet,Fichier);
                CheminCarteMusique  = cheminMachine(CheminCarteMusique,Fichier);

                /********* Copie la musique sur la carte contrôleurs *********/
                if (QFile::copy(P_CheminComplet,CheminCarteMusique))
                    AfficheProgress("Copie réussite","green");                          // Affiche l'état de la carte contrôleur
                else
                    AfficheProgress("Erreur lors de la copie","red");                   // Affiche l'état de la carte contrôleur

/*************************** Convertisseur musicale ***************************/

                /******* Ouvre la musique et un fichier de partition ********/
                QFile FichierMusique(P_CheminComplet);
                CheminCarteMusique.remove(FormatMusique,Qt::CaseInsensitive);
                CheminCarteMusique.append(FormatTexte);
                QFile FichierPartitionMusique(CheminCarteMusique);

                /******* Ouvre le fichier musique ********/
                if(FichierPartitionMusique.open(QIODevice::ReadWrite))
                {
                    /******* Si la musique s'est correctement ouverte, ouvre le fichier de partition ********/
                    if(FichierMusique.open(QIODevice::ReadOnly))
                    {
                        /********* Création des Streams **************/
                        QTextStream     StreamFichierMusique(&FichierMusique);          // Stream pour le fichier musique
                        QDataStream     StreamDataFichierMusique(&FichierMusique);      // Stream pour les informations du fichier musique

                        /********* Copie l'en-tête du fichier musique dans la structure d'en-tête **************/
                        FichierMusique.read((char*)&StructureFichierMusique,sizeof(StructureFichierMusique));

                        /*********Recherche la position des données musicale du fichier musique **************/
                        while(StreamFichierMusique.readLine(4) != FoundData && AntiBoucleInfini < 300)
                        {
                            StreamFichierMusique.seek(++indicePositionData);
                            AntiBoucleInfini++ ;
                        }

                        /******* Si les données musical ne sont pas trouvées ********/
                        if(AntiBoucleInfini == 300)
                            AfficheProgress("Erreur lors de la copie","RED");
                        else
                        {
                            /*************** Définit le nombre d'échantillons et le nombre de boucle *****************/
                            NbEchantillonCollecte   = static_cast<unsigned int>(pow(2,PuissanceDe2));
                            NbSecondeMusique        = StructureFichierMusique.taille_Du_Fichier / StructureFichierMusique.octetPerSecond;
                            double TableauDataIn[NbEchantillonCollecte];

/************************************ TFD ***************************************/

                            /******* Pour toutes les secondes de la musique ********/
                            while(indiceForNbSecondeMusique < NbSecondeMusique)
                            {
                                indiceForNbSecondeMusique++;

                                /******* Alloue dynamiquement un tableau de complex pour contenir les données de sortie de la TFD ********/
                                TableauComplexOut = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * NbEchantillonCollecte));

                                /******* Boucle au nombre d'échantillons ********/
                                indiceTableauDatafftIn = 0;
                                while(indiceTableauDatafftIn < NbEchantillonCollecte)
                                {
                                    /******* Récupère les 2 octets de données et les places dans le tableau de donnée ********/
                                    StreamDataFichierMusique >> oct1;
                                    StreamDataFichierMusique >> oct2;
                                    TableauDataIn[indiceTableauDatafftIn] = oct1 + (oct2 << 8);
                                    indiceTableauDatafftIn++;
                                }

                                /**************** Associe les données au plan pour effectuer la TFD ****************/
                                planfft = fftw_plan_dft_r2c_1d(NbEchantillonCollecte, TableauDataIn, TableauComplexOut, FFTW_ESTIMATE);

                                /**************** Exécute la TFD ****************/
                                fftw_execute(planfft);

                                /******* Pour tout les échantillons ********/
                                indiceForRechercheFondamental=0;
                                while(indiceForRechercheFondamental < NbEchantillonCollecte)
                                {
                                    indiceForRechercheFondamental ++;

                                    /**************** Recherche l'amplitude la plus haute ****************/
                                    if(ValeurLaPlusHaute > *static_cast<double*>(TableauComplexOut[indiceForRechercheFondamental]))
                                        PositionActuel++;
                                    else
                                    {
                                        ValeurLaPlusHaute = *static_cast<double*>(TableauComplexOut[indiceForRechercheFondamental]);
                                        PositionActuel++;
                                        PositionFondamentale = PositionActuel; // A voir
                                    }
                                }
                                ValeurLaPlusHaute   = 0.0   ;
                                PositionActuel      = 0     ;

                                /***** Recherche la note de musique correspondante à la fondamentale ****/
                                PartitionComplet += convertionNoteSchemat(NbEchantillonCollecte, PositionFondamentale,StructureFichierMusique.frequence_Echantillonage );
                                PartitionComplet += ",";

                                /**************** Libère les données ****************/
                                fftw_destroy_plan(planfft);
                                fftw_cleanup();
                                fftw_free(TableauComplexOut);
                            }
                        }
                        /******* Inscrit les notes de musique dans le fichier partition ********/
                        FichierPartitionMusique.write(PartitionComplet.data());
                        FichierPartitionMusique.close();
                    }
                }
                ui->progressBar->setValue(100);     // Barre de progression à 100%
            }
        }
        else
            AfficheProgress("La musique choisi n'est pas au bon format", "Red");
    }
}

/****************** Fonction pour rechercher une note de musique à partir d'une fréquence fondamentale **********************/
const char* convertionNoteSchemat(int P_NbEchantillonCollecte, int P_PositionFondamentale,int P_Frequence_Echantillonage )
{
    int     FrequenceFondamental        = (P_Frequence_Echantillonage / P_NbEchantillonCollecte) * P_PositionFondamentale;
    int     IndicePositionNoteActuel    = 0;
    int     IndicePositionNoteFinal     = 0;

    /****************** Recherche la note ***********/
    for(int Indicefor=0; Indicefor < 120; Indicefor++)
    {
        if((FrequenceFondamental - static_cast<int>(notefreq[Indicefor])) > 0)
            IndicePositionNoteFinal = IndicePositionNoteActuel;

        IndicePositionNoteActuel++;
    }
    IndicePositionNoteFinal++;

    /*********** Retourne la position par rapport au protocole inscrit ****************/
    return note[IndicePositionNoteFinal % 12];
}

/************************************************************ SLOTS ************************************************************************/

/****************** Affichage d'ajout d'une musique **********************/
void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    S_Setting.MusicPath = dirmodel->fileInfo(index).absolutePath();                                 // Récupère le chemin sélectionné
    S_Setting.MusicName = dirmodel->fileName(index);                                                // Récupère le nom de la musique

    /*********** Assemble le chemin complet de la musique ***********/
    S_Setting.Music = S_Setting.MusicPath.insert(S_Setting.MusicPath.size(), "/");
    S_Setting.Music = S_Setting.MusicPath.insert(S_Setting.MusicPath.size(), S_Setting.MusicName);

    AfficheCheminMusique(S_Setting.Music);                                                          // Affiche le chemin complet de la musique
}
void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    on_treeView_doubleClicked(index);                                                               // En cas de simple ou de double cliques, on exécute la même fonction
}
